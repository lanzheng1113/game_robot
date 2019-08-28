#pragma once

#include "MyNineXUsr.h"
#include <list>
#include <vector>
#include "util/DateTime.h"

class basic_utils_for_UsrMgr
{
public:
	static int getRemainHours(const std::string& dateTimeEnd);
	static std::string sub3days(const std::string& dateTimeEnd);
	static std::string sub1days( const std::string& dateTimeEnd );
	static std::string add5days(const std::string& dateTimeEnd);
	static DateTime fromString(const std::string& dateTimeConvert);
protected:
private:
};

#ifdef _SRVIC
class DBAccessor;
#endif

class CLock_CS;
class DBAccessor;
class MyUserMgr
{
public:
	MyUserMgr(void);
	~MyUserMgr(void);
public:
	BOOL load();
	std::vector<CMyNineXUsr*> getCopy();
	std::vector<CMyNineXUsr*> addNewUsr(int count,int days,const std::string& name);
	CMyNineXUsr* GetUsrInfo(const std::string& kardId);
	BOOL saveUsrListByName( const std::string& name ); //在服务器程序中已经被废弃。
	BOOL saveDb();
	int  unbindNoCondition(const std::string& kardId);
	int  unbindSub1Days(const std::string& kardId,const std::string& proxyName);
	void  RegistUser(const std::string& kardId,const std::string& hardid);
	void bindUser(const std::string& kardId,const std::string& hardid);
	void deleteOutDateKards();
	int addfivedaysall();
	void deleteAcc( const CString& m_strToDelAcc );
	BOOL CheckProxyUser( const std::string& proxyAcc,const std::string& proxyPswd );
private:
	std::list<std::string> getFilesInDir(const std::string& mulu);
	std::list<CMyNineXUsr*> congwenjianduqu(const std::string& zhanghaowenjianming );
	CMyNineXUsr* lineToCMyNineXUsrObj(const std::string& line,const std::string& name);
	void addNineXUsrs( const std::list<CMyNineXUsr*>& zhanghaoxinxis );
	std::vector<CMyNineXUsr*> _usrs;

	HANDLE hEventExit;
	HANDLE hThreadSaveDb;
	CLock_CS* m_locker_cs;
#ifdef _SRVIC
	DBAccessor* m_dbm;
#endif
};
