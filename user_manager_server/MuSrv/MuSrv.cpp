
// MuSrv.cpp : 定义应用程序的类行为。
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


// CMuSrvApp 构造

CMuSrvApp::CMuSrvApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMuSrvApp 对象

CMuSrvApp theApp;
static bool callback(const wchar_t *dump_path, const wchar_t *id,  
					 void *context, EXCEPTION_POINTERS *exinfo,  
					 MDRawAssertionInfo *assertion,  
					 bool succeeded) {  
						 if (succeeded) {  
							 //TCHAR szMsg[200] = {0};
							 //swprintf(szMsg,200,_T("程序发生崩溃，DUMP文件为：%s"),id);
							 //AfxMessageBox(szMsg);
							 printf("dump guid is %ws\n", id);  
						 } else {  
							 AfxMessageBox("程序发生崩溃，dump 失败");
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
	StringCbPrintfA(szTimestamp,sizeof(szTimestamp),"服务器日志_%d年%d月%d日%d时%d分%d秒.txt",timeinfo->tm_year + 1900,timeinfo->tm_mon + 1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

	string strPath = Path::getApplicationDirPath();
	strPath += "server_logs\\";
	if (!File::exists(strPath))
	{
		File::createPath(strPath);
	}

	strPath += string(szTimestamp);

	return strPath;
}
// CMuSrvApp 初始化
extern map<string,string> g_ServerParameterMap;
BOOL CMuSrvApp::InitInstance()
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
		AfxMessageBox("读取配置失败！");
		ExitProcess(1);
	}
	g_mum.load();
	
	//载入公告信息
	string strPath = Path::getApplicationDirPath();
	strPath += "Notice\\";
	if (!File::exists(strPath))
		File::createPath(strPath);
	strPath += "Notice.txt";
	if (!File::exists(strPath)){
		FileWriter fw(strPath);
		fw.write("欢迎使用。");
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

int CMuSrvApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	CTLogger::getInstance().close();
	return CWinAppEx::ExitInstance();
}
