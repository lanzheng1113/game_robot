#pragma once

#include <string>
using std::string;

#include <map>
using std::map;

class TaskDescribe
{
public:
	int id;	//1,2,3,etc.
	string strTaskName;//task name.some as "my server"
	string strFullPath;//".exe" file full path.as "d:/work/my_server.exe" or "d:\work\my_server.exe"
	int iExecuteDelay;//seconds
	string strWndName;//window title
protected:
private:
};

typedef map<int,TaskDescribe> TASK_DESCRIBE_MAP;
typedef map<int,TaskDescribe>::const_iterator TaskDescribeMapConstIter;
typedef map<int,TaskDescribe>::iterator TaskDescribeMapIter;

class TiXmlDocument;

class TaskWatch
{
public:
	TaskWatch(void);
	~TaskWatch(void);
public:
	int					LoadTaskFromXML();
	TASK_DESCRIBE_MAP	GetMap() const { return m_TaskDescribeMap; }
	void				SetTaskFullPath(int id,const char* fullPath);
	void				SetWindowName(int id,const char* windowName);
	bool				GetOnWatch() const { return m_bOnWatch; }
	void				SetOnWatch(bool val) { m_bOnWatch = val; }
	void				TestFun();
protected:
	static UINT			thread_worker(LPVOID lpParam);
private:
	void				CheckExecuteTasks();
private:
	TASK_DESCRIBE_MAP m_TaskDescribeMap;
	bool m_bOnWatch;
	HANDLE m_hThdWorker;

};
