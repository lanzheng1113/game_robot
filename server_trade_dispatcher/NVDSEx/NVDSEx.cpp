
// NVDSEx.cpp : 定义应用程序的类行为。
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


// CNVDSExApp 构造

CNVDSExApp::CNVDSExApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNVDSExApp 对象

CNVDSExApp theApp;

static bool callback(const wchar_t *dump_path, const wchar_t *id,  
					 void *context, EXCEPTION_POINTERS *exinfo,  
					 MDRawAssertionInfo *assertion,  
					 bool succeeded) {  
						 if (succeeded) {  
							 //TCHAR szMsg[200] = {0};
							 //swprintf(szMsg,200,_T("程序发生崩溃，DUMP文件为：%s"),id);
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
							 AfxMessageBox(L"程序发生崩溃，dump 失败");
							 //printf("dump failed\n");  
						 }  
						 fflush(stdout);  

						 return succeeded;  
}

static void CrashFunction() {  
	int *i = reinterpret_cast<int*>(0x45);  
	*i = 5;  // crash!  
} 

// CNVDSExApp 初始化
string generalLogFileName(){
	string appDir = Path::getApplicationDirPath();
	appDir += "server_logs\\";
	if (!File::exists(appDir)){
		File::createPath(appDir);
	}
	
	if (!File::exists(appDir)){
		MessageBoxW(NULL,L"创建日志文件夹失败！日志将创建在当前运行目录下",NULL,0);
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
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

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
		AfxMessageBox(L"读取配置NVDSEx.xml失败！");
		ExitProcess(1);
	}

	if (!CSettings::getInstance().initGamersConfigure())
	{
		AfxMessageBox(L"配置游戏类型games.xml失败了,程序将退出");
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
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


IDatabaseObject * afkGetDbObject()
{
	return CObjectFactory::getInstance().getDatabaseInterface();
}
