#include "stdafx.h"
#include "dbm.h"
#include "../common/adodb.h"
#include "strsafe.h"
#include "../common/DebugMessage.h"
#include "../common/shareStruct.h"
#include "util/DateTime.h"
#include <libUtil/util/Logger.h>
#include "TradeOrder.h"
#include "Settings.h"
#include "../common/stringtool.h"
#include "RemoteDataSource.h"
#include "tradeClientMachine.h"
#include "util/File.h"
#include "../common/AutoLock.h"
#include "ConfirmOrder.h"
//
	extern int i_order_new_counter;
CTDatabaseObject::CTDatabaseObject()
: m_pConnect(NULL)
{
	m_pConnect = new CADOConnection();
	if (!m_pConnect)
		return;
	m_is_load_confirm_in_process = false;
	m_is_load_order_in_process = false;
}

CTDatabaseObject::CTDatabaseObject( ADODB_DataBaseType dbType,LPCTSTR DBName,LPCTSTR User,LPCTSTR Password )
:m_pConnect(NULL),
m_strDBName(DBName),
m_strUser(User),
m_strPassword(Password)
{
	m_dbType = dbType;
	m_pConnect = new CADOConnection();
	if (!m_pConnect)
		return;
	m_is_load_confirm_in_process = false;
	m_is_load_order_in_process = false;
}

CTDatabaseObject::CTDatabaseObject( ADODB_DataBaseType dbType,LPCTSTR DBName )
:m_pConnect(NULL),
m_strDBName(DBName)
{
	m_pConnect = new CADOConnection();
	if (!m_pConnect)
		return;
	m_dbType = dbType;
	m_strUser = _T("");
	m_strPassword = _T("");
	m_is_load_confirm_in_process = false;
	m_is_load_order_in_process = false;
}

CTDatabaseObject::~CTDatabaseObject()
{
	if (m_pConnect)
	{
		BOOL isOpen = m_pConnect->IsOpen();
		m_pConnect->Close();
		
		try{
			delete m_pConnect;
		}catch(...){
			LOG_ERROR("在释放连接时发生异常，该异常可能是::CoUninitialize()导致的;");
		}
		
		m_pConnect = NULL;
	}
}

BOOL CTDatabaseObject::Init(const CString& strDbName,const CString& strUserName,
			   const CString& strPassword,ADODB_DataBaseType dbType)
{
	if (!(m_pConnect))
	{
		return FALSE;
	}
	
	m_cs_save = new CLock_CS();
	m_cs_insert_fatal = new CLock_CS();
	m_save_buffer_counter = 0;
	m_insert_fatal_buffer_counter = 0;

	m_strDBName = strDbName;
	m_strPassword = strPassword;
	m_strUser = strUserName;
	m_dbType = dbType;

	m_thread_is_live_flag = 0;
	CWinThread* ptd = AfxBeginThread(CTDatabaseObject::threadfun_exec_database,this);
	if (!ptd)
	{
		return FALSE;
	}
	m_hWorkThread = ptd->m_hThread;
	m_nWorkThread = ptd->m_nThreadID;

	m_flushthread_is_live_flag = false;
	CWinThread* ptd2 = AfxBeginThread(CTDatabaseObject::threadfun_flush,this);
	if (!ptd2)
	{
		return FALSE;
	}
	m_nFlushThread = ptd2->m_nThreadID;

	if(!m_pConnect->ConDB(m_strDBName,m_strPassword,m_strUser,m_dbType))
		return FALSE;

	m_last_flush_time = GetTickCount();
// 	m_strEventTableName = strEventTableName;
// //	m_strUserInfoTableName = strUserInfoTableName;
// 	m_strProxyInfoTableName = PROXY_INFO_TABLE_NAME;
// 	//check events table exist? if not,create it.
// 	CheckCreateEventTable();
// 	//check proxy information table exist or not,if not ,create the table.
// 	CheckCreateProxyInfoTable();
// 	//CheckCreateProxyNameField();//test create proxy name field.
// 	//DoValidateUserInfo ui;	//test
// 	//GetUserInfoByUserID(1000,&ui);
	vector<type_game_settings> vec = CSettings::getInstance().getSettingsGameTypes();
	if (!vec.size())
	{
		return FALSE;
	}

	if (_trade_order_tb_settings.size())
	{
		_trade_order_tb_settings.clear();
	}

	for (int i=0; i!=vec.size();i++)
	{
		tbset ts;
		ts.order_table_name = vec[i]._order_table_name;
		ts.record_table_name = vec[i]._record_table_name;
		int iGameType = vec[i]._game_type_id;
		_trade_order_tb_settings[iGameType] = ts;

	}

	if (_confirm_order_tb_settings.size())
	{
		_trade_order_tb_settings.clear();
	}

	for (int i = 0; i!=vec.size();i++)
	{
		tbset ts;
		ts.order_table_name = vec[i]._confirm_table_name;
		ts.record_table_name = vec[i]._record_table_name;
		int iGameType = vec[i]._game_type_id;
		_confirm_order_tb_settings[iGameType] = ts;
	}
//	test1();
// 	CTradeOrder order;
// 	string strid;
// 	GetTradeOrder(order,strid);
	
	
	return TRUE;
}

void CTDatabaseObject::Close()
{
	PostThreadMessage(m_nWorkThread,E_TMSG_EXIT,(WPARAM)this,0);
	PostThreadMessage(m_nFlushThread,E_TMSG_EXIT,(WPARAM)this,0);
	int icounter = 0;
	while (m_thread_is_live_flag)
	{	
		if (icounter%10 == 0)
		{
			DebugMessageA("等待工作线程退出!");
		}
		Sleep(100);
	}

	icounter = 0;
	while (m_flushthread_is_live_flag)
	{
		if (icounter%10 == 0) 
		{
			DebugMessageA("等待缓冲区刷新线程退出!");
		}
		Sleep(100);
	}

	if (m_pConnect)
	{
		try{
			m_pConnect->Close();
			delete m_pConnect;
			m_pConnect = NULL;
		}catch(_com_error& e){
#ifdef _DEBUG
			AfxMessageBox((LPCTSTR)e.Description());
#endif
			LOG_ERROR("%s",(char*)e.Description());
		}
	}
}

void CTDatabaseObject::test1()
{
	// 	list<CTradeOrder*> olist = GetTradeNoneGetOrder(10001,-1);
	// 	for (list<CTradeOrder*>::const_iterator it = olist.begin();it != olist.end(); ++it)
	// 	{
	// 		DebugMessageA("%s",(*it)->dump().c_str());
	// 	}
// 	srand(GetTickCount());
 	int tradeidbegin = 1000001;
 	int uid_begin = 1001;
  	COleDateTime getdt = COleDateTime::GetCurrentTime();
  	string tp_strSeller = "testSellerAccount";
  	string tp_strSellerPswd = "testSellerPswd";
  	string tp_strSellerRole = "testSellerRole";
  	string tp_strBuyer = "testBuyerAcc";
  	string tp_strBuyerPswd = "testBuyerPswd";
  	string tp_strBuyerRole = "testBuyerRole";
  	string tp_strTradeId = "TEST";
 	m_pConnect->BeginTransaction();
 	for (int i = 0; i!=32766;i++)
 	{
 		CTradeOrder order_seller;
 		order_seller._order_id = tp_strTradeId + String::fromLong(tradeidbegin++);
 		order_seller._acc = tp_strBuyer + String::fromLong(uid_begin++);
 		order_seller._game_type = 10001;
 		order_seller.setGetTime(DateTime());
 		order_seller._pswd = tp_strBuyerPswd + String::fromLong(uid_begin);
 		order_seller._role_name = tp_strBuyerRole + String::fromLong(uid_begin);
 		memset(&order_seller._trade_good_detail,0,sizeof(order_seller._trade_good_detail));
 		order_seller._trade_place = "";
 		order_seller._world = "望海崖";
 		order_seller.setTradeMoney(123456);
 		order_seller.setTradeResult(0);
 
 		CTradeOrder order_buyer;
 		order_buyer._acc = tp_strBuyer + String::fromLong(uid_begin++);
 		order_buyer._pswd = tp_strBuyerPswd + String::fromLong(uid_begin);
 		order_buyer._role_name = tp_strBuyerRole + String::fromLong(uid_begin);
 
 		order_buyer._order_id = order_seller._order_id;
 		order_buyer._game_type = order_seller._game_type;
 		order_buyer.setGetTime(order_seller.getGetTime());
 		order_buyer._trade_good_detail = order_seller._trade_good_detail;
 		order_buyer._trade_place = order_seller._trade_place;
 		order_buyer._world = order_seller._world;
 		order_buyer.setTradeMoney(order_seller.getTradeMoney());
 		order_buyer.setTradeResult(0);
 
 		order_buyer.setBuddyOrder(&order_seller);
 		order_seller.setBuddyOrder(&order_buyer);
 
 		InsertTradeRecord(order_buyer);
 	}
	m_pConnect->CommitTransaction();

	//CADODataset dataset;
	//dataset.SetConnection(m_pConnect);
	//try{
	//	m_pConnect->BeginTransaction();
	//}catch(_com_error& e){
	//	AfxMessageBox((LPCTSTR)e.Description());
	//}
	//
	//for (int i=0; i!=32766*2; i++)
	//{
	//	string testorder_id = string("test_confirm_id_") + String::fromLong(i);
	//	string test_acc = string("test_confirm_acc_") + String::fromLong(i);
	//	string test_pswd = string("test_confirm_pswd_")+ String::fromLong(i);
	//	string test_gameword = string("test_confirm_world") + String::fromLong(i);
	//	string test_role = string("test_role_name") + String::fromLong(i);
	//	string test_params = "&lm=1&rp=12345&6789098&7654321&wp=9876543&21012&3456789";
	//
	//	CString strSql;
	//	strSql.Format(L"INSERT INTO qqhx2confirm (order_id,account,password,gameworld,rolename,params,gettime)VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\"%s\")",
	//		(LPCTSTR)StringToCString(testorder_id),(LPCTSTR)StringToCString(test_acc),
	//		(LPCTSTR)StringToCString(test_pswd),(LPCTSTR)StringToCString(test_gameword),
	//		(LPCTSTR)StringToCString(test_role),(LPCTSTR)StringToCString(test_params),
	//		(LPCTSTR)StringToCString(DateTime().toString())		
	//		);
	//	try{
	//		dataset.ExecSql(strSql);
	//	}catch(_com_error& e){
	//		AfxMessageBox((LPCTSTR)dataset.dump_com_error(e));
	//	}
	//}
	//try{
	//	m_pConnect->CommitTransaction();
	//}catch(_com_error& e){
	//	AfxMessageBox((LPCTSTR)e.Description());
	//}
}

int CTDatabaseObject::InsertConfirmOrder( const CConfirmOrder& confirm )
{
	string order_tb_name = GetOrderTableName(confirm.getGameTypeId());
	if (order_tb_name == "")
	{
		return -1;
	}
	
	CString strSql;
	strSql.Format(L"INSERT INTO %s(order_id,account,password,gameworld,rolename,params,gettime)VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\"%s\")",
		(LPCTSTR)StringToCString(order_tb_name),
		(LPCTSTR)StringToCString(confirm.getOrderId()),(LPCTSTR)StringToCString(confirm.getAccount()),
		(LPCTSTR)StringToCString(confirm.getPassword()),(LPCTSTR)StringToCString(confirm.getWorld()),
		(LPCTSTR)StringToCString(confirm.getRole()),(LPCTSTR)StringToCString(confirm.getParams()),
		(LPCTSTR)StringToCString(DateTime().toString())		
		);
	CADODataset dataset;
	try{
		dataset.SetConnection(m_pConnect);
		dataset.ExecSql(strSql);
		dataset.Close();
	}catch(_com_error& e){
#ifdef _DEBUG
		AfxMessageBox((LPCTSTR)e.Description());
#endif
		LOG_ERROR(CStringToString(dataset.dump_com_error(e)).c_str());
		return 1;
	}
	return 0;
}

int CTDatabaseObject::InsertTradeRecord(  const CTradeOrder& order_seller )
{
 	CADODataset dataset;
	string order_tb_name = GetOrderTableName(order_seller._game_type);
	if (order_tb_name == ""){
		return -1;
	}

 	try{
 		HRESULT hr = dataset.m_pRecordset->Open(order_tb_name.c_str(), 
 									_variant_t((IDispatch *)m_pConnect->GetActiveConnection(), true),
 									adOpenKeyset, adLockOptimistic, adCmdTable);
		if (FAILED(hr))
		{
			return -3;
		}
		
 		dataset.SetConnection(m_pConnect);
 		if(!dataset.AddNew())
			return -2;
  		dataset.SetFieldValue(L"TradeId",StringToCString(order_seller._order_id));
		dataset.SetFieldValue(L"World",StringToCString(order_seller._world));
 		dataset.SetFieldValue(L"Seller",StringToCString(order_seller._acc));
 		dataset.SetFieldValue(L"SellerPswd",StringToCString(order_seller._pswd));
 		dataset.SetFieldValue(L"SellerRole",StringToCString(order_seller._role_name));
 		dataset.SetFieldValue(L"Buyer",StringToCString(order_seller.getBuddyOrder()->_acc));
 		dataset.SetFieldValue(L"BuyerPswd",StringToCString(order_seller.getBuddyOrder()->_pswd));
 		dataset.SetFieldValue(L"BuyerRole",StringToCString(order_seller.getBuddyOrder()->_role_name));
 		dataset.SetFieldValue(L"GetTime",COleDateTime(order_seller.getGetTime().toTime_tFormat()));
 		dataset.SetFieldValue(L"DispatchTime",COleDateTime(order_seller.getDispatchedTime().toTime_tFormat()));
 		dataset.SetFieldValue(L"TradeTime",COleDateTime(order_seller.getTradeTime().toTime_tFormat()));
	  	//dataset.SetFieldValue(L"GoodCharactic",(unsigned char*)(&order_seller._trade_good_detail),sizeof(order_seller._trade_good_detail));
		dataset.SetFieldValue(L"TradePosition",StringToCString(order_seller._trade_place));
		dataset.SetFieldValue(L"ResultSeller",(long)(order_seller.getTradeResult()));
		dataset.SetFieldValue(L"ResultBuyer",(long)(order_seller.getBuddyOrder()->getTradeResult()));
		dataset.SetFieldValue(L"TradeMoney",(long)(order_seller.getTradeMoney()));
		dataset.SetFieldValue(L"isMoneyTrade",true);
		dataset.SetFieldValue(L"SellerParams",CString(L"lm=1&rp=123gh4567&890&wp=6543218895sdfa0"));
		dataset.SetFieldValue(L"BuyerParams",CString(L"lm=1&rp=01223&4ads45566&wp=65432ad10012455"));
		
 		if(!dataset.Update())
			return -4;

 		dataset.Close();
 	}catch(_com_error& e){
#ifdef _DEBUG
		AfxMessageBox(dataset.dump_com_error(e));
#endif
		LOG_ERROR(CStringToString(dataset.dump_com_error(e)).c_str());
		return 1;
 	}
	return 0;
}

string CTDatabaseObject::GetConfirmTableName( int game_type_id ) const
{
	map<int,tbset>::const_iterator it = _confirm_order_tb_settings.find(game_type_id);
	if (it == _confirm_order_tb_settings.end())
		return string("");
	else
		return it->second.order_table_name;
}

string CTDatabaseObject::GetOrderTableName( int game_type_id ) const
{
	map<int,tbset>::const_iterator it = _trade_order_tb_settings.find(game_type_id);
	if(it == _trade_order_tb_settings.end())
		return string("");
	else
		return it->second.order_table_name;
}

string CTDatabaseObject::GetRecordTableName( int game_type_id ) const
{
	map<int,tbset>::const_iterator it = _trade_order_tb_settings.find(game_type_id);
	if (it == _trade_order_tb_settings.end())
		return string("");
	else
		return it->second.record_table_name;
}

// void CTDatabaseObject::test1()
// {
// // 	list<CTradeOrder*> olist = GetTradeNoneGetOrder(10001,-1);
// // 	for (list<CTradeOrder*>::const_iterator it = olist.begin();it != olist.end(); ++it)
// // 	{
// // 		DebugMessageA("%s",(*it)->dump().c_str());
// // 	}
// }

void CTDatabaseObject::GetNoneConfirmOrder( int game_type_id,unsigned int max_size )
{
	if (m_is_load_confirm_in_process)
	{
		return;
	}else{
		m_is_load_confirm_in_process = true;
		ParamsForGetConfirm *param = new ParamsForGetConfirm;
		param->game_type_id = game_type_id;
		param->max_size = max_size;
		PostThreadMessage(m_nWorkThread,E_TMSG_FETCH_CONFIRM,(WPARAM)this,(LPARAM)param);
	}
}

void CTDatabaseObject::GetTradeNoneGetOrder( int game_type_id,unsigned int max_size )
{
	if (m_is_load_order_in_process)
	{
		return;
	}else{
		m_is_load_order_in_process = true;
		ParamsForGetOrder *params = new ParamsForGetOrder;
		params->game_type_id = game_type_id;
		params->max_size = max_size;
		PostThreadMessage(m_nWorkThread,E_TMSG_FETCH_RECORD_ORDERS,(WPARAM)this,(LPARAM)params);
	}
}

void CTDatabaseObject::exitRecyleConfirmOrder( CConfirmOrder* confirm )
{
	CString tb_name = StringToCString(GetConfirmTableName(confirm->getGameTypeId()));
	CString strSql;
	if (confirm->getCurState() == ECS_COMPLETE)
	{
		LOG_ERROR("最后清理时发现有完成的审核订单%s",confirm->getOrderId().c_str());
		strSql.Format(L"UPDATE %s set getstate=0,resultid=%d WHERE order_id = \'%s\'",(LPCTSTR)tb_name,confirm->getResultId(),(LPCTSTR)StringToCString(confirm->getOrderId()));
	}else{
		strSql.Format(L"UPDATE %s set getstate=0,resultid=0 WHERE order_id=\'%s\'",(LPCTSTR)tb_name,(LPCTSTR)StringToCString(confirm->getOrderId()));
	}
	pushSqlSaveBuffer(strSql);
	
}
void CTDatabaseObject::exitRecyleTradeOrder( CTradeOrder* order )
{
	string order_tb_name = GetOrderTableName(order->_game_type);
	CString csOrderTableName = StringToCString(order_tb_name);
	CString strSql;
	if (order->getCurState() == EOS_GETFROM_DB || order->getCurState() == EOS_SEND_TO_DISPATCHER)//< 从数据库中取出，初始状态，未被派发，故而只需要修改getState字段。
	{
		strSql.Format(L"UPDATE %s set Getstate = 0,ResultBuyer = 0,ResultSeller = 0 where TradeId = \'%s\'",
							(LPCTSTR)csOrderTableName,(LPCTSTR)(StringToCString(order->_order_id)));
	}else if (order->getCurState() == EOS_DISPATCHED || order->getCurState() == EOS_READY_TO_TRADE){
		if (m_dbType == E_DB_TYPE_ACCESS)
		{
			strSql.Format(L"UPDATE %s set Getstate = 0,DispatchTime=#%s# where TradeId = \'%s\'",
				(LPCTSTR)csOrderTableName,(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),(LPCTSTR)(StringToCString(order->_order_id)));
		}else{
			strSql.Format(L"UPDATE %s set Getstate = 0,DispatchTime=\"%s\" where TradeId = \'%s\'",
				(LPCTSTR)csOrderTableName,(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),(LPCTSTR)(StringToCString(order->_order_id)));
		}
	}else if (order->getCurState() == EOS_TRADE_COMPLETE){
		if (order->_tradeRole == CLIENT_TYPE_BUYER)
		{
			if (m_dbType == E_DB_TYPE_ACCESS)
			{
				strSql.Format(L"UPDATE %s set Getstate = 0,ResultBuyer=%d,DispatchTime=#%s# ,TradeTime=#%s# where TradeId = \'%s\'",
					(LPCTSTR)csOrderTableName,
					order->getTradeResult(),
					(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
					(LPCTSTR)StringToCString(order->getTradeTime().toString()),
					(LPCTSTR)(StringToCString(order->_order_id)));
			}else{
				strSql.Format(L"UPDATE %s set Getstate = 0,ResultBuyer=%d,DispatchTime=\"%s\" ,TradeTime=\"%s\" where TradeId = \'%s\'",
					(LPCTSTR)csOrderTableName,
					order->getTradeResult(),
					(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
					(LPCTSTR)StringToCString(order->getTradeTime().toString()),
					(LPCTSTR)(StringToCString(order->_order_id)));
			}
		}else{
			if (m_dbType == E_DB_TYPE_ACCESS)
			{
				strSql.Format(L"UPDATE %s set Getstate = 0,ResultSeller=%d,DispatchTime=#%s# ,TradeTime=#%s# where TradeId = \'%s\'",
					(LPCTSTR)csOrderTableName,
					order->getTradeResult(),
					(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
					(LPCTSTR)StringToCString(order->getTradeTime().toString()),
					(LPCTSTR)(StringToCString(order->_order_id)));
			}else{
				strSql.Format(L"UPDATE %s set Getstate = 0,ResultSeller=%d,DispatchTime=\"%s\" ,TradeTime=\"%s\" where TradeId = \'%s\'",
					(LPCTSTR)csOrderTableName,
					order->getTradeResult(),
					(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
					(LPCTSTR)StringToCString(order->getTradeTime().toString()),
					(LPCTSTR)(StringToCString(order->_order_id)));
			}
		}
	}else{
		return;
	}

	pushSqlSaveBuffer(strSql);
}

void CTDatabaseObject::saveConfirm( CConfirmOrder* confirm,bool can_be_fetch )
{
	string confirm_tb_name = GetConfirmTableName(confirm->getGameTypeId());
	if (confirm_tb_name == "")
	{
		LOG_ERROR("没有找到游戏ID：%d对应的审核表",confirm->getGameTypeId());
		return;
	}
	CString csConfirmTableName = StringToCString(confirm_tb_name);

	CString strSql;
	if (m_dbType == E_DB_TYPE_ACCESS)
	{
		strSql.Format(L"UPDATE %s SET getstate=%d,dispatch_time=#%s#,completetime=#%s#,resultid=%d where order_id=\'%s\'",
			(LPCTSTR)csConfirmTableName,
			can_be_fetch?0:1,
			(LPCTSTR)StringToCString(confirm->getDispatchTime().toString()),
			(LPCTSTR)StringToCString(confirm->getCompleteTime().toString()),
			confirm->getResultId(),
			(LPCTSTR)StringToCString(confirm->getOrderId()));
	}else{
		strSql.Format(L"UPDATE %s SET getstate=%d,dispatch_time=\"%s\",completetime=\"%s\",resultid=%d where order_id=\'%s\'",
			(LPCTSTR)csConfirmTableName,
			can_be_fetch?0:1,
			(LPCTSTR)StringToCString(confirm->getDispatchTime().toString()),
			(LPCTSTR)StringToCString(confirm->getCompleteTime().toString()),
			confirm->getResultId(),
			(LPCTSTR)StringToCString(confirm->getOrderId()));
	}
	pushSqlSaveBuffer(strSql);
}

void CTDatabaseObject::saveTradeOrder( CTradeOrder* order,bool can_be_fetch )
{
	CString strSql;
	string order_tb_name = GetOrderTableName(order->_game_type);
	if (order_tb_name == "")
	{
		LOG_ERROR("没有找到游戏ID:%d对应的订单表",order->_game_type);
		return;
	}
	CString csOrderTableName = StringToCString(order_tb_name);

	if (order->_tradeRole == CLIENT_TYPE_BUYER)
	{
		if (m_dbType == E_DB_TYPE_ACCESS)
		{
			strSql.Format(L"UPDATE %s SET GetState = %d,DispatchTime=#%s#,TradeTime=#%s#,ResultBuyer=%d where TradeId = \'%s\'",
				(LPCTSTR)csOrderTableName,
				can_be_fetch?0:1,
				(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
				(LPCTSTR)StringToCString(order->getTradeTime().toString()),
				order->getTradeResult(),
				(LPCTSTR)StringToCString(order->_order_id));
		}else{
			strSql.Format(L"UPDATE %s SET GetState = %d,DispatchTime=\"%s\",TradeTime=\"%s\",ResultBuyer=%d where TradeId = \'%s\'",
				(LPCTSTR)csOrderTableName,
				can_be_fetch?0:1,
				(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
				(LPCTSTR)StringToCString(order->getTradeTime().toString()),
				order->getTradeResult(),
				(LPCTSTR)StringToCString(order->_order_id));
		}
	}else{
		if (m_dbType == E_DB_TYPE_ACCESS)
		{
			strSql.Format(L"UPDATE %s SET GetState = %d,DispatchTime=#%s#,TradeTime=#%s#,ResultSeller=%d where TradeId = \'%s\'",
				(LPCTSTR)csOrderTableName,
				can_be_fetch?0:1,
				(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
				(LPCTSTR)StringToCString(order->getTradeTime().toString()),
				order->getTradeResult(),
				(LPCTSTR)StringToCString(order->_order_id));
		}else{
			strSql.Format(L"UPDATE %s SET GetState = %d,DispatchTime=\"%s\",TradeTime=\"%s\",ResultSeller=%d where TradeId = \'%s\'",
				(LPCTSTR)csOrderTableName,
				can_be_fetch?0:1,
				(LPCTSTR)StringToCString(order->getDispatchedTime().toString()),
				(LPCTSTR)StringToCString(order->getTradeTime().toString()),
				order->getTradeResult(),
				(LPCTSTR)StringToCString(order->_order_id));
		}
	}

	pushSqlSaveBuffer(strSql);
}

void CTDatabaseObject::saveFatalFailedTradeRecord( CTradeOrder* seller,CTradeOrder* buyer )
{
	if (!(seller && buyer))
	{
		LOG_ERROR("程序执行错误，函数不允许出现为NULL的参数！");
		return;
	}
	//保存严重错误到数据库中，例如出现买卖上方一个成功另一个则失败的情况。需要保存处理该订单的机器。
	CString order_id = StringToCString(seller->_order_id);
	long seller_result = seller->getTradeResult();
	long buyer_result = buyer->getTradeResult();
	
	long seller_client_id = -1;
	if (seller->getClient())
		seller_client_id = seller->getClient()->getMachineId();
	
	long buyer_client_id = -1;
	if (buyer->getClient())
		buyer_client_id = buyer->getClient()->getMachineId();
	
	CString strSql;
	if (m_dbType == E_DB_TYPE_ACCESS)
	{
		strSql.Format(L"INSERT INTO trade_fatal (TradeId,seller_client_id,buyer_client_id) VALUES (\'%s\',%d,%d)",
			(LPCTSTR)order_id,seller_client_id,buyer_client_id);
	}else{
		//cause mysql do not support datetime column default 'now()';
		strSql.Format(L"INSERT INTO trade_fatal (TradeId,store_time,seller_client_id,buyer_client_id) VALUES (\'%s\',\"%s\",%d,%d)",
			(LPCTSTR)order_id,(LPCTSTR)StringToCString(DateTime().toString()),seller_client_id,buyer_client_id);
	}

	{
		CAutoLock locker(*m_cs_insert_fatal);
		m_insert_fatal_buffer.Add(strSql);
		m_insert_fatal_buffer_counter ++;
		if (m_insert_fatal_buffer_counter > 200)
		{
			CStringArray* thd_param_buffer = new CStringArray();
			thd_param_buffer->Append(m_insert_fatal_buffer);
			PostThreadMessage(m_nWorkThread,E_TMSG_saveFatalRec,(WPARAM)this,(LPARAM)thd_param_buffer);

			m_insert_fatal_buffer.RemoveAll();
			m_insert_fatal_buffer_counter = 0;
		}

	}

}

void CTDatabaseObject::saveConfirmLog( CConfirmOrder* confirm )
{
	string order_tb_name = GetConfirmTableName(confirm->getGameTypeId());
	if (order_tb_name == "")
	{
		LOG_ERROR("没有找到类型%d对应的审核表！");
		return;
	}
	string log = confirm->getLog();
	CString strSql;
	strSql.Format(L"UPDATE %s set logs=\'%s\' where order_id=\'%s\'",(LPCTSTR)StringToCString(order_tb_name),(LPCTSTR)StringToCString(log),(LPCTSTR)StringToCString(confirm->getOrderId()));
	
	pushSqlSaveBuffer(strSql);
}

void CTDatabaseObject::saveTradeOrderLog( CTradeOrder* orderSeller,CTradeOrder* orderBuyer )
{
	string order_tb_name = GetOrderTableName(orderSeller->_game_type);

	string log = orderSeller->_log + orderBuyer->_log;
	CString strOpenSql;
	strOpenSql.Format(L"update QQHX2Order set TradeLogs=\"%s\" where TradeId=\'%s\'",(LPCTSTR)StringToCString(log),(LPCTSTR)StringToCString(orderSeller->_order_id));
	pushSqlSaveBuffer(strOpenSql);
}

UINT CTDatabaseObject::threadfun_exec_database( LPVOID thread_param )
{
	// Start the message loop. 
	DebugMessageA("database work thread start now!");
	CTDatabaseObject *pthis = (CTDatabaseObject*)thread_param;
	pthis->m_thread_is_live_flag = 1;

	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ))
	{ 
		switch (msg.message)
		{
		case E_TMSG_EXEC_ONE:
			{
				DebugMessageA("work thread get E_TMSG_EXEC_ONE");
				CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
				CString* pExec = (CString*)msg.lParam;
				CADODataset dataset;
				dataset.SetConnection(lpDbObject->m_pConnect);
				try{
					dataset.ExecSql((LPCTSTR)(*pExec));
				}catch(_com_error& e){
					LOG_ERROR("%s",CStringToString(dataset.dump_com_error(e)).c_str());
#ifdef _DEBUG
					AfxMessageBox(dataset.dump_com_error(e));
#endif
				}
				dataset.Close();
				delete pExec;
			}
			break;
		case E_TMSG_saveOrder:
			{
				DebugMessageA("work thread get E_TMSG_saveOrder");
				CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
				CStringArray* pSqlArray = (CStringArray*)msg.lParam;
				lpDbObject->saveOrder_exec_sql(*pSqlArray);
				delete pSqlArray;
			}
			break;
		case E_TMSG_saveFatalRec:
			{
				DebugMessageA("work thread get E_TMSG_saveFatalRec");
				CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
				CStringArray* pSqlArray = (CStringArray*)msg.lParam;
				lpDbObject->saveFatalRec_exec_sql(*pSqlArray);
				delete pSqlArray;
			}
			break;
		case E_TMSG_EXIT:
			{
				DebugMessageA("Database work thread stop now!");
				CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
				lpDbObject->m_thread_is_live_flag = 0;
				return 0;
			}
		case E_TMSG_FETCH_CONFIRM:
			{
				DebugMessageA("work thread get E_TMSG_FETCH_CONFIRM");
				CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
				ParamsForGetConfirm* param = (ParamsForGetConfirm*)msg.lParam;
				int game_type_id = param->game_type_id;
				unsigned int max_size = param->max_size;
				list<CConfirmOrder*> li = lpDbObject->GetNoneConfirmOrderInternal(game_type_id,max_size);
				lpDbObject->_event_confirm_order_loaded(lpDbObject,game_type_id,li);
				delete param;
				lpDbObject->m_is_load_confirm_in_process = false;
				break;
			}
		case E_TMSG_FETCH_RECORD_ORDERS:
			{
				DebugMessageA("work thread get E_TMSG_FETCH_RECORD_ORDERS");
				CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
				ParamsForGetOrder* params = (ParamsForGetOrder*)msg.lParam;
				int game_type_id = params->game_type_id;
				unsigned int max_size = params->max_size;

				list<CTradeOrder*>li = lpDbObject->GetTradeNoneGetOrderInternal(game_type_id,max_size);
				lpDbObject->_event_trade_order_loaded(lpDbObject,game_type_id,li);
				delete params;
				lpDbObject->m_is_load_order_in_process = false;
				break;
			}
		default:
			break;
		}
		DebugMessageA("leave thread!");
	} 
	return 0;
}

void CTDatabaseObject::saveOrder_exec_sql(const CStringArray& m_saved_sql)
{
	setLast_flush_time(GetTickCount());
	m_pConnect->BeginTransaction();
	CADODataset dataset;
	dataset.SetConnection(m_pConnect);
	for (int i=0; i!=m_saved_sql.GetCount(); i++)
	{
		try{
			dataset.ExecSql(m_saved_sql[i]);
		}catch(_com_error& e){
			LOG_ERROR("%s",CStringToString(dataset.dump_com_error(e)).c_str());
#ifdef _DEBUG
			AfxMessageBox(dataset.dump_com_error(e));
#endif
		}				
	}
	m_pConnect->CommitTransaction();
	dataset.Close();
}

void CTDatabaseObject::saveFatalRec_exec_sql( const CStringArray& m_saved_sql )
{
	setLast_flush_time(GetTickCount());
	CADODataset dataset;
	dataset.SetConnection(m_pConnect);
	m_pConnect->BeginTransaction();
	try{
		for (int i=0;i!=m_saved_sql.GetCount();i++){
			dataset.ExecSql(m_saved_sql[i]);
		}
	}catch(_com_error& e){
		LOG_ERROR("%d==%s",__LINE__,CStringToString(dataset.dump_com_error(e)).c_str());
#ifdef _DEBUG
		AfxMessageBox(dataset.dump_com_error(e));
#endif
	}
	m_pConnect->CommitTransaction();
}

list<CConfirmOrder*> CTDatabaseObject::GetNoneConfirmOrderInternal( int game_type_id,unsigned int max_size )
{

	list<CConfirmOrder*> tolist;
	unsigned long start_tick = GetTickCount();
	CString strTableName = StringToCString(GetConfirmTableName(game_type_id));
	if (strTableName == L"")
	{
		return tolist;
	}
	CADODataset dataset;
	dataset.SetConnection(m_pConnect);
	long count = 0;
	try{
		if (m_dbType == E_DB_TYPE_ACCESS)
		{
			CString strGetCount;
			strGetCount.Format(L"SELECT COUNT(*) AS RecordTotalNum FROM %s WHERE GetState = 0 AND ResultId = 0",(LPCTSTR)strTableName);
			dataset.Open(strGetCount);
		}else{
			dataset.ExecStoreProc("CALL get_none_confirm_count()");
		}
		if (!dataset.IsOpen())
		{
			return tolist;
		}
		dataset.MoveFirst();
		dataset.GetFieldValue(L"RecordTotalNum",count);
		dataset.Close();
	}catch(_com_error& e){
		count = -1;
		LOG_ERROR(CStringToString(dataset.dump_com_error(e)).c_str());
	}

	if (count == -1)
	{
		return tolist;
	}
	if (count == 0)
	{
		int i = CRemoteDataSource::getInstance().getConfirm(game_type_id);
		if (i == 0)
		{
			DebugMessageA("not get any confirm from remote server.");
			return tolist;
		}else{
			return tolist;
		}
	}

	int get_size = max_size == -1?CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(game_type_id):max_size;

	CString strSql;
	if (m_dbType == E_DB_TYPE_ACCESS)
	{
		strSql.Format(L"SELECT TOP %d idqqhx2confirm, order_id, account, password, gameworld, rolename, params, dispatch_time,\
					   gettime, logs, completetime, resultid, getstate \
					   FROM %s where getstate = 0  AND resultid = 0\
					   ORDER BY gettime",get_size,(LPCTSTR)strTableName);
	}else{
		//remove GoodCharactic column cause ado do not support mysql's getchunk()/setchunk() function.
		strSql.Format(L"SELECT idqqhx2confirm, order_id, account, password, gameworld, rolename, params,dispatch_time,\
					   gettime, logs, completetime, resultid, getstate \
					   FROM %s where getstate = 0  AND resultid = 0\
					   ORDER BY GetTime limit %d ",(LPCTSTR)strTableName,get_size);
	}

	try{
		DebugMessageW(L"%s",(LPCTSTR)strSql);
		dataset.Open(strSql);
		if (!dataset.IsOpen())
		{
			return tolist;
		}

		DebugMessageA("count of record opened = %d",dataset.GetRecordCount());

		if (!dataset.IsEof())
		{
			dataset.MoveFirst();
		}
		
		int defRetryTime = CSettings::getInstance().getDefOrderRetryTime(game_type_id);
		bool is_log_to_text = CSettings::getInstance().getIsEnableIndividualTextLog(game_type_id);
		if (is_log_to_text)
		{
			string log_path = CSettings::getInstance().getDefOrderLogPath(game_type_id);
			if (log_path == ""){
				LOG_ERROR("游戏类型%d的日志存放路径是错误的！",game_type_id);
			}else{
				log_path += "\\";//cause File::exists need last char is '\' to identify a path.
				if (!File::exists(log_path))
				{
					File::createPath(log_path);
				}
			}
		}
		m_pConnect->BeginTransaction();
		while (!dataset.IsEof())
		{
			CConfirmOrder* order_confirm = new CConfirmOrder();
			i_order_new_counter +=1;

			order_confirm->setGameTypeId(game_type_id);

			CString temp;
			dataset.GetFieldValue(L"order_id",temp);
			order_confirm->setOrderId(CStringToString(temp));
			order_confirm->setIsLogToText(is_log_to_text);
			if (is_log_to_text)
			{
				order_confirm->initlog();//_game_type 必须先赋值。
			}

			dataset.GetFieldValue(L"gameworld",temp);
			order_confirm->setWorld(CStringToString(temp));

			dataset.GetFieldValue(L"account",temp);
			order_confirm->setAccount(CStringToString(temp));

			dataset.GetFieldValue(L"password",temp);
			order_confirm->setPassword(CStringToString(temp));

			dataset.GetFieldValue(L"rolename",temp);
			order_confirm->setRole(CStringToString(temp));

			dataset.GetFieldValue(L"params",temp);
			order_confirm->setParams(CStringToString(temp));
			
			COleDateTime tm;

			dataset.GetFieldValue(L"gettime",&tm);
			DateTime getTime(tm.GetDay(),tm.GetMonth(),tm.GetYear(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
			order_confirm->setGetTime(getTime);

			dataset.GetFieldValue(L"dispatch_time",&tm);
			DateTime DispatchTime(tm.GetDay(),tm.GetMonth(),tm.GetYear(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
			order_confirm->setDispatchTime(DispatchTime);

			dataset.GetFieldValue(L"completetime",&tm);
			DateTime TradeTime(tm.GetDay(),tm.GetMonth(),tm.GetYear(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
			order_confirm->setCompleteTime(TradeTime);

			long res = -1;
			dataset.GetFieldValue(L"resultid",res);
			order_confirm->setResultId(res);

			dataset.SetFieldValue(L"Getstate",1);
			dataset.Update();

			tolist.push_back(order_confirm);
			dataset.MoveNext();
		}
		
	}catch(_com_error& e){
#ifdef _DEBUG
		AfxMessageBox(dataset.dump_com_error(e));
#endif
		LOG_ERROR(CStringToString(dataset.dump_com_error(e)).c_str());
	}
	m_pConnect->CommitTransaction();
	dataset.Close();
	DebugMessageA("load order to list in %d ms",GetTickCount()-start_tick);

	return tolist;
}

list<CTradeOrder*> CTDatabaseObject::GetTradeNoneGetOrderInternal( int game_type_id,unsigned int max_size )
{
	//首先确认数量，若为1则先尝试去远程服务器取。

	CString strTableName = StringToCString(GetOrderTableName(game_type_id));
	CString strGetCount;
	strGetCount.Format(L"SELECT COUNT(*) AS RecordTotalNum FROM %s WHERE GetState = 0 AND ResultBuyer = 0 AND ResultSeller = 0",(LPCTSTR)strTableName);

	int i_test = 0;
	unsigned long start_tick = GetTickCount();
	

	list<CTradeOrder*> tolist;

	CADODataset dataset;
	dataset.SetConnection(m_pConnect);
	long count = 0;
	try{
		start_tick = GetTickCount();
		if (m_dbType == E_DB_TYPE_ACCESS)
		{
			dataset.Open(strGetCount);
		}else{
			dataset.ExecStoreProc("CALL get_none_use_count()");
			//当使用mysql的时候如果使用strGetCount，则发现程序和mysqld有大量的数据通信。似乎ADO将表内的所有记录都传到了客户机一样。
			//但是使用c++/connecter驱动则没有发现这个情况。具体问题待查。先使用储存过程代替一下。
		}

		if (!dataset.IsOpen())
		{
			return tolist;
		}
		dataset.MoveFirst();
		dataset.GetFieldValue(L"RecordTotalNum",count);
		dataset.Close();
	}catch(_com_error& e){
		count = -1;
		LOG_ERROR(CStringToString(dataset.dump_com_error(e)).c_str());
	}
	DebugMessageA("get trade order count complete in %d ms,remain count total:%d",GetTickCount()-start_tick,count);

	if (count == -1)
	{
		return tolist;
	}

	if (count == 0)
	{
		int i = CRemoteDataSource::getInstance().getOrder(game_type_id);
		if (i == 0)//没有从远程服务器取到任何订单
		{
			DebugMessageA("do not get any from remote server interface!");
			return tolist;
		}
	}

	start_tick = GetTickCount();

	int get_size = max_size == -1?CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(game_type_id):max_size;

	CString strSql;
	if (m_dbType == E_DB_TYPE_ACCESS)
	{
		strSql.Format(L"SELECT TOP %d Indexs, TradeId, World, Seller, SellerPswd, SellerRole, Buyer, \
					   BuyerPswd, BuyerRole, GetTime, DispatchTime, TradeTime, GoodCharactic, \
					   TradePosition, isMoneyTrade, ResultBuyer,ResultSeller,TradeMoney, Getstate,TradeLogs,sellerParams,buyerParams \
					   FROM %s where GetState = 0  AND ResultSeller = 0 AND ResultBuyer = 0 \
					   ORDER BY GetTime",get_size,(LPCTSTR)strTableName);
	}else{
		//remove GoodCharactic column cause ado do not support mysql's getchunk()/setchunk() function.
		strSql.Format(L"SELECT Indexs, TradeId, World, Seller, SellerPswd, SellerRole, Buyer, \
					   BuyerPswd, BuyerRole, GetTime, DispatchTime, TradeTime, \
					   TradePosition, isMoneyTrade, ResultBuyer,ResultSeller,TradeMoney, Getstate,TradeLogs,sellerParams,buyerParams \
					   FROM %s where GetState = 0  AND ResultSeller = 0 AND ResultBuyer = 0 \
					   ORDER BY GetTime limit %d ",(LPCTSTR)strTableName,get_size);
	}

	try{
		DebugMessageW(L"%s",(LPCTSTR)strSql);
		dataset.Open(strSql);
		if (!dataset.IsOpen())
		{
			return tolist;
		}

		DebugMessageA("dataset.Open complete in %d ms. ",GetTickCount()-start_tick);
		start_tick = GetTickCount();
		DebugMessageA("count of record opened = %d",dataset.GetRecordCount());

		if (!dataset.IsEof())
		{
			dataset.MoveFirst();
		}
		
		int defRetryTime = CSettings::getInstance().getDefOrderRetryTime(game_type_id);
		bool is_log_to_text = CSettings::getInstance().getIsEnableIndividualTextLog(game_type_id);
		if (is_log_to_text)
		{
			string log_path = CSettings::getInstance().getDefOrderLogPath(game_type_id);
			if (log_path == ""){
				LOG_ERROR("游戏类型%d的日志存放路径是错误的！",game_type_id);
			}else{
				log_path += "\\";//cause File::exists need last char is '\' to identify a path.
				if (!File::exists(log_path))
				{
					File::createPath(log_path);
				}
			}
		}
		m_pConnect->BeginTransaction();
		while (!dataset.IsEof())
		{
			CTradeOrder* order_seller = new CTradeOrder;
			CTradeOrder* order_buyer = new CTradeOrder;
			i_order_new_counter +=2;
			order_buyer->_tradeRole = CLIENT_TYPE_BUYER;
			order_seller->_tradeRole = CLIENT_TYPE_SELLER;

			order_seller->_game_type = game_type_id;
			order_buyer->_game_type = game_type_id;

			order_seller->setRetryTimes(defRetryTime);
			order_buyer->setRetryTimes(defRetryTime);

			CString temp;
			dataset.GetFieldValue(L"TradeId",temp);
			order_seller->_order_id = CStringToString(temp);
			order_buyer->_order_id = order_seller->_order_id;
			
			order_seller->setIs_log_to_text(is_log_to_text);
			order_buyer->setIs_log_to_text(is_log_to_text);
			if (is_log_to_text)
			{
				order_seller->initlog();//_game_type 必须先赋值。
				order_buyer->initlog();
			}

			dataset.GetFieldValue(L"World",temp);
			order_seller->_world = CStringToString(temp);
			order_buyer->_world = order_seller->_world;

			dataset.GetFieldValue(L"Seller",temp);
			order_seller->_acc = CStringToString(temp);

			dataset.GetFieldValue(L"SellerPswd",temp);
			order_seller->_pswd = CStringToString(temp);

			dataset.GetFieldValue(L"SellerRole",temp);
			order_seller->_role_name = CStringToString(temp);

			dataset.GetFieldValue(L"Buyer",temp);
			order_buyer->_acc = CStringToString(temp);

			dataset.GetFieldValue(L"BuyerPswd",temp);
			order_buyer->_pswd = CStringToString(temp);

			dataset.GetFieldValue(L"BuyerRole",temp);
			order_buyer->_role_name = CStringToString(temp);

			dataset.GetFieldValue(L"sellerParams",temp);
			order_seller->setOrderParams(CStringToString(temp));

			dataset.GetFieldValue(L"buyerParams",temp);
			order_buyer->setOrderParams(CStringToString(temp));

			order_seller->_buddy_role_name = order_buyer->_role_name;
			order_buyer->_buddy_role_name = order_seller->_role_name;
			
			COleDateTime tm;

			dataset.GetFieldValue(L"GetTime",&tm);
			DateTime getTime(tm.GetDay(),tm.GetMonth(),tm.GetYear(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
			order_buyer->setGetTime(getTime);
			order_seller->setGetTime(getTime);
			//*(order->_get_time) = dt1;

			dataset.GetFieldValue(L"DispatchTime",&tm);
			DateTime DispatchTime(tm.GetDay(),tm.GetMonth(),tm.GetYear(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
			order_seller->setDispatchedTime(DispatchTime);
			order_buyer->setDispatchedTime(DispatchTime);
			//order->setDispatchedTime(dt2);

			dataset.GetFieldValue(L"TradeTime",&tm);
			DateTime TradeTime(tm.GetDay(),tm.GetMonth(),tm.GetYear(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
			order_seller->setTradeTime(TradeTime);
			order_buyer->setTradeTime(TradeTime);
			//order->setTradeTime(dt3);

			dataset.GetFieldValue(L"TradePosition",temp);
			string trade_place = CStringToString(temp);
			order_seller->_trade_place = trade_place;
			order_buyer->_trade_place = trade_place;
			//order->_trade_place = CStringToString(temp);


			long res = -1;
			dataset.GetFieldValue(L"ResultSeller",res);
			order_seller->setTradeResult((short)res);
			dataset.GetFieldValue(L"ResultBuyer",res);
			order_buyer->setTradeResult((short)res);

			dataset.GetFieldValue(L"isMoneyTrade",order_seller->_type_trade_is_money);
			order_buyer->_type_trade_is_money = order_seller->_type_trade_is_money;
			
			dataset.GetFieldValue(L"TradeMoney",res);
			order_buyer->setTradeMoney(res);
			order_seller->setTradeMoney(res);

// 			int i = sizeof(Wares);
// 			dataset.GetFieldValue(L"GoodCharactic",(unsigned char*)&order_seller->_trade_good_detail,&i);
// 			order_buyer->_trade_good_detail = order_seller->_trade_good_detail;
			
			CString log;
			dataset.GetChunk(L"TradeLogs",log);
			order_seller->_log = CStringToString(log);
			order_buyer->_log = "";

			dataset.SetFieldValue(L"Getstate",1);
			//dataset.SetChunk(L"TradeLogs",L"app");
			dataset.Update();

			order_seller->setBuddyOrder(order_buyer);
			order_buyer->setBuddyOrder(order_seller);

			tolist.push_back(order_seller);
			tolist.push_back(order_buyer);

			dataset.MoveNext();
		}
		
	}catch(_com_error& e){
#ifdef _DEBUG
		AfxMessageBox(dataset.dump_com_error(e));
#endif
		LOG_ERROR(CStringToString(dataset.dump_com_error(e)).c_str());
	}
	m_pConnect->CommitTransaction();
	dataset.Close();
	DebugMessageA("load order to list in %d ms",GetTickCount()-start_tick);
	start_tick = GetTickCount();
	return tolist;
}

void CTDatabaseObject::flush()
{
	{
		CAutoLock Locer(*m_cs_save);
		if (m_save_buffer.GetCount() == 0)
		{
			goto flush_ins;
		}
		CStringArray* thd_param_buffer = new CStringArray();
		thd_param_buffer->Append(m_save_buffer);
		PostThreadMessage(m_nWorkThread,E_TMSG_saveOrder,(WPARAM)this,(LPARAM)thd_param_buffer);
		//			saveOrder_exec_sql(m_save_buffer);
		m_save_buffer.RemoveAll();
		m_save_buffer_counter=0;
	}
flush_ins:
	{
		CAutoLock locker(*m_cs_insert_fatal);
		if (m_insert_fatal_buffer.GetCount() == 0)
		{
			goto flush_end;
		}
		CStringArray* thd_param_buffer = new CStringArray();
		thd_param_buffer->Append(m_insert_fatal_buffer);
		PostThreadMessage(m_nWorkThread,E_TMSG_saveFatalRec,(WPARAM)this,(LPARAM)thd_param_buffer);
		m_insert_fatal_buffer.RemoveAll();
		m_insert_fatal_buffer_counter = 0;
	}
flush_end:
	return;
}

UINT CTDatabaseObject::threadfun_flush( LPVOID thread_param )
{
	CTDatabaseObject* pDBO = (CTDatabaseObject*)thread_param;
	SetTimer(NULL,111,1000,NULL);
	pDBO->m_flushthread_is_live_flag = true;
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		switch(msg.message){
			case WM_TIMER:
				{
					if (GetTickCount() - pDBO->getLast_flush_time() > 10000 || GetTickCount() - pDBO->getLast_flush_time() < 0){
						pDBO->flush();
						pDBO->setLast_flush_time(GetTickCount());
						DebugMessageA("触发定时刷新数据库缓存！");
					}else{
//						DebugMessageA("检查通过，无需刷新！");
					}
					break;
				}
			case E_TMSG_EXIT:
				{
					DebugMessageA("flush thread exit now!");
					CTDatabaseObject* lpDbObject = (CTDatabaseObject*)msg.wParam;
					lpDbObject->m_flushthread_is_live_flag = false;
					return 0;
				}
			default:
				break;
		}
	}
	pDBO->m_flushthread_is_live_flag = false;
	return 0;
}

void CTDatabaseObject::pushSqlSaveBuffer( const CString& strSql )
{
	ASSERT(strSql.GetLength() != 0);
	CAutoLock Locer(*m_cs_save);
	m_save_buffer.Add(strSql);
	m_save_buffer_counter++;

	if (m_save_buffer_counter > 200)
	{
		CStringArray* thd_param_buffer = new CStringArray();
		thd_param_buffer->Append(m_save_buffer);
		PostThreadMessage(m_nWorkThread,E_TMSG_saveOrder,(WPARAM)this,(LPARAM)thd_param_buffer);
		//			saveOrder_exec_sql(m_save_buffer);
		m_save_buffer.RemoveAll();
		m_save_buffer_counter=0;
	}
}

