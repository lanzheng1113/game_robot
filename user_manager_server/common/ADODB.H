/*************************************************************************
*     数据库操作类:
*                  CADOConnection(数据库连接类)
*                  CADODataset(数据集操作类)
*
*
**************************************************************************/
#ifndef ADODB_H
#define ADODB_H

#pragma once
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")


struct CADOFieldInfo//字段信息
{
	TCHAR m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};
//整型量与字符串的转换
CString IntToStr(int nVal);

CString LongToStr(long lVal);

int StrToInt(CString str);


class CADOConnection : public CObject
{
public:
    //1.构造和系构函数
	CADOConnection();
	virtual ~CADOConnection();

	DECLARE_DYNCREATE(CADOConnection );
public:
	//2.数据库连接:open ConDB
	bool ConDB(CString DbName,CString Password=_T(""),int DBType=0);
	bool Open(LPCTSTR lpstrConnection = _T(""));
	bool Execute(LPCTSTR lpstrExec);
    //3.
	_ConnectionPtr GetActiveConnection() {return m_pConnection;};
	void SetConnectionString(LPCTSTR lpstrConnection)
		{m_strConnection = lpstrConnection;};
	CString GetConnectionString()
		{return m_strConnection;};
	//4.事务处理
	long BeginTransaction() 
		{return m_pConnection->BeginTrans();};
	long CommitTransaction() 
		{return m_pConnection->CommitTrans();};
	long RollbackTransaction() 
		{return m_pConnection->RollbackTrans();};
	//5.状态标识
	bool IsOpen();
	void Close();

protected:
    _ConnectionPtr m_pConnection;
 	CString m_strConnection;
};

class CADODataset:public CObject
{
public:
	//1打开参数 编辑参数 位置参数 检索参数
	_RecordsetPtr m_pRecordset;
	enum cadoOpenEnum
	{
		openUnknown = 0,
		openQuery = 1,
		openTable = 2,
		openStoredProc = 3
	};

	enum cadoEditEnum
	{
		dbEditNone = 0,
		dbEditNew = 1,
		dbEdit = 2
	};
	
	enum cadoPositionEnum
	{
	
		positionUnknown = -1,
		positionBOF = -2,
		positionEOF = -3
	};
	
	enum cadoSearchEnum
	{	
		searchForward = 1,
		searchBackward = -1
	};

public:
	//2.构造于系构函数
	CADODataset();
	CADODataset(CADOConnection* pAdoDatabase);
	virtual ~CADODataset();
	DECLARE_DYNCREATE(CADODataset);

private:
   //3.私有 
	//_RecordsetPtr m_pRecordset;
  	//CADOConnection* m_pAdocon;
	CString m_strLastError;
	int m_nSearchDirection;
	CString m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	void dump_com_error(_com_error &e);
	


	_CommandPtr m_pCmd;
	_ConnectionPtr m_pConnection;
	CString m_strQuery;
   //4.用于列表操作
    void ClearList(CListCtrl &ListCtrl1);
	void FillColumn(CListCtrl &ListCtrl1);
	void FillContent(CListCtrl &ListCtrl1);
public:
	void FillList(CListCtrl &ListCtrl1);

   //5.设置字段值,获取字段值
	bool SetFieldValue(int nIndex, CString strValue);
	bool SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	bool SetFieldValue(int nIndex, int nValue);
	bool SetFieldValue(LPCTSTR lpFieldName, int nValue);
	bool SetFieldValue(int nIndex, long lValue);
	bool SetFieldValue(LPCTSTR lpFieldName, long lValue);
	bool SetFieldValue(int nIndex, double dblValue);
	bool SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	bool SetFieldValue(int nIndex, COleDateTime time);
	bool SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);

	bool GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
	bool GetFieldValue(int nIndex, double& dbValue);
	bool GetFieldValue(LPCTSTR lpFieldName, bool& dbValue);
	bool GetFieldValue(int nIndex, bool& dbValue);
	bool GetFieldValue(LPCTSTR lpFieldName, long& lValue);
	bool GetFieldValue(int nIndex, long& lValue);
	bool GetFieldValue(LPCTSTR lpFieldName, int& nValue);
	bool GetFieldValue(int nIndex, int& nValue);
	bool GetFieldValue(LPCTSTR lpFieldName, CString& strValue);
	bool GetFieldValue(int nIndex, CString& strValue);
	bool GetFieldValue(LPCTSTR lpFieldName, COleDateTime* time);
	bool GetFieldValue(int nIndex, COleDateTime* time);
	bool IsFieldNull(LPCTSTR lpFieldName);
	bool IsFieldNull(int nIndex);
	bool IsFieldEmpty(LPCTSTR lpFieldName);
	bool IsFieldEmpty(int nIndex);	
   //6.编辑功能
	void CancelUpdate();
	bool Update();
	void Edit();
	bool AddNew();
    //7.关键字查找
	bool Find(LPCTSTR lpFind, int nSearchDirection = CADODataset::searchForward);
	bool FindFirst(LPCTSTR lpFind);
	bool FindNext();
    CString GetQuery()	{return m_strQuery;};
	void SetQuery(LPCSTR strQuery)	{m_strQuery = strQuery;};
	//bool RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	bool IsOpen();
	void Close();
	bool Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = CADODataset::openUnknown);
	bool Open(LPCTSTR lpstrExec = _T(""), int nOption = CADODataset::openUnknown);
	_RecordsetPtr OpenSql(LPCTSTR TblName,LPCTSTR FldList=_T("*"),LPCTSTR Constr=_T(""),LPCTSTR OrderBy=_T(""));
    bool ExecSql(LPCTSTR SqlStr);
	_RecordsetPtr    ExecStoreProc(LPCSTR SqlProc);

	bool IsEof()
		{return m_pRecordset->adoEOF == VARIANT_TRUE;};
	
	bool IsBof()
		{return m_pRecordset->BOF  == VARIANT_TRUE;};
	void MoveFirst() 
		{m_pRecordset->MoveFirst();};
	void MoveNext() 
		{m_pRecordset->MoveNext();};
	void MovePrevious() 
		{m_pRecordset->MovePrevious();};
	void MoveLast() 
		{m_pRecordset->MoveLast();};

	long GetAbsolutePage()
		{return m_pRecordset->GetAbsolutePage();};
	long GetAbsolutePosition()
		{return m_pRecordset->GetAbsolutePosition();};
		bool GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	void SetAbsolutePage(int nPage)
		{m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);};
	void SetAbsolutePosition(int nPosition)
		{m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);};

	long GetPageCount()
		{return m_pRecordset->GetPageCount();};
	long GetPageSize()
		{return m_pRecordset->GetPageSize();};
	void SetPageSize(int nSize)
		{m_pRecordset->PutPageSize(nSize);};

	bool GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	bool GetChunk(LPCTSTR lpFieldName, CString& strValue);
	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	long GetFieldCount(){ return this->m_pRecordset->GetFields()->GetCount(); }
	void GetBookmark()
		{m_varBookmark = m_pRecordset->Bookmark;};
	bool SetBookmark();
	bool Delete();
	void SetConnection(CADOConnection *pCon)
	  {
           this->m_pConnection=pCon->GetActiveConnection(); 	
	  };
	void SetActiveConnection(_ConnectionPtr pConnection)
	{
	   this->m_pConnection=pConnection;
	};
};



#endif
