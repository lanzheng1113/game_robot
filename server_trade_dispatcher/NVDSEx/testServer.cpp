#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>
using std::string;
using std::cout;
using std::endl;
using std::map;

#include "test.h"
#include "session.h"
#include "IOCPServer.h"

#include "../common/DebugMessage.h"
#include "../common/NetBase.h"
//#include "SendFun.h"
#include "../common/shareStruct.h"
#include "../common/tinyxml/include/tinyxml.h"
#include "../common/tinyxml/include/tinystr.h"
#include "../common/adodb.h"
#include "../common/TradeMesgDef.h"
#include "../common/StringTool.h"
#include "util/Logger.h"

#include "DlgExitProgress.h"
#include "TradeAffair.h"
#include "TradeAffairMgr.h"
#include "testServer.h"
#include "DBM.h"
#include "dlglogin.h"
#include "OrderDispatcher.h"
#include "ObjectFactory.h"
#include "TradeMacDispatcher.h"
#include "ConfirmDispatcher.h"

map<string,string> g_ServerParameterMap;
NVDSNetworkObject::NVDSNetworkObject()
{
	m_TradeAffair = NULL;
}

int i_order_new_counter = 0;
NVDSNetworkObject::~NVDSNetworkObject(){
	//DebugMessageA("[NVDSNetworkObject] ~NVDSNetworkObject");
}

VOID NVDSNetworkObject::OnDisconnect()
{
	//DebugMessageA("[NVDSNetworkObject][OnDisconnect]");
	if (m_TradeAffair){
		m_TradeAffair->SetNetworkCallback(NULL);
		m_TradeAffair->OnSessionClosed();
	}
}


VOID NVDSNetworkObject::OnRecv( BYTE *pMsg, WORD wSize )
{
	if (wSize == 6 && *(WORD*)(pMsg+4) == heart)
	{
		//心跳包，大小=6以减少服务器负担
	}
	else if(wSize < sizeof(NET_HEAD)){
		Disconnect();
		return;
	}

	DWORD AffairType = *(DWORD*)pMsg;
	switch(AffairType)
	{
	case message_tag_trade:
		{
			if (!m_TradeAffair){
				m_TradeAffair = new CTradeAffair(afkGetTradeAffairManager(),
												CObjectFactory::getInstance().GetMacDispatcher(),
												CObjectFactory::getInstance().GetOrderDispatcher());
				if (!m_TradeAffair){
					Disconnect();
					return;
				}
				afkGetTradeAffairManager()->AddAffair(m_TradeAffair);
				m_TradeAffair->SetNetworkCallback(this);
			}
			m_TradeAffair->OnDataRecv(pMsg,wSize);
			break;
		}
	default://unknow packet.
		Disconnect();
		break;
	}
}

void NVDSNetworkObject::OnSessionSetup()
{
	DebugMessageA("sesson %d setup!",GetSession()->GetSocket());
}

NetworkCallBack* NVDSCreateAcceptedObject(){
	if (!CObjectFactory::getInstance().isObjectInit())
	{
		return NULL;
	}
	NVDSNetworkObject* cbk = new NVDSNetworkObject();
	//cbk->OnSessionSetup();
	return cbk;
}

VOID NVDSDestroyAcceptedObject(NetworkCallBack *pNetworkObject){
	delete pNetworkObject;
};


IocpServer server;
IOHANDLER_DESC g_desc;

BOOL InitSrvGlobalParameter(){

	char moduleName[MAX_PATH];
	GetModuleFileNameA(NULL, moduleName, sizeof(moduleName));
	string strAppDir(moduleName);
	string::size_type pos = strAppDir.rfind('\\');

	if (pos == string::npos || pos == strAppDir.length() - 1) {
		;
	} else {
		strAppDir = strAppDir.substr(0, pos);
	}
	strAppDir += "\\NVDSEx.xml";
	//
	DebugMessageA("%s",strAppDir.c_str());
	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument(strAppDir.c_str());
	if (!myDocument)
	{
		AfxMessageBox(L"创建XML配置对象失败！");
		return FALSE;
	}
	DebugMessageA("open trade server main configure file.%s\n",strAppDir.c_str());
	//AfxMessageBox(L"创建配置对象成功！");
	if (!myDocument->LoadFile())
	{
		AfxMessageBox(L"打开配置文件NVDSEx.xml失败！");
		delete myDocument;
		return FALSE;
	}

	//AfxMessageBox(L"打开配置文件NVDSEx.xml成功！");
	//获得根元素

	TiXmlElement *RootElement = myDocument->RootElement();
	if (RootElement)
	{
		//根元素
		//获得第一个子节点。
		TiXmlElement *Element = RootElement->FirstChildElement();
		while (Element)
		{
			g_ServerParameterMap[Element->Value()] = Element->GetText();
			DebugMessageA("%s - %s\n",Element->Value(),Element->GetText());
			Element = Element->NextSiblingElement();
		}
	}
	delete myDocument;
	return TRUE;
}

void InitServerParameter(IOHANDLER_DESC* desc){

	
	desc->dwMaxAcceptSession = atoi(g_ServerParameterMap[string("MaxAcceptSession")].c_str());
	desc->dwMaxPacketSize = atoi(g_ServerParameterMap[string("MaxPacketSize")].c_str());
	desc->dwNumberOfIoThreads = atoi(g_ServerParameterMap[string("NumberOfIoThread")].c_str());
	desc->dwRecvBufferSize = atoi(g_ServerParameterMap[string("RecvBufferSize")].c_str());
	desc->dwSendBufferSize = atoi(g_ServerParameterMap[string("SendBufferSize")].c_str());
	desc->usListenPort = (USHORT)atoi(g_ServerParameterMap[string("ListenPort")].c_str());
	desc->dwTimeOut = (DWORD)atoi(g_ServerParameterMap[string("SessionTimeOut")].c_str());


	if (desc->dwMaxAcceptSession == 0){
		AfxMessageBox(L"配置错误！Max accept session = 0");
		ExitProcess(1);
	}

	if (desc->dwMaxPacketSize == 0)
	{
		AfxMessageBox(L"配置错误！Max packet size = 0");
		ExitProcess(1);
	}

	if (desc->dwRecvBufferSize == 0 || desc->dwSendBufferSize == 0)
	{
		AfxMessageBox(L"配置错误！Recv buffer size or send buffer size = 0");
		ExitProcess(1);
	}

	if (desc->dwNumberOfIoThreads == 0)
	{
		AfxMessageBox(L"配置错误！io thread = 0");
		ExitProcess(1);
	}

	if (desc->usListenPort == 0)
	{
		AfxMessageBox(L"配置错误！listen port = 0");
		ExitProcess(1);
	}

	return;
}

BOOL RunServer()
{
	memset(&g_desc,0,sizeof(IOHANDLER_DESC));

	InitServerParameter(&g_desc);
	
	string strDbName = g_ServerParameterMap["dataBaseName"];
	string strDbType = g_ServerParameterMap["dataBaseType"];

	int dbType = atoi(strDbType.c_str());
	
	CString strAcc;
	CString strPswd;
	CString strDataBaseName;

	//登录数据库
	CDlgLogin dlg;
	if (dlg.DoModal() != IDOK) //do not execute this before 	CNVDSExDlg dlg; m_pMainWnd = &dlg; or it iwll make debug assert;
	{
		return FALSE;
	}
	strAcc = dlg.m_strAcc;
	strPswd = dlg.m_strPassword;

	if (dbType == E_DB_TYPE_ACCESS)
	{
		TCHAR szDbName[MAX_PATH] = {0};
		char moduleName[MAX_PATH];
		GetModuleFileNameA(NULL, moduleName, sizeof(moduleName));
		string strAppDir(moduleName);
		string::size_type pos = strAppDir.rfind('\\');
		if (pos == string::npos || pos == strAppDir.length() - 1) {
			;
		} else {
			strAppDir = strAppDir.substr(0, pos);
		}
		strAppDir += "\\";
		strAppDir += strDbName;

		MultiByteToWideChar(CP_ACP,0,strAppDir.c_str(),strAppDir.length()+1,szDbName,_countof(szDbName));
		strDataBaseName = szDbName;
	}else if (dbType == E_DB_TYPE_MYSQL){
		strDataBaseName = StringToCString(strDbName);
	}else{
		ASSERT(0);
	}

	DebugMessageW(L"DbName:%s - dbType:%d\n",(LPCTSTR)strDataBaseName,dbType);
	
	BOOL bIni = CObjectFactory::getInstance().CreateDataBaseObj(strDataBaseName,strAcc,strPswd,(ADODB_DataBaseType)dbType);
	if (!bIni)
	{
		return FALSE;
	}
	
	//启动TCP服务器
	g_desc.fnCreateAcceptedObject = NVDSCreateAcceptedObject;
	g_desc.fnDestroyAcceptedObject = NVDSDestroyAcceptedObject;

	if(!server.Init(&g_desc)){
		AfxMessageBox(L"启动TCP服务器失败");
		return FALSE;
	}

	CObjectFactory::getInstance().GetOrderDispatcher()->PreLoadOrders();
	CObjectFactory::getInstance().GetConfirmDispatcher()->preLoadConfirm();
	CObjectFactory::getInstance().setObjectInit(true);
	DebugMessageA("CObjectFactory::getInstance().GetOrderDispatcher() complete!");

	test_all_struct_size();

	afkGetTradeAffairManager()->Run();
	return server.Run();
}

UINT __cdecl exit_thread( LPVOID pParam ){

	CDlgExitProgress* pdlg = (CDlgExitProgress*) pParam;
	while (!pdlg->m_isReady){
		Sleep(100);
	}

	HWND hWnd = pdlg->GetSafeHwnd();
	::PostMessage(hWnd,EXIT_MSG_CLOSING_DISPATCHER,0,0);
	
	COrderDispatcher* orderDispatcher = CObjectFactory::getInstance().GetOrderDispatcher();
	orderDispatcher->EnableDispatcher(FALSE);
	CObjectFactory::getInstance().GetConfirmDispatcher()->disable();
	Sleep(100);

	CTradeMacDispatcher* MacDispatcher = CObjectFactory::getInstance().GetMacDispatcher();
	MacDispatcher->exit();

	int iWaitMostSeconds = 30;//最多等待60秒，否则强行关闭;
	while (iWaitMostSeconds--)
	{
		int remain_count = MacDispatcher->getCurrentProcessOrderCount();
		DebugMessageA("剩余%d个未完成的，等待客户端结束。",remain_count);
		if (remain_count == 0){
			break;
		}
		::PostMessage(hWnd,EXIT_MSG_WAIT_PROCESS_ORDER,remain_count,iWaitMostSeconds);
		Sleep(1000);
	}

	int iWaitMostSeconds_2 = 60;
	while (iWaitMostSeconds_2 -- )
	{
		int remiancount = MacDispatcher->GetCurrentProcessConfirmCount();
		DebugMessageA("剩余%d个审核订单，等待客户端结束。",remiancount);
		if (remiancount == 0)
		{
			break;
		}
		::PostMessage(hWnd,EXIT_MS_WAIT_PROCESS_CONFIRM,remiancount,iWaitMostSeconds_2);
		Sleep(1000);
	}
	
	//"强行"关闭，通过断开连接。
	if (MacDispatcher->getCurrentProcessOrderCount() + MacDispatcher->GetCurrentProcessConfirmCount() != 0)
	{
		int waitseconds = 3;
		::PostMessage(hWnd,EXIT_MSG_FORCE_EXIT,MacDispatcher->getCurrentProcessOrderCount()+MacDispatcher->GetCurrentProcessConfirmCount(),waitseconds);
		MacDispatcher->force_exit();
		Sleep(waitseconds*1000);
	}

	::PostMessage(hWnd,EXIT_MSG_CLOSING_USERS_MGR,0,0);
	afkGetTradeAffairManager()->Close();

	::PostMessage(hWnd,EXIT_MSG_CLOSING_TCP_SERVER,0,0);
	DebugMessageA("shut down tcp server!");
	server.ShutDown();

	::PostMessage(hWnd,EXIT_MSG_CLOSING_ORDER_DISPATCHER,0,0);
	DebugMessageA("now order count in order dispatcher = %d",CObjectFactory::getInstance().GetOrderDispatcher()->getNowTradeListCount());
	CObjectFactory::getInstance().GetOrderDispatcher()->exit();

	::PostMessage(hWnd,EXIT_MSG_CLOSING_CONFIRM_DISPATCHER,0,0);
	DebugMessageA("当前剩余%d个审核订单！",CObjectFactory::getInstance().GetConfirmDispatcher()->getCurConfirmCount());
	CObjectFactory::getInstance().GetConfirmDispatcher()->exit();

	::PostMessage(hWnd,EXIT_MSG_CLOSING_DATABASE,0,0);
	afkGetDbObject()->flush();
	afkGetDbObject()->Close();
	
	CObjectFactory::getInstance().clear();
	::PostMessage(hWnd,EXIT_COMPLETE,0,0);

	extern int i_order_del_counter;

	DebugMessageA("total new %d,total del %d",i_order_new_counter,i_order_del_counter);
	
	return 0;
}


void ShutDownServer(){
	CDlgExitProgress exitdlg;
	AfxBeginThread(exit_thread,(LPVOID)&exitdlg);
	exitdlg.DoModal();
}
