#include "StdAfx.h"
#include "TaskWatch.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "../common/DebugMessage.h"
#include <algorithm>
#include <list>
using std::list;

static list<string> gs_TopWindowNameList;
static BOOL CALLBACK EnumTopWindowProcCbk(HWND hWnd,LPARAM lParam);
TaskWatch::TaskWatch(void)
{
	m_bOnWatch = false;
	m_hThdWorker = 0;
}

TaskWatch::~TaskWatch(void)
{
	if(m_hThdWorker){
		TerminateThread(m_hThdWorker,1);
		m_hThdWorker = 0;
	}
}

int TaskWatch::LoadTaskFromXML()
{
	int iLoad = 0;

	char modulePath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, modulePath, MAX_PATH);
	string strModulePath(modulePath);
	strModulePath = strModulePath.substr(0,strModulePath.rfind('\\'));
	strcpy(modulePath,strModulePath.c_str());
	strcat_s(modulePath,sizeof(modulePath),"\\sentinel.xml");
	
	if (!PathFileExistsA(modulePath))
	{
		AfxMessageBox(L"文件不存在！");
		return 0;
	}

	TiXmlDocument* XmlDoc = new TiXmlDocument(modulePath);
	if (!XmlDoc){
		return 0;
	}


	if(!XmlDoc->LoadFile()){
		delete XmlDoc;
		return 0;
	}
	
	TiXmlElement* rootElement = XmlDoc->RootElement();
	if (!rootElement){
		delete XmlDoc;
		return 0;
	}
	
	TiXmlElement* XmlTaskDesc = rootElement->FirstChildElement("process");
	while (XmlTaskDesc)
	{
		TaskDescribe desc;
		TiXmlElement* XmlTaskSettings = XmlTaskDesc->FirstChildElement();
		while (XmlTaskSettings)
		{
			DebugMessageA("%s--%s\n",XmlTaskSettings->Value(),XmlTaskSettings->GetText());
			string strElementValue = XmlTaskSettings->Value();
			string strElementText = XmlTaskSettings->GetText();
			if (strElementValue == "id"){
				desc.id = atoi(XmlTaskSettings->GetText());
			}
			else if(strElementValue == "task_name"){
				desc.strTaskName = XmlTaskSettings->GetText();
			}
			else if(strElementValue == "ExeFullPath"){
				desc.strFullPath = XmlTaskSettings->GetText();
			}
			else if (strElementValue == "DelayTime")
			{
				desc.iExecuteDelay = atoi(XmlTaskSettings->GetText());
			}
			else if (strElementValue == "WindowName"){
				string strTemp = XmlTaskSettings->GetText();
				//UTF8ToGB(strTemp);
				desc.strWndName = strTemp;
			}

			m_TaskDescribeMap[desc.id] = desc;

			XmlTaskSettings = XmlTaskSettings->NextSiblingElement();
		}
		XmlTaskDesc = XmlTaskDesc->NextSiblingElement("process");
	}

	for (TaskDescribeMapConstIter it = m_TaskDescribeMap.begin(); it!=m_TaskDescribeMap.end();++it)
	{
		DebugMessageA("%d-%s-%s-%d-%s\n",it->second.id,it->second.strTaskName.c_str(),it->second.strFullPath.c_str(),it->second.iExecuteDelay,it->second.strWndName.c_str());
	}
	delete XmlDoc;
	
	iLoad = m_TaskDescribeMap.size();
	if (iLoad && (!m_hThdWorker))
		m_hThdWorker = AfxBeginThread(thread_worker,this)->m_hThread;

	return iLoad;
}

void TaskWatch::SetTaskFullPath( int id,const char* fullPath )
{
	//get module path
	char modulePath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, modulePath, MAX_PATH);
	string strModulePath(modulePath);
	strModulePath = strModulePath.substr(0,strModulePath.rfind('\\'));
	strcpy(modulePath,strModulePath.c_str());
	strcat_s(modulePath,sizeof(modulePath),"\\sentinel.xml");

	TiXmlDocument* XmlDoc = new TiXmlDocument(modulePath);
	if (!XmlDoc){
		return;
	}


	if(!XmlDoc->LoadFile()){
		delete XmlDoc;
		return;
	}

	TiXmlElement* rootElement = XmlDoc->RootElement();
	if (!rootElement){
		delete XmlDoc;
		return;
	}

	BOOL bFind = FALSE;
	TiXmlElement* XmlTaskDesc = rootElement->FirstChildElement();

	char szid[20] = {0};
	sprintf_s(szid,sizeof(szid),"%d",id);

	while (XmlTaskDesc)
	{
		TaskDescribe desc;
		TiXmlElement* XmlTaskSettings = XmlTaskDesc->FirstChildElement();
		//find child where id = szId
		while (XmlTaskSettings)
		{
			string strValue = XmlTaskSettings->Value();
			string strText = XmlTaskSettings->GetText();
			if (strValue == string("id") && strText == string(szid))
			{
				bFind = TRUE;
			}

			if (bFind && strValue == string("ExeFullPath"))
			{
				TiXmlNode* pText = XmlTaskSettings->FirstChild();
				pText->SetValue(fullPath);
				XmlDoc->SaveFile();
				break;
			}
			XmlTaskSettings = XmlTaskSettings->NextSiblingElement();
		}

		if (bFind)
		{
			//Set full path where id = szId.
			break;
		}
		XmlTaskDesc = XmlTaskDesc->NextSiblingElement();
	}
	delete XmlDoc;
}

void TaskWatch::SetWindowName( int id,const char* windowName )
{
	char modulePath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, modulePath, MAX_PATH);
	string strModulePath(modulePath);
	strModulePath = strModulePath.substr(0,strModulePath.rfind('\\'));
	strcpy(modulePath,strModulePath.c_str());
	strcat_s(modulePath,sizeof(modulePath),"\\sentinel.xml");

	TiXmlDocument* XmlDoc = new TiXmlDocument(modulePath);
	if (!XmlDoc){
		return;
	}


	if(!XmlDoc->LoadFile()){
		delete XmlDoc;
		return;
	}

	TiXmlElement* rootElement = XmlDoc->RootElement();
	if (!rootElement){
		delete XmlDoc;
		return;
	}

	BOOL bFind = FALSE;
	TiXmlElement* XmlTaskDesc = rootElement->FirstChildElement();

	char szid[20] = {0};
	sprintf_s(szid,sizeof(szid),"%d",id);

	while (XmlTaskDesc)
	{
		TaskDescribe desc;
		TiXmlElement* XmlTaskSettings = XmlTaskDesc->FirstChildElement();
		//find child where id = szId
		while (XmlTaskSettings)
		{
			string strValue = XmlTaskSettings->Value();
			string strText = XmlTaskSettings->GetText();
			if (strValue == string("id") && strText == string(szid))
			{
				bFind = TRUE;
			}

			if (bFind && strValue == string("WindowName"))
			{
				TiXmlNode* pText = XmlTaskSettings->FirstChild();
				pText->SetValue(windowName);
				XmlDoc->SaveFile();
				break;
			}
			XmlTaskSettings = XmlTaskSettings->NextSiblingElement();
		}

		if (bFind)
		{
			//Set full path where id = szId.
			break;
		}
		XmlTaskDesc = XmlTaskDesc->NextSiblingElement();
	}
	delete XmlDoc;
}

UINT TaskWatch::thread_worker( LPVOID lpParam )
{
	TaskWatch* pTaskWatch = (TaskWatch*)lpParam;
	while(TRUE){
		if (pTaskWatch->m_bOnWatch)
		{
			pTaskWatch->CheckExecuteTasks();
			Sleep(2000);
		}
		Sleep(1000);
	}
}

void TaskWatch::TestFun()
{
	//CheckExecuteTasks();
}

void TaskWatch::CheckExecuteTasks()
{
	gs_TopWindowNameList.clear();
	::EnumWindows(EnumTopWindowProcCbk,0);//枚举窗口
	for (TaskDescribeMapIter it = m_TaskDescribeMap.begin();it != m_TaskDescribeMap.end(); ++it)
	{
		if (it->second.strWndName == "N/A" || it->second.strFullPath == "N/A"){
			DebugMessageA("skip N/A");
			continue;
		}else{
			if (find(gs_TopWindowNameList.begin(),gs_TopWindowNameList.end(),it->second.strWndName) == gs_TopWindowNameList.end()){
				//没有找到窗口
				DebugMessageA("Not find window %s",it->second.strWndName.c_str());
				string strFullPath = it->second.strFullPath;
				int SleepTime = it->second.iExecuteDelay;
				if (!PathFileExistsA(strFullPath.c_str())){
					DebugMessageA("Not find exe file: %s",strFullPath.c_str());
				}else{
					string path = strFullPath.substr(0,strFullPath.rfind('\\'));
					TCHAR szOldPath[200] = {0};
					GetCurrentDirectory(sizeof(szOldPath)/sizeof(TCHAR),szOldPath);
					DebugMessageA("Chdir to %s",path.c_str());
					if(!SetCurrentDirectoryA(path.c_str())){
						DebugMessageA("Chdir failed!");
						SetCurrentDirectory(szOldPath);
					}else{
						Sleep(SleepTime*1000);
						DebugMessageA("WinExec to start task %s...",strFullPath.c_str());
						WinExec(strFullPath.c_str(),SW_SHOW);
						Sleep(SleepTime*1000);
						SetCurrentDirectory(szOldPath);
					}
				}
			}
			else{
				DebugMessageA("find window %s",it->second.strWndName.c_str());
			}
		}

	}
}

BOOL CALLBACK EnumTopWindowProcCbk(HWND hWnd,LPARAM lParam)//枚举窗口的回调函数
{
	wchar_t wszWindowText[256];
	char aszWindowText[256];
	GetWindowText(hWnd,wszWindowText,255);//获得窗口标题

	if (!wcscmp(wszWindowText,L""))
	{
		return true;
	}

	WideCharToMultiByte(CP_ACP,0,wszWindowText,sizeof(wszWindowText)/sizeof(TCHAR),aszWindowText,sizeof(aszWindowText),0,0);
	gs_TopWindowNameList.push_back(string(aszWindowText));

	return true;
}