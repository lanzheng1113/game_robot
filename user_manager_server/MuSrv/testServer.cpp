#include "stdafx.h"
#include "testServer.h"
#include "session.h"
#include "IOCPServer.h"
#include "../common/DebugMessage.h"
#include "../common/NetBase.h"
#include "DoValidateAffair.h"
#include "dvdtAffairMgr.h"
#include "../common/NVDSNetHeader.h"
#include "../common/tinyxml/include/tinyxml.h"
#include "../common/tinyxml/include/tinystr.h"
#include <iostream>
#include <string>
#include <map>
#include "util/Logger.h"
#include "util/Path.h"
#include "../common/adodb.h"
#include "DlgExitProgress.h"

using std::string;
using std::cout;
using std::endl;
using std::map;

extern DoValidateAffairMgr g_DvlidateAffairMgr;

map<string,string> g_ServerParameterMap;

NVDSNetworkObject::NVDSNetworkObject()
{
	m_dwRecvCount = 0;
	m_pDovalidateAffair = NULL;
}

NVDSNetworkObject::~NVDSNetworkObject(){
	//DebugMessageA("[NVDSNetworkObject] ~NVDSNetworkObject");
}

VOID NVDSNetworkObject::OnDisconnect()
{
	//DebugMessageA("[NVDSNetworkObject][OnDisconnect]");
	if (m_pDovalidateAffair){
		m_pDovalidateAffair->SetNetworkCallback(NULL);
		m_pDovalidateAffair->OnSessionClosed();
	}
}

VOID NVDSNetworkObject::OnRecv( BYTE *pMsg, WORD wSize )
{
	if (wSize == 6 && *(WORD*)(pMsg+4) == heart){
		//����������С=6�Լ��ٷ���������
	}else if(wSize < sizeof(PACKETGENERALHEAD)){
		Disconnect();
		return;
	}

	enum_AffairType AffairType = (enum_AffairType)(*(DWORD*)pMsg);
	switch(AffairType)
	{
	case AffairType_Dovalidate://Here no need to consider think about the multi-thread.because the iocpserver once only post one RECV-request.
		{
			if (wSize == 6 && *(WORD*)(pMsg+4) == heart)
			{
				if (!m_pDovalidateAffair){
					//δ�������������յ�������������İ���
					LOG_ERROR("δ�������������յ�������������İ���");
					Disconnect();
				}else{
					//��������ͨ��ʱ��
					m_pDovalidateAffair->SetLastCommTime(GetTickCount());
					WORD packet_ack = heart_ack;
					Send((BYTE*)&packet_ack,2);
				}
				break;
			}

			if (!m_pDovalidateAffair)
			{
				m_pDovalidateAffair = new DoValidateAffair();
				m_pDovalidateAffair->SetNetworkCallback(this);
				m_pDovalidateAffair->SetClientIp(GetIP());
			}
			m_pDovalidateAffair->OnDataRecv(pMsg,wSize);
// 			if (m_dwRecvCount == 20)
// 			{
// 				DebugMessageA("[NVDSNetworkObject] close session;\n");
// 				Disconnect();
// 				break;
// 			}else{
// 				m_dwRecvCount ++;
// 				BYTE sendbuf[20] = "aabb";
// 				Send(sendbuf,sizeof(sendbuf));
// 			}
		}
		break;
	case AffairType_Assist:
		{
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

	std::string s = Path::getApplicationDirPath();
	s+= "NVDSEx.xml";

	//����һ��XML���ĵ�����
	TiXmlDocument *myDocument = new TiXmlDocument(s.c_str());
	if (!myDocument)
	{
		AfxMessageBox("����XML���ö���ʧ�ܣ�");
		return FALSE;
	}
	DebugMessageA("open Nvds main ini.%s\n",s.c_str());
	//AfxMessageBox(L"�������ö���ɹ���");
	if (!myDocument->LoadFile())
	{
		AfxMessageBox("�������ļ�NVDSEx.xmlʧ�ܣ�");
		delete myDocument;
		return FALSE;
	}

	//AfxMessageBox(L"�������ļ�NVDSEx.xml�ɹ���");
	//��ø�Ԫ��

	TiXmlElement *RootElement = myDocument->RootElement();
	if (RootElement)
	{
		//�����Ԫ�����ƣ������NVDSEx
		//DebugMessageA("%s\n",RootElement->Value());
		//��õ�һ���ӽڵ㡣
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
		AfxMessageBox("���ô���Max accept session = 0");
		ExitProcess(1);
	}

	if (desc->dwMaxPacketSize == 0)
	{
		AfxMessageBox("���ô���Max packet size = 0");
		ExitProcess(1);
	}

	if (desc->dwRecvBufferSize == 0 || desc->dwSendBufferSize == 0)
	{
		AfxMessageBox("���ô���Recv buffer size or send buffer size = 0");
		ExitProcess(1);
	}

	if (desc->dwNumberOfIoThreads == 0)
	{
		AfxMessageBox("���ô���io thread = 0");
		ExitProcess(1);
	}

	if (desc->usListenPort == 0)
	{
		AfxMessageBox("���ô���listen port = 0");
		ExitProcess(1);
	}

	return;
}

void RunTestServer()
{
	memset(&g_desc,0,sizeof(IOHANDLER_DESC));
	InitServerParameter(&g_desc);

// 	g_desc.dwMaxAcceptSession = 3000;
// 	g_desc.dwMaxPacketSize = 4096;
// 	g_desc.dwNumberOfIoThreads = 4;
// 	g_desc.dwRecvBufferSize = 4096*4;
// 	g_desc.dwSendBufferSize = 4096*4;
// 	g_desc.dwTimeOut = 0xffffffff;
//	g_desc.usListenPort = 6481;
	g_desc.fnCreateAcceptedObject = NVDSCreateAcceptedObject;
	g_desc.fnDestroyAcceptedObject = NVDSDestroyAcceptedObject;

	if(!server.Init(&g_desc)){
		AfxMessageBox("����������ʧ��");
		ExitProcess(1);
		return;
	}

	g_DvlidateAffairMgr.Run();
	server.Run();
}

void ShutDownServer(){
	DlgExitProgress* pDlg = new DlgExitProgress();
	pDlg->Create(IDD_DIALOG1,NULL);
	pDlg->ShowWindow(SW_SHOW);
	//pDlg->m_strProgress = L"�ر����ݿ����Ӻ���־д��ģ�顭��";
	//pDlg->UpdateData(FALSE);
	//g_dbm.WriteEvent(0,0,0,L"NVDSEx �ر�");
	//g_dbm.Close();
	//pDlg->m_Progress_Exit.SetPos(20);
	pDlg->m_strProgress = "�ر��û�����������";
	pDlg->UpdateData(FALSE);
	g_DvlidateAffairMgr.Exit();
	pDlg->m_Progress_Exit.SetPos(40);

	pDlg->m_strProgress = "�ر�TCP������ģ�顭��";
	pDlg->UpdateData(FALSE);
	server.ShutDown();
	pDlg->m_Progress_Exit.SetPos(80);
	pDlg->UpdateData(FALSE);
	delete pDlg;
	Sleep(100);
}