// DB.cpp : 实现文件
//

#include "stdafx.h"
#include "afx.h"
#include "ADODB.h"


// CDB
IMPLEMENT_DYNCREATE(CADOConnection, CObject)
CADOConnection::CADOConnection()
{    
	::CoInitialize(NULL);
   HRESULT hr;
   try
   {
     hr=this->m_pConnection.CreateInstance("ADODB.Connection");
     if(SUCCEEDED(hr))
     {
		return;
	 }
   }
   catch(_com_error e)///捕捉异常
   {
        CString errormessage;
        errormessage.Format(_T("连接数据库失败!\r\n错误信息:%s"),e.ErrorMessage());
        AfxMessageBox(errormessage);///显示错误信息
    } 
}

CADOConnection::~CADOConnection()
{
	//if (this->m_pConnection->State  
	
	if (this->IsOpen()) 
	{
	  this->m_pConnection->Close();
	  //this->m_pConnection->Release();  
	  this->m_pConnection=NULL; 
	}
	
	::CoUninitialize();
}

bool CADOConnection::ConDB(CString DbName,CString Password,int DBType)
{
  //HRESULT hr;
  CString constr;
  //获取连接字符串 
  DbName.TrimRight();
  ASSERT(DbName!=""); 
  if (DBType==0) 
  {
     constr=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")+DbName+_T(";Persist Security Info=False;");
     Password.TrimRight();
	 if(Password!="")
	 {
	  constr=constr+_T("Jet OLEDB:Database Password=")+Password+_T(";");
	 }
  }
  else
  {
  
  }
  return this->Open(constr);

}


bool CADOConnection::Open(LPCTSTR lpstrConnection)
{
	HRESULT hr = S_OK;

	if(IsOpen())
		Close();

	if(lstrcmp(lpstrConnection, _T("")) != 0)
		m_strConnection = lpstrConnection;

	ASSERT(!m_strConnection.IsEmpty());

	try
	{
		hr = m_pConnection->Open(_bstr_t(m_strConnection), _T(""), _T(""), NULL);
		return hr == S_OK;
	}
	catch(_com_error &e)
	{
		CString errormessage;
        errormessage.Format(_T("连接数据库失败!\r\n错误信息:%s"),e.ErrorMessage());
        AfxMessageBox(errormessage);///显示错误信息
	}
	return false;
}

bool CADOConnection::Execute(LPCTSTR lpstrExec)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(lstrcmp(lpstrExec, _T("")) != 0);

	try
	{
		m_pConnection->Execute(_bstr_t(lpstrExec), NULL, adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		CString errormessage;
        errormessage.Format(_T("连接数据库失败!\r\n错误信息:%s"),e.ErrorMessage());
        AfxMessageBox(errormessage);///显示错误信息
	}
	return true;	
}


bool CADOConnection::IsOpen()
{
	if(m_pConnection )
		return m_pConnection->GetState() != adStateClosed;
	return false;
}

void CADOConnection::Close()
{
	if(IsOpen())
		m_pConnection->Close();
}

// CDB 成员函数

//CADODataset
IMPLEMENT_DYNCREATE(CADODataset, CObject)
CADODataset::CADODataset()
{
	//m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pRecordset = NULL;
	m_pCmd = NULL;
	m_strQuery = _T("");
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	m_pCmd.CreateInstance(__uuidof(Command));
	m_nEditStatus = CADODataset::dbEditNone;
	m_nSearchDirection = CADODataset::searchForward;
}

CADODataset::~CADODataset()
{  
	Close();
	m_pRecordset.Release();
	m_pCmd.Release();
	m_pRecordset = NULL;
	m_pCmd = NULL;
	m_strQuery = _T("");
	m_nEditStatus = dbEditNone;
}

_RecordsetPtr CADODataset::OpenSql(LPCTSTR TblName,LPCTSTR FldList,LPCTSTR Constr,LPCTSTR OrderBy)
{  
	//ASSERT(this->m_pAdocon->m_pConnection!=NULL);
   ASSERT(this->m_pConnection!=NULL);
   TCHAR sqlstr[256];
   lstrcpy(sqlstr,_T("select "));
   lstrcat(sqlstr,FldList); lstrcat(sqlstr,_T(" from "));
   lstrcat(sqlstr,TblName);lstrcat(sqlstr,_T("  "));
   lstrcat(sqlstr,Constr);lstrcat(sqlstr,_T("  "));lstrcat(sqlstr,OrderBy);
   if (IsOpen())
   {
	   this->m_pRecordset->Close();
     }
   m_pRecordset->Open((_variant_t) sqlstr,
	                    this->m_pConnection.GetInterfacePtr(),
					   adOpenStatic,
					   adLockOptimistic,
					   adCmdText);
  return m_pRecordset;
}

bool CADODataset::ExecSql(LPCTSTR SqlStr)
{
   ASSERT(this->m_pConnection!=NULL);
   ASSERT(this->m_pCmd!=NULL); 
   m_pCmd->ActiveConnection = this->m_pConnection;
   m_pCmd->CommandText = _bstr_t(SqlStr);
   m_pCmd->CommandType = adCmdText;		
   m_pCmd->Execute(NULL, NULL, adCmdText);
   return true;
}
_RecordsetPtr    CADODataset::ExecStoreProc(LPCSTR SqlProc)
{
   ASSERT(this->m_pConnection!=NULL);
   ASSERT(this->m_pCmd!=NULL); 
   m_pCmd->ActiveConnection = this->m_pConnection;
   m_pCmd->CommandText = _bstr_t(SqlProc);
   m_pCmd->CommandType = adCmdStoredProc;		
   this->m_pRecordset=m_pCmd->Execute(NULL, NULL, adCmdText);
   return this->m_pRecordset;
}
void CADODataset::FillList(CListCtrl &ListCtrl1)
{
  	if(this->IsOpen()) 
	{
      ListCtrl1.SetRedraw(false);
	  this->ClearList(ListCtrl1);
	  this->FillColumn(ListCtrl1);
	  this->FillContent(ListCtrl1);
	  ListCtrl1.SetRedraw(true);
	  ListCtrl1.Invalidate();
	}
	else
	{
		AfxMessageBox(_T("记录集没有打开！"));
	}
}

void CADODataset::ClearList(CListCtrl &ListCtrl1)
{
	 ListCtrl1.DeleteAllItems ();
	
	int nColumnCount = ListCtrl1.GetHeaderCtrl()->GetItemCount();
	// Delete all of the columns.
	for (int i=0;i < nColumnCount;i++)
	{
		ListCtrl1.DeleteColumn (0);
	}
  
}
void CADODataset::FillColumn(CListCtrl &ListCtrl1)
{
	int fieldCount=m_pRecordset->Fields ->Count;
	int fieldLength = 0;
	FieldPtr m_fieldCtl;

	for(int i=0;i<fieldCount;i++)
	{
		m_fieldCtl = m_pRecordset->Fields ->GetItem(long(i));
		fieldLength = m_fieldCtl->DefinedSize*20;
 		if (fieldLength >250)   //when field is very long then trim it
 		{
 			fieldLength = 250;
 		}

 		if (fieldLength < 50)
		{
			fieldLength = 50;
		}
// 		else
// 		{
// 		   fieldLength = 80;
// 		}
		/*
		if (fieldLength < m_fieldCtl->Name.length()*12)
		{
			fieldLength = m_fieldCtl->Name.length()*12;
		}
        */
		ListCtrl1.InsertColumn(i,m_fieldCtl->Name,LVCFMT_LEFT,fieldLength);
	}
   
}

void CADODataset::FillContent(CListCtrl &ListCtrl1)
{
	int nItem = 0;
	FieldPtr m_fieldCtl;
	int fieldCount=m_pRecordset->Fields ->Count;
	_variant_t varValue;
	_bstr_t bstrValue;

	while(!m_pRecordset->adoEOF)
	{
		m_fieldCtl = m_pRecordset->Fields ->GetItem(long(0));
		varValue = m_fieldCtl->Value;
		if (varValue.vt == VT_NULL)
		{
			bstrValue = "";
		}
		else if (varValue.vt==VT_BOOL)
		{
		    if(varValue.boolVal==-1)
			{bstrValue="是";}
			else
			{bstrValue="否";}
		}
		else
		{
			bstrValue=varValue;
		}
		nItem=ListCtrl1.InsertItem(0xffff,bstrValue); //fisrt value

		for(int i=1;i<fieldCount;i++)  //next all value
		{
			m_fieldCtl = m_pRecordset->Fields ->GetItem(long(i));
			varValue = m_fieldCtl->Value;
			if (varValue.vt == VT_NULL)
			{
				bstrValue = "";
			}
			else if (varValue.vt==VT_BOOL)
			{
				if(varValue.boolVal==-1)//-1 true
				{bstrValue="是";}
				else
				{bstrValue="否";}
			}
			else
			{
				bstrValue=varValue;
			}

			ListCtrl1.SetItem(nItem,i,1,bstrValue,NULL,0,0,0);
		}

		m_pRecordset->MoveNext ();
	}

}
bool CADODataset::Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec, int nOption)
{	
	Close();
	
	if(lstrcmp(lpstrExec, _T("")) != 0)
		m_strQuery = lpstrExec;

	ASSERT(!m_strQuery.IsEmpty());
	
	m_strQuery.TrimLeft();
	BOOL bIsSelect =(m_strQuery.Mid(0, lstrlen(_T("Select "))).CompareNoCase(_T("select ")) == 0);

	try
	{
		m_pRecordset->CursorLocation = adUseClient;
		if(bIsSelect || nOption == openQuery)
			m_pRecordset->Open((LPCTSTR)m_strQuery, _variant_t((IDispatch*)mpdb, true), 
							adOpenStatic, adLockOptimistic, adCmdUnknown);
		else if(nOption == openTable)
			m_pRecordset->Open((LPCTSTR)m_strQuery, _variant_t((IDispatch*)mpdb, true), 
							adOpenDynamic, adLockOptimistic, adCmdTable);
		else if(nOption == openStoredProc)
		{
			m_pCmd->ActiveConnection = mpdb;
			m_pCmd->CommandText = _bstr_t(m_strQuery);
			m_pCmd->CommandType = adCmdStoredProc;
			
			m_pRecordset = m_pCmd->Execute(NULL, NULL, adCmdText);
		}
		else
		{
			TRACE( "Unknown parameter. %d", nOption);
			return false;
		}
	}
	catch(_com_error &e)
	{
        	
		dump_com_error(e);
		return false;
	}

	return m_pRecordset != NULL;
}

bool CADODataset::Open(LPCTSTR lpstrExec, int nOption)
{
	ASSERT(this->m_pConnection != NULL);
	return Open(this->m_pConnection, lpstrExec, nOption);
}

bool CADODataset::GetFieldValue(LPCTSTR lpFieldName, double& dbValue)
{	
	double val = (double)NULL;
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.dblVal;
	dbValue = val;
	return true;
}


bool CADODataset::GetFieldValue(int nIndex, double& dbValue)
{	
	double val = (double)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.dblVal;
	dbValue = val;
	return true;
}


bool CADODataset::GetFieldValue(LPCTSTR lpFieldName, long& lValue)
{
	long val = (long)NULL;
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.lVal;
	lValue = val;
	return true;
}

bool CADODataset::GetFieldValue(int nIndex, long& lValue)
{
	long val = (long)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.lVal;
	lValue = val;
	return true;
}


bool CADODataset::GetFieldValue(LPCTSTR lpFieldName, int& nValue)
{
	int val = NULL;
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt)
	{
	case VT_I2:
		val = vtFld.iVal;
		break;
	case VT_I4:
		val = vtFld.iVal;
		break;
	case VT_BOOL:
		val = vtFld.boolVal;
		break;
	case VT_NULL:
		val=0;
		break;
	case VT_EMPTY:
		val=0;
		break;
	case VT_BSTR:
		try
		{
		   val=atoi((char*)vtFld.bstrVal);
		   break;
		}
        catch( TCHAR* str )
		{
		  AfxMessageBox(str);
		  return 0;
		};
	default:
		nValue = 0;
		return false;
	}	
	nValue = val;
	return true;
}

bool CADODataset::GetFieldValue(int nIndex, int& nValue)
{
	int val = (int)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt)
	{
	case VT_I2:
		val = vtFld.iVal;
		break;
	case VT_I4:
		val = vtFld.iVal;
		break;
	case VT_NULL:
        val = 0;
		break;
	case VT_BOOL:
		val = vtFld.boolVal;
		break;
	case VT_EMPTY:
		val = 0;
		break;
	default:
		return false;
	}	
	nValue = val;
	return true;
}

bool CADODataset::GetFieldValue(LPCTSTR lpFieldName, CString& strValue)
{
	CString str = _T("");
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt) 
	{
	case VT_BSTR:
		str = vtFld.bstrVal;
		break;
	case VT_DATE:
		{
			COleDateTime dt(vtFld);

			str = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		break;
	case VT_EMPTY:
          
	case VT_NULL:
		str="";
		break;
	case VT_I1:
		str.Format(_T("%d"),vtFld.intVal);
		break;
	case VT_I2:
		str.Format(_T("%d"),vtFld.iVal);
		break;
	case VT_I4:
		str.Format(_T("%d"),vtFld.intVal);
		break;
	case VT_BOOL:
		if(vtFld.boolVal==-1)
		{str=_T("TRUE");}
		else
		{str=_T("FALSE");}
		break;
	case VT_DECIMAL:
		str.Format(_T("%f"),vtFld.decVal);
		break;
	default:
		strValue.Empty();
		return false;
	}
	strValue = str;
	return true;
}

bool CADODataset::GetFieldValue(int nIndex, CString& strValue)
{
	CString str = _T("");
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt) 
	{
	case VT_BSTR:
		str = vtFld.bstrVal;
		break;
	case VT_DATE:
		{
			COleDateTime dt(vtFld);
			str = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		break;
	case VT_EMPTY:
		str="";
		break;
	case VT_NULL:
		str="";
		break;
	case VT_I1:
		str.Format(_T("%d"),vtFld.intVal);
		break;
	case VT_I2:
		str.Format(_T("%d"),vtFld.iVal);
		break;
	case VT_I4:
		str.Format(_T("%d"),vtFld.intVal);
		break;
	case VT_BOOL:
		if(vtFld.boolVal==-1)
		{str=_T("TRUE");}
		else
		{str=_T("FALSE");}
		break;
	case VT_DECIMAL:
		str.Format(_T("%f"),vtFld.decVal);
		break;
	default:
		strValue.Empty();
		break;
	}
	strValue = str;
	return true;
}

bool CADODataset::GetFieldValue(LPCTSTR lpFieldName, COleDateTime* time)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt) 
	{
	case VT_DATE:
		{
			time->m_dt = vtFld.dblVal;
			time->m_status =COleDateTime::DateTimeStatus::valid;//有效 
		}
		break;
	case VT_EMPTY:		  
	case VT_NULL:
		break;
	default:
		return false;
	}
	return true;
}

bool CADODataset::GetFieldValue(int nIndex, COleDateTime* time)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt) 
	{
	case VT_DATE:
		{
		    time->m_dt = vtFld.dblVal;
			time->m_status =COleDateTime::DateTimeStatus::valid;//有效 		
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		return false;
	}
	return true;
}
bool CADODataset::GetFieldValue(LPCTSTR lpFieldName, bool& boolValue)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt)
	{
	case VT_BOOL:
		if(vtFld.boolVal==-1)
		  boolValue=true;
		else
          boolValue=false;
		break;
	default:
		return false;
	}
	return true;
}
bool CADODataset::GetFieldValue(int nIndex, bool& boolValue)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt)
	{
	case VT_BOOL:
    	if(vtFld.boolVal==-1)
		  boolValue=true;
		else
          boolValue=false;
		break;
	default:
		return false;
	}
	return true;

}
bool CADODataset::IsFieldNull(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	return vtFld.vt == VT_NULL;
}

bool CADODataset::IsFieldNull(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	return vtFld.vt == VT_NULL;
}

bool CADODataset::IsFieldEmpty(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
}

bool CADODataset::IsFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
		
	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
}

DWORD CADODataset::GetRecordCount()
{
	DWORD nRows = 0;
	
	nRows = m_pRecordset->GetRecordCount();

	if(nRows == -1)
	{
		nRows = 0;
		if(m_pRecordset->adoEOF != VARIANT_TRUE)
			m_pRecordset->MoveFirst();
		
		while(m_pRecordset->adoEOF != VARIANT_TRUE)
		{
			nRows++;
			m_pRecordset->MoveNext();
		}
		if(nRows > 0)
			m_pRecordset->MoveFirst();
	}
	
	return nRows;
}

bool CADODataset::IsOpen()
{
	if( m_pRecordset)
		return m_pRecordset->GetState() != adStateClosed;
	return false;
}

void CADODataset::Close()
{
	if(IsOpen())
		m_pRecordset->Close();
		
}


/*
bool CADODataset::RecordBinding(CADORecordBinding &pAdoRecordBinding)
{
	IADORecordBinding *picRs = NULL;
	HRESULT hr;

	//Open the binding interface.
	if(FAILED(hr = m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&picRs )))
	{
		_com_issue_error(hr);
		return false;
	}
	
	//Bind the recordset to class
	if(FAILED( hr = picRs->BindToRecordset(&pAdoRecordBinding)))
	{
		_com_issue_error(hr);
		return false;
	}
	return true;
}
*/
void CADODataset::dump_com_error(_com_error &e)
{
	CString ErrorStr;
	
	
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( _T("CADORecordset Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
		e.Error(), e.ErrorMessage(), (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription );
	m_strLastError = _T("Query = " + GetQuery() + '\n' + ErrorStr);

	#ifdef _DEBUG
		AfxMessageBox( ErrorStr, MB_OK | MB_ICONERROR );
	#endif	
}

bool CADODataset::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)
{
	_variant_t vtFld;
	
	lstrcpy(fldInfo->m_strName, (LPCTSTR)m_pRecordset->Fields->GetItem(lpFieldName)->GetName());
	//fldInfo->m_lSize = m_pRecordset->Fields->GetItem(lpFieldName)->GetActualSize();
	fldInfo->m_lDefinedSize = m_pRecordset->Fields->GetItem(lpFieldName)->GetDefinedSize();
	fldInfo->m_nType = m_pRecordset->Fields->GetItem(lpFieldName)->GetType();
	//fldInfo->m_lAttributes = m_pRecordset->Fields->GetItem(lpFieldName)->GetAttributes();
	return true;
}

bool CADODataset::GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)
{
	_variant_t vtFld;
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	lstrcpy(fldInfo->m_strName, (LPCTSTR)m_pRecordset->Fields->GetItem(vtIndex)->GetName());
	//fldInfo->m_lSize = m_pRecordset->Fields->GetItem(vtIndex)->GetActualSize();
	fldInfo->m_lDefinedSize = m_pRecordset->Fields->GetItem(vtIndex)->GetDefinedSize();
	fldInfo->m_nType = m_pRecordset->Fields->GetItem(vtIndex)->GetType();
	//fldInfo->m_lAttributes = m_pRecordset->Fields->GetItem(vtIndex)->GetAttributes();
	return true;
}


bool CADODataset::GetChunk(LPCTSTR lpFieldName, CString& strValue)
{
	CString str = _T("");
	long lngSize, lngOffSet = 0;
	_variant_t varChunk;    
	int ChunkSize = 100;

	lngSize = m_pRecordset->Fields->GetItem(lpFieldName)->ActualSize;
	
	str.Empty();
	while(lngOffSet < lngSize)
	{
		varChunk = m_pRecordset->Fields->GetItem(lpFieldName)->GetChunk(ChunkSize);
		str += varChunk.bstrVal;
		lngOffSet += ChunkSize;
	}

	lngOffSet = 0;
	strValue = str;
	return TRUE;
}

CString CADODataset::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
{
	_bstr_t varOutput;
	_bstr_t varNull("");
	_bstr_t varCols("\t");
	_bstr_t varRows("\r");

	if(lstrlen(lpCols) != 0)
		varCols = _bstr_t(lpCols);

	if(lstrlen(lpRows) != 0)
		varRows = _bstr_t(lpRows);
	
	if(numRows == 0)
		numRows =(long)GetRecordCount();			
			
	varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

	return (LPCTSTR)varOutput;
}

CString IntToStr(int nVal)
{
	CString strRet;
	char buff[10];
	
	itoa(nVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString LongToStr(long lVal)
{
	CString strRet;
	char buff[20];
	
	ltoa(lVal, buff, 10);
	strRet = buff;
	return strRet;
}

int StrToInt(CString str)
{
	int temp;
// 	char *chAstr = new char [str.GetLength()];
// 	WideCharToMultiByte(CP_ACP,0,
// 						(LPCTSTR)str,(str.GetLength()+1)*2,
// 						chAstr,str.GetLength()+1,
// 						0,0
// 						);
// 	//WideCharToMultiByte(CP_ACP,0,IPsw,64,IPsa,32,0,0);
// 	temp=atoi(chAstr);
// 	delete chAstr;
	temp = atoi((LPCTSTR)str);
	return temp;
}
void CADODataset::Edit()
{
	m_nEditStatus = dbEdit;
}

bool CADODataset::AddNew()
{
	m_nEditStatus = dbEditNone;
	if(m_pRecordset->AddNew() != S_OK)
		return false;

	m_nEditStatus = dbEditNew;
	return true;
}

bool CADODataset::Update()
{
	bool bret = true;

	if(m_nEditStatus != dbEditNone)
	{
		if(m_pRecordset->Update() != S_OK)
			bret = false;
	}

	m_nEditStatus = dbEditNone;
	return bret;
}

void CADODataset::CancelUpdate()
{
	m_pRecordset->CancelUpdate();
	m_nEditStatus = dbEditNone;
}

bool CADODataset::SetFieldValue(int nIndex, CString strValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BSTR;
	vtFld.bstrVal = _bstr_t(strValue);

	_variant_t vtIndex;	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = _bstr_t(vtFld);//_bstr_t(strValue);
	return true;

}

bool CADODataset::SetFieldValue(LPCTSTR lpFieldName, CString strValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BSTR;
	vtFld.bstrVal = _bstr_t(strValue);
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = _bstr_t(vtFld);//_bstr_t(strValue);
	return true;
	
}

bool CADODataset::SetFieldValue(int nIndex, int nValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;
	
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(LPCTSTR lpFieldName, int nValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(int nIndex, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;
	
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(LPCTSTR lpFieldName, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(int nIndex, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(LPCTSTR lpFieldName, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;
		
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(int nIndex, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;
	
	_variant_t vtIndex;
	
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	
	m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetFieldValue(LPCTSTR lpFieldName, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;
	
	m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	return true;
	
}

bool CADODataset::SetBookmark()
{
	if(m_varBookmark.vt != VT_EMPTY)
	{
		m_pRecordset->Bookmark = m_varBookmark;
		return true;
	}
	return false;
}

bool CADODataset::Delete()
{
	if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
		return false;

	if(m_pRecordset->Update() != S_OK)
		return false;

	return true;
}

bool CADODataset::Find(LPCTSTR lpFind, int nSearchDirection)
{

	m_strFind = lpFind;
	m_nSearchDirection = nSearchDirection;

	ASSERT(!m_strFind.IsEmpty());

	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchForward, "");
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else if(m_nSearchDirection == searchBackward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchBackward, "");
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else
	{
		TRACE("Unknown parameter. %d", nSearchDirection);
		m_nSearchDirection = searchForward;
	}
	return false;
}

bool CADODataset::FindFirst(LPCTSTR lpFind)
{
	m_pRecordset->MoveFirst();
	return Find(lpFind);
}

bool CADODataset::FindNext()
{
	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchForward, m_varBookFind);
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	else
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchBackward, m_varBookFind);
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return true;
		}
	}
	return false;
}

