
// MuSrv.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MuSrv.h"
#include "MuSrvDlg.h"
#include "testServer.h"
#include "../common/MyUserMgr.h"
//google-breakpad
#include "client/windows/handler/exception_handler.h"
//#include "DBM.h"
#include <Libutil/util/Logger.h>
#include <libUtil/util/DateTime.h>
#include <libUtil/util/Path.h>
#include <libutil/util/File.h>
#include "../common/Notice.h"
#include "TLogger.h"
#include <string>
#include <map>
using std::map;
using std::string;
#include "strsafe.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MyUserMgr g_mum;

// CMuSrvApp

BEGIN_MESSAGE_MAP(CMuSrvApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMuSrvApp ����

CMuSrvApp::CMuSrvApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMuSrvApp ����

CMuSrvApp theApp;
static bool callback(const wchar_t *dump_path, const wchar_t *id,  
					 void *context, EXCEPTION_POINTERS *exinfo,  
					 MDRawAssertionInfo *assertion,  
					 bool succeeded) {  
						 if (succeeded) {  
							 //TCHAR szMsg[200] = {0};
							 //swprintf(szMsg,200,_T("������������DUMP�ļ�Ϊ��%s"),id);
							 //AfxMessageBox(szMsg);
							 printf("dump guid is %ws\n", id);  
						 } else {  
							 AfxMessageBox("������������dump ʧ��");
							 //printf("dump failed\n");  
						 }  
						 fflush(stdout);  
						 return succeeded;  
}

static void CrashFunction() {  
	int *i = reinterpret_cast<int*>(0x45);  
	*i = 5;  // crash!  
} 

string generalLogFileName(){
	std::time_t curTime = time(NULL);
	struct std::tm * timeinfo = std::localtime(&curTime);
	char szTimestamp[200] = {0};
	StringCbPrintfA(szTimestamp,sizeof(szTimestamp),"��������־_%d��%d��%d��%dʱ%d��%d��.txt",timeinfo->tm_year + 1900,timeinfo->tm_mon + 1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

	string strPath = Path::getApplicationDirPath();
	strPath += "server_logs\\";
	if (!File::exists(strPath))
	{
		File::createPath(strPath);
	}

	strPath += string(szTimestamp);

	return strPath;
}
// CMuSrvApp ��ʼ��
extern map<string,string> g_ServerParameterMap;
BOOL CMuSrvApp::InitInstance()
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
	//google-breakpad
	google_breakpad::ExceptionHandler eh(  
		L".", NULL, callback, NULL,  
		google_breakpad::ExceptionHandler::HANDLER_ALL);  
	//test crash
	//CrashFunction();
	setlocale( LC_ALL, "chs" );
	Logger::getInstance()->setLogFileName(generalLogFileName());
	BOOL bRet = InitSrvGlobalParameter();
	if (!bRet)
	{
		AfxMessageBox("��ȡ����ʧ�ܣ�");
		ExitProcess(1);
	}
	g_mum.load();
	
	//���빫����Ϣ
	string strPath = Path::getApplicationDirPath();
	strPath += "Notice\\";
	if (!File::exists(strPath))
		File::createPath(strPath);
	strPath += "Notice.txt";
	if (!File::exists(strPath)){
		FileWriter fw(strPath);
		fw.write("��ӭʹ�á�");
		fw.close();
	}
	CNotice::getInstance().setNoticeFileName(strPath);
	CNotice::getInstance().read();
	CTLogger::getInstance().init();
	RunTestServer();

	LOG_INFO("NVDS start @ %s",DateTime().toString().c_str());
	CMuSrvDlg dlg;
	dlg.setTitle(CString(g_ServerParameterMap[string("WindowTitle")].c_str()));
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

int CMuSrvApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CTLogger::getInstance().close();
	return CWinAppEx::ExitInstance();
}
