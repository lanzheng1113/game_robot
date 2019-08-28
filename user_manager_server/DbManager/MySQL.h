#pragma once
#include "../Common/ADODB.H"
#include "util/Singleton.h"
class CMySQL : public Singleton<CMySQL>
{
public:
	CMySQL(void);
	~CMySQL(void);
public:
	BOOL connectDB();
	BOOL Exec(const CString& strSql,CListCtrl& pListControl);
	BOOL Close();
private:
	BOOL ExecSelect(const CString& strSql,CListCtrl& pListControl);
	BOOL ExecDelete(const CString& strSql,CListCtrl& pListControl);
	BOOL ExecUpdate(const CString& strSql,CListCtrl& pListControl);
	BOOL ExecInsert(const CString& strSql,CListCtrl& pListControl);
private:
	CADOConnection *m_connection;
};
