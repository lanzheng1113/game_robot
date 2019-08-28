#pragma once

class CADOConnection;
class CADODataset;
/*
Index �Զ����
User_ID �ı�
UserDeadLine ����/ʱ��
AccType ����
UseAble ��/��
AccSuccessCount ����
AccUseCount ����
UserSystemInfo �ı�
UserCreateTime ����/ʱ��
UserRegistTime ����/ʱ��
ReActiveTime ����/ʱ��
ProxyName �ı�
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
	void DBI_registKard( const std::string& kardId,const std::string& hardid); //ֻ�ڰ󶨵�ʱ���ܵ���һ��
	void DBI_bindUser( const std::string& kardId, const std::string& hardid );
	void DBI_DelAcc( const std::string& m_strToDelAcc );

	BOOL	DBI_CheckProxyUser(const std::string& proxyAcc,const std::string& proxyPswd);
private:
	CADOConnection *m_connection;
};
