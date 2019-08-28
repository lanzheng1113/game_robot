#include "StdAfx.h"
#include "MySQL.h"
#include "DebugMessage.h"
#include "util/StringEx.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
#endif

#define _DATABASE_NAME _T("UserInfo_DB.mdb")
#define _DATABASE_PASSWORD _T("4260201asdf")
#define _USER_TABLE_NAME  _T("Users")

CMySQL::CMySQL(void)
:m_connection(NULL)
{
	m_connection = new CADOConnection();
}

CMySQL::~CMySQL(void)
{
	if (m_connection)
	{
		m_connection->Close();
		SAFE_DELETE(m_connection);
	}
}

BOOL CMySQL::connectDB()
{
	if (!(m_connection))
	{
		return FALSE;
	}

	if(!m_connection->ConDB(_DATABASE_NAME,_DATABASE_PASSWORD))
		return FALSE;

	return TRUE;
}

BOOL CMySQL::Exec(const CString& strSql,CListCtrl& pListControl)
{
	String s((LPCTSTR)strSql);
	String sUpper = s.toUpperCase();
	if (sUpper.beginsWith("SELECT ")){
		return ExecSelect(strSql,pListControl);
	}else if (sUpper.beginsWith("UPDATE ")){
		return ExecUpdate(strSql,pListControl);
	}else if (sUpper.beginsWith("DELETE ")){
		return ExecDelete(strSql,pListControl);
	}else if (sUpper.beginsWith("INSERT ")){
		return ExecInsert(strSql,pListControl);
	}
	else{
		AfxMessageBox("不支持的命令");
		return FALSE;
	}
	return TRUE;
}
BOOL CMySQL::ExecSelect( const CString& strSql,CListCtrl& pListControl )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.Open(strSql);
		//bSuccessExe = dataset.ExecSql(strSql);
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}
	
	if (bSuccessExe){
		dataset.FillList(pListControl);
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}
	return bSuccessExe;
}

BOOL CMySQL::ExecDelete( const CString& strSql,CListCtrl& pListControl)
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(strSql);
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (bSuccessExe){
		AfxMessageBox("执行成功。");
	}else
		AfxMessageBox("执行失败了。");

	if (dataset.IsOpen()){
		dataset.Close();
	}
	return bSuccessExe;
}

BOOL CMySQL::ExecUpdate( const CString& strSql,CListCtrl& pListControl )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(strSql);
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (bSuccessExe){
		AfxMessageBox("执行成功。");
	}else
		AfxMessageBox("执行失败了。");

	if (dataset.IsOpen()){
		dataset.Close();
	}
	return bSuccessExe;
}

BOOL CMySQL::ExecInsert( const CString& strSql,CListCtrl& pListControl )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(strSql);
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (bSuccessExe){
		AfxMessageBox("执行成功。");
	}else
		AfxMessageBox("执行失败了。");

	if (dataset.IsOpen()){
		dataset.Close();
	}
	return bSuccessExe;
}

BOOL CMySQL::Close()
{
	if (!m_connection)
	{
		return FALSE;
	}
	m_connection->Close();
	SAFE_DELETE(m_connection);
	return TRUE;
}
