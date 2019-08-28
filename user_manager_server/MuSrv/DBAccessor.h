#pragma once

class CADOConnection;
class CADODataset;
/*
Index 自动编号
User_ID 文本
UserDeadLine 日期/时间
AccType 数字
UseAble 是/否
AccSuccessCount 数字
AccUseCount 数字
UserSystemInfo 文本
UserCreateTime 日期/时间
UserRegistTime 日期/时间
ReActiveTime 日期/时间
ProxyName 文本
*/
#include <list>
#include <vector>
class CMyNineXUsr;

class DBAccessor
{
public:
	DBAccessor(void);
	~DBAccessor(void);
public:
	BOOL		connectDB();
	DWORD		getTableAllRecordCount(LPCTSTR tablename);
	DWORD		getAllTableName( TCHAR tablename[][200],DWORD dwTalbeNameLen,DWORD maxcount);
	std::list<CMyNineXUsr*> loadAllUserToMemory();
	DWORD DBI_addNewUsr( std::vector<CMyNineXUsr*> addList );
	void DBI_unbindNoCondition( const std::string& kardId );
	void DBI_AddFiveDays();
	void DBI_unbindSub1Day( const std::string& kardId,const std::string& dateAfterSub );
	void DBI_registKard( const std::string& kardId,const std::string& hardid); //只在绑定的时候能调用一次
	void DBI_bindUser( const std::string& kardId, const std::string& hardid );
	void DBI_DelAcc( const std::string& m_strToDelAcc );

	BOOL	DBI_CheckProxyUser(const std::string& proxyAcc,const std::string& proxyPswd);
private:
	CADOConnection *m_connection;
};
