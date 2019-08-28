
// NVDSEx.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "NVDSEx.h"
#include "NVDSExDlg.h"
#include "testServer.h"
//google-breakpad
#include "client/windows/handler/exception_handler.h"
//#include "DBM.h"
#include <util/Logger.h>
#include <util/DateTime.h>
#include <util/Path.h>
#include <util/File.h>
#include <string>
#include <cstdlib>
#include "ObjectFactory.h"
#include "Settings.h"
#include "OrderDispatcher.h"
#include "../common/DebugMessage.h"
using std::string;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CNVDSExApp

BEGIN_MESSAGE_MAP(CNVDSExApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNVDSExApp ����

CNVDSExApp::CNVDSExApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNVDSExApp ����

CNVDSExApp theApp;

static bool callback(const wchar_t *dump_path, const wchar_t *id,  
					 void *context, EXCEPTION_POINTERS *exinfo,  
					 MDRawAssertionInfo *assertion,  
					 bool succeeded) {  
						 if (succeeded) {  
							 //TCHAR szMsg[200] = {0};
							 //swprintf(szMsg,200,_T("������������DUMP�ļ�Ϊ��%s"),id);
							 //AfxMessageBox(szMsg);
							 printf("dump guid is %ws\n", id);
							 string appDir = Path::getApplicationDirPath();
							 appDir += "CrashReport.exe";
							 wchar_t* pExePath = new wchar_t [appDir.length()*2];
							 size_t conver = 0;
							 mbstowcs_s(&conver,pExePath,appDir.length()*2,appDir.c_str(),appDir.length());
							 ShellExecute(NULL,NULL,pExePath,id,NULL,SW_SHOW);
							 delete [] pExePath;
						 } else {  
							 AfxMessageBox(L"������������dump ʧ��");
							 //printf("dump failed\n");  
						 }  
						 fflush(stdout);  

						 return succeeded;  
}

static void CrashFunction() {  
	int *i = reinterpret_cast<int*>(0x45);  
	*i = 5;  // crash!  
} 

// CNVDSExApp ��ʼ��
string generalLogFileName(){
	string appDir = Path::getApplicationDirPath();
	appDir += "server_logs\\";
	if (!File::exists(appDir)){
		File::createPath(appDir);
	}
	
	if (!File::exists(appDir)){
		MessageBoxW(NULL,L"������־�ļ���ʧ�ܣ���־�������ڵ�ǰ����Ŀ¼��",NULL,0);
		appDir = Path::getApplicationDirPath();
	}

	std::time_t curTime = time(NULL);
	struct std::tm * timeinfo = std::localtime(&curTime);
	char szTimestamp[64] = {0};
	sprintf_s(szTimestamp,sizeof(szTimestamp),"%d-%d-%d==%d-%d-%d.txt",timeinfo->tm_year + 1900,timeinfo->tm_mon + 1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	
	string strRet = appDir + string(szTimestamp);
	return strRet;
}

BOOL CNVDSExApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	string strPath = Path::getApplicationDirPath();
	strPath += "crash_dump\\";
	if (!File::exists(strPath))
		File::createPath(strPath);
	
	if (!File::exists(strPath))
		strPath = Path::getApplicationDirPath();
	
	wchar_t* sz = new wchar_t[strPath.length()*2];
	size_t convers = 0;
	mbstowcs_s(&convers,sz,strPath.length()*2,strPath.c_str(),strPath.length());
	std::wstring dump_path(sz);
	delete [] sz;
	
	//google-breakpad
 	google_breakpad::ExceptionHandler eh(  
 		dump_path, NULL, callback, NULL,  
 		google_breakpad::ExceptionHandler::HANDLER_ALL);  
//	//test crash
//	CrashFunction();

	Logger::getInstance()->setLogFileName(generalLogFileName());
	BOOL bRet = InitSrvGlobalParameter();
	if (!bRet)
	{
		AfxMessageBox(L"��ȡ����NVDSEx.xmlʧ�ܣ�");
		ExitProcess(1);
	}

	if (!CSettings::getInstance().initGamersConfigure())
	{
		AfxMessageBox(L"������Ϸ����games.xmlʧ����,�����˳�");
		ExitProcess(1);
	}

	CNVDSExDlg dlg;
	CObjectFactory::getInstance().setMainWndPtr(&dlg);
	CObjectFactory::getInstance().CreateDispatcher();

	if(!RunServer())
		return FALSE;
	
	LOG_INFO("Trade Server start @ %s",DateTime().toString().c_str());

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


IDatabaseObject * afkGetDbObject()
{
	return CObjectFactory::getInstance().getDatabaseInterface();
}
