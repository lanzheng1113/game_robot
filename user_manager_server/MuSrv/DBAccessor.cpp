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
		AfxMessageBox("打开记录集失败！");
		retList;
	}


	dataset.MoveFirst();

	//User_ID,User_Pswd,UserDeadLine,CorrespondingGameType,CorrespondingGameID,AccType,UseAble,DLLHash,AccSuccessCount,AccUseCount,UserSystemInfo,\
	//UserRegistTime,ReActiveTime,RC5EnDeCryptKey
	DWORD dwGet = 0;
	while ((!dataset.IsEof()))
	{
		CMyNineXUsr* ui = new CMyNineXUsr;

		long index = 0; //Index 自动编号
		CString userid; //User_ID 文本
		long acctype = 0; //AccType 数字
		bool usable = false; //UseAble 是/否
		long accSuccCount = 0; //AccSuccessCount 数字
		long accUseCount = 0; //AccUseCount 数字
		CString strSysInfo; //UserSystemInfo 文本
		CString strProxyName;  //ProxyName 文本
		COleDateTime OleCreateTime;//UserCreateTime 日期/时间
		COleDateTime OleDtRegTime; //UserRegistTime 日期/时间
		COleDateTime OleDtReActiveTime; //ReActiveTime 日期/时间
		COleDateTime OleDtUserDeadLine; //UserDeadLine 日期/时间
		COleDateTime OleDtLastUnbind; //LastUnbindTime 最后解绑的时间
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
			strOleDtUserDeadLine = "未绑定";
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
	bp->setDate_of_end(endTime);//写入数据库的时候需要判断一下是否是unbinded_tag，因为数据库里面的是时间格式，对应的，从数据库里读出的时候也需要判断一下。
	bp->setHarddisk(harddiskId);
	bp->setAccessSuccessCount(0);
	bp->setAccUseCount(0);
	bp->setIndex(-1); //写入数据库的时候需要判断这个值是否为-1，是-1表示为新值。
	bp->setIsDirty(true);
	bp->setReActiveTime("1900-1-1 00:00:00");
	bp->setUserCreateTime(DateTime().toString());
	bp->setUserRegistTime("1900-1-1 00:00:00");*/
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

	CADODataset dataset;
	dataset.SetConnection(m_connection);
	DWORD dwCreateCount = 0;
	for (std::vector<CMyNineXUsr*>::const_iterator it = addList.begin(); it != addList.end(); ++it)
	{
		char szSQL[1024] = {0};
		std::string dateofend = (*it)->getDate_of_end();
		if (dateofend == "未绑定"){
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
	StringCbPrintf(szSQL,sizeof(szSQL),"UPDATE %s SET UserSystemInfo = \'未绑定\' WHERE User_ID = \'%s\'",_USER_TABLE_NAME,kardId.c_str());
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
	StringCbPrintf(szSQL,sizeof(szSQL),"UPDATE %s SET UserSystemInfo = \'未绑定\',UserDeadLine = #%s#,LastUnbindTime=#%s# WHERE User_ID = \'%s\'",
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
	//Indexs ProxyAcc文本 ProxyPswd文本 Privilege数字 Balance数字 ProxyName文本 ProxyDetail文本
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

