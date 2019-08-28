#include "StdAfx.h"
#include "DBAccessor.h"
#include "ADODB.H"
#include "../common/MyNineXUsr.h"
#include "strsafe.h"
#include "util/StringEx.h"
#include "util/DateTime.h"
#include "util/DateTimeSpan.h"
#include "DebugMessage.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
#endif

#define _DATABASE_NAME _T("UserInfo_DB.mdb")
#define _DATABASE_PASSWORD _T("4260201asdf")
#define _USER_TABLE_NAME  _T("Users")
DBAccessor::DBAccessor(void)
:m_connection(NULL)
{
	m_connection = new CADOConnection();
}

DBAccessor::~DBAccessor(void)
{
	m_connection->Close();
	SAFE_DELETE(m_connection);
}

BOOL DBAccessor::connectDB()
{
	if (!(m_connection))
	{
		return FALSE;
	}

	if(!m_connection->ConDB(_DATABASE_NAME,_DATABASE_PASSWORD))
		return FALSE;

	return TRUE;
}

std::list<CMyNineXUsr*> DBAccessor::loadAllUserToMemory()
{

	std::list<CMyNineXUsr*> retList;

	CADODataset dataset;
	dataset.SetConnection(m_connection);

	try{
		dataset.OpenSql(_USER_TABLE_NAME);
	}
	catch(_com_error &e){
		AfxMessageBox(e.ErrorMessage());
	}

	if (!dataset.IsOpen())
	{
		AfxMessageBox("�򿪼�¼��ʧ�ܣ�");
		retList;
	}


	dataset.MoveFirst();

	//User_ID,User_Pswd,UserDeadLine,CorrespondingGameType,CorrespondingGameID,AccType,UseAble,DLLHash,AccSuccessCount,AccUseCount,UserSystemInfo,\
	//UserRegistTime,ReActiveTime,RC5EnDeCryptKey
	DWORD dwGet = 0;
	while ((!dataset.IsEof()))
	{
		CMyNineXUsr* ui = new CMyNineXUsr;

		long index = 0; //Index �Զ����
		CString userid; //User_ID �ı�
		long acctype = 0; //AccType ����
		bool usable = false; //UseAble ��/��
		long accSuccCount = 0; //AccSuccessCount ����
		long accUseCount = 0; //AccUseCount ����
		CString strSysInfo; //UserSystemInfo �ı�
		CString strProxyName;  //ProxyName �ı�
		COleDateTime OleCreateTime;//UserCreateTime ����/ʱ��
		COleDateTime OleDtRegTime; //UserRegistTime ����/ʱ��
		COleDateTime OleDtReActiveTime; //ReActiveTime ����/ʱ��
		COleDateTime OleDtUserDeadLine; //UserDeadLine ����/ʱ��
		COleDateTime OleDtLastUnbind; //LastUnbindTime ������ʱ��
		try{
			dataset.GetFieldValue(_T("Index"),index);
			dataset.GetFieldValue(_T("User_ID"),userid);
			dataset.GetFieldValue(_T("AccType"),acctype);
			dataset.GetFieldValue(_T("UseAble"),usable);
			dataset.GetFieldValue(_T("AccSuccessCount"),accSuccCount);
			dataset.GetFieldValue(_T("AccUseCount"),accUseCount);
			dataset.GetFieldValue(_T("UserSystemInfo"),strSysInfo);
			dataset.GetFieldValue(_T("UserCreateTime"),&OleCreateTime);
			dataset.GetFieldValue(_T("UserRegistTime"),&OleDtRegTime);
			dataset.GetFieldValue(_T("ReActiveTime"),&OleDtReActiveTime);
			dataset.GetFieldValue(_T("UserDeadLine"),&OleDtUserDeadLine);
			dataset.GetFieldValue(_T("ProxyName"),strProxyName);
			dataset.GetFieldValue(_T("LastUnbindTime"),&OleDtLastUnbind);
		}catch(_com_error& e){
			AfxMessageBox(e.ErrorMessage());
			break;
		}

		CString strOleCreateTime = OleCreateTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strOleDtRegTime = OleDtRegTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strOleDtReActiveTime = OleDtReActiveTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		CString strOleDtLastUnbindTime = OleDtLastUnbind.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strOleDtUserDeadLine;
		if (OleDtUserDeadLine.GetYear() == 1900){
			strOleDtUserDeadLine = "δ��";
		}else{
			strOleDtUserDeadLine = OleDtUserDeadLine.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		
		ui->setAccessSuccessCount(accSuccCount);
		ui->setAccUseCount(accUseCount);
		ui->setAuthId((LPCTSTR)userid);
		ui->setCardDaysType(String::fromLong(acctype));
		ui->setDate_of_end((LPCTSTR)strOleDtUserDeadLine); //
		ui->setHarddisk((LPCTSTR)strSysInfo);
		ui->setIndex(index);
		ui->setIsDirty(false);
		ui->setName((LPCTSTR)strProxyName);
		ui->setReActiveTime((LPCTSTR)strOleDtReActiveTime);
		ui->setUserCreateTime((LPCTSTR)strOleCreateTime);
		ui->setUserRegistTime((LPCTSTR)strOleDtRegTime);
		ui->setDate_of_last_unbind((LPCTSTR)strOleDtLastUnbindTime);
		retList.push_back(ui);
		dataset.MoveNext();
		dwGet ++;
	}

	dataset.Close();
	return retList;
}

DWORD DBAccessor::DBI_addNewUsr( std::vector<CMyNineXUsr*> addList )
{
	/*
	bp->setName(name);
	bp->setAuthId(szKardId);
	bp->setCardDaysType(szType);
	bp->setDate_of_end(endTime);//д�����ݿ��ʱ����Ҫ�ж�һ���Ƿ���unbinded_tag����Ϊ���ݿ��������ʱ���ʽ����Ӧ�ģ������ݿ��������ʱ��Ҳ��Ҫ�ж�һ�¡�
	bp->setHarddisk(harddiskId);
	bp->setAccessSuccessCount(0);
	bp->setAccUseCount(0);
	bp->setIndex(-1); //д�����ݿ��ʱ����Ҫ�ж����ֵ�Ƿ�Ϊ-1����-1��ʾΪ��ֵ��
	bp->setIsDirty(true);
	bp->setReActiveTime("1900-1-1 00:00:00");
	bp->setUserCreateTime(DateTime().toString());
	bp->setUserRegistTime("1900-1-1 00:00:00");*/
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

	CADODataset dataset;
	dataset.SetConnection(m_connection);
	DWORD dwCreateCount = 0;
	for (std::vector<CMyNineXUsr*>::const_iterator it = addList.begin(); it != addList.end(); ++it)
	{
		char szSQL[1024] = {0};
		std::string dateofend = (*it)->getDate_of_end();
		if (dateofend == "δ��"){
			dateofend = INIT_DB_TIMESTAMP;
		}
		StringCbPrintf(szSQL,sizeof(szSQL),
			_T("Insert into %s(User_ID,AccType,UseAble,UserRegistTime,ReActiveTime,UserDeadLine,ProxyName,UserSystemInfo) \
			   values(\'%s\',\'%s\',true,#%s#,#%s#,#%s#,\'%s\',\'%s\');"),
			   _USER_TABLE_NAME,
			   (*it)->getAuthId().c_str(),
			   (*it)->getCardDaysType().c_str(),
			   (*it)->getUserRegistTime().c_str(),
			   INIT_DB_TIMESTAMP,
			   dateofend.c_str(),
			   (*it)->getName().c_str(),
			   (*it)->getHarddisk().c_str());
		
		bool bSuccessExe = false;
		try{
			bSuccessExe = dataset.ExecSql(szSQL);
		}

		catch(_com_error& e){
			AfxMessageBox(e.ErrorMessage());
			bSuccessExe = false;
		}

		if (bSuccessExe){
			dwCreateCount ++;
		}
		else{
			continue;
		}
	}
	return dwCreateCount;
}

void DBAccessor::DBI_unbindNoCondition( const std::string& kardId )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	StringCbPrintf(szSQL,sizeof(szSQL),"UPDATE %s SET UserSystemInfo = \'δ��\' WHERE User_ID = \'%s\'",_USER_TABLE_NAME,kardId.c_str());
	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(szSQL);
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}
	return ;
}


void DBAccessor::DBI_unbindSub1Day( const std::string& kardId,const std::string& dateAfterSub )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	StringCbPrintf(szSQL,sizeof(szSQL),"UPDATE %s SET UserSystemInfo = \'δ��\',UserDeadLine = #%s#,LastUnbindTime=#%s# WHERE User_ID = \'%s\'",
		_USER_TABLE_NAME,
		dateAfterSub.c_str(),
		DateTime().toString().c_str(),
		kardId.c_str());

	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(szSQL);
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}
	return ;
}

void DBAccessor::DBI_AddFiveDays()
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	StringCbPrintf(szSQL,sizeof(szSQL),
		"update %s set UserDeadLine = DateAdd(\"d\",5,Now())",
		_USER_TABLE_NAME);

	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(szSQL);
	}catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}

	return ;

}

void DBAccessor::DBI_registKard( const std::string& kardId,const std::string& hardid )
{
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
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	StringCbPrintf(szSQL,sizeof(szSQL),
		"update %s set UserRegistTime = Now(),UserDeadLine = DateAdd(\"d\",AccType,Now()),UserSystemInfo=\'%s\' WHERE User_ID = \'%s\'",
		_USER_TABLE_NAME,
		hardid.c_str(),
		kardId.c_str());

	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(szSQL);
	}catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}

	return ;
}

void DBAccessor::DBI_bindUser( const std::string& kardId, const std::string& hardid )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	StringCbPrintf(szSQL,sizeof(szSQL),
		"update %s set UserSystemInfo=\'%s\' WHERE User_ID = \'%s\'",
		_USER_TABLE_NAME,
		hardid.c_str(),
		kardId.c_str());

	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(szSQL);
	}catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}

	return ;
}

void DBAccessor::DBI_DelAcc( const std::string& m_strToDelAcc )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	StringCbPrintf(szSQL,sizeof(szSQL),
		"delete from %s where User_ID = \"%s\"",
		_USER_TABLE_NAME,
		m_strToDelAcc.c_str());
	//AfxMessageBox(szSQL);
	bool bSuccessExe = false;
	try{
		bSuccessExe = dataset.ExecSql(szSQL);
	}catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = false;
	}

	if (dataset.IsOpen()){
		dataset.Close();
	}

	return ;
}

BOOL DBAccessor::DBI_CheckProxyUser( const std::string& proxyAcc,const std::string& proxyPswd )
{
	CADODataset dataset;
	dataset.SetConnection(m_connection);
	char szSQL[1024] = {0};
	//Indexs ProxyAcc�ı� ProxyPswd�ı� Privilege���� Balance���� ProxyName�ı� ProxyDetail�ı�
	StringCbPrintf(szSQL,sizeof(szSQL),"where ProxyAcc=\"%s\" and ProxyPswd=\"%s\"",proxyAcc.c_str(),proxyPswd.c_str());
	BOOL bSuccessExe = FALSE;
	try{
		dataset.OpenSql("ProxyInfoTable","ProxyAcc,ProxyPswd",szSQL);
		if (!dataset.IsOpen())
		{
			bSuccessExe = FALSE;
		}
		else
		{
			if (dataset.m_pRecordset->BOF)
			{
				bSuccessExe = FALSE;
			}
			else
			{
				bSuccessExe = TRUE;
			}
		}
	}catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
		bSuccessExe = FALSE;
	}
	return bSuccessExe;
}

