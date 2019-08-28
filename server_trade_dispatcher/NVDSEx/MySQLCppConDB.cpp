#include "StdAfx.h"
#include "MySQLCppConDB.h" 
#include <driver/mysql_driver.h>
#include <driver/mysql_connection.h>
#include <cppconn/driver.h>   
#include <cppconn/exception.h>   
#include <cppconn/resultset.h>   
#include <cppconn/statement.h>   
#include <cppconn/prepared_statement.h>   

#include "../common/DebugMessage.h"
#include "../common/shareStruct.h"
#include "../common/stringtool.h"
#include "../common/AutoLock.h"
#include "util/DateTime.h"
#include <util/Logger.h>
#include "util/File.h"
#include "TradeOrder.h"
#include "Settings.h"
#include "RemoteDataSource.h"
#include "tradeClientMachine.h"
#include "ConfirmOrder.h"
#include <sstream>
	extern int i_order_new_counter;
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}

CMySQLCppConDB::CMySQLCppConDB(void)
{
	_driver = NULL;
	_con = NULL;
	m_is_load_confirm_in_process = false;
	m_is_load_order_in_process = false;
}

CMySQLCppConDB::~CMySQLCppConDB(void)
{
	SAFE_DELETE(_con);
}


int CMySQLCppConDB::InsertTradeRecord( const CTradeOrder& order )
{
	return 0;
}

int CMySQLCppConDB::InsertConfirmOrder( const CConfirmOrder& confirm )
{
	//throw std::exception("The method or operation is not implemented.");
	return 0;
}

void CMySQLCppConDB::exitRecyleTradeOrder( CTradeOrder* order )
{
	string order_tb_name = GetOrderTableName(order->_game_type);
	std::stringstream ss;
	if (order->getCurState() == EOS_GETFROM_DB || order->getCurState() == EOS_SEND_TO_DISPATCHER)//< 从数据库中取出，初始状态，未被派发，故而只需要修改getState字段。
	{
		ss << "update " << order_tb_name << " set Getstate = 0,ResultBuyer = 0,ResultSeller = 0 where TradeId = \'"<< order->_order_id << "\'";
	}else if (order->getCurState() == EOS_DISPATCHED || order->getCurState() == EOS_READY_TO_TRADE){
		ss << "UPDATE " << order_tb_name << " set Getstate = 0,DispatchTime=\"" << order->getDispatchedTime().toString() << "\""
			<< " where TradeId = \'"<<order->_order_id << "\'";
	}else if (order->getCurState() == EOS_TRADE_COMPLETE){
		if (order->_tradeRole == CLIENT_TYPE_BUYER)
		{
			ss << "UPDATE " << order_tb_name << " set Getstate = 0,ResultBuyer="<<order->getTradeResult()
				<< ",DispatchTime=\"" << order->getDispatchedTime().toString() << "\",TradeTime=\"" <<order->getTradeTime().toString()
				<< "\" where TradeId = \'" << order->_order_id << "\'";
		}else{
			ss << "UPDATE " << order_tb_name << " set Getstate = 0,ResultSeller="<<order->getTradeResult()
				<< ",DispatchTime=\"" << order->getDispatchedTime().toString() << "\",TradeTime=\"" <<order->getTradeTime().toString()
				<< "\" where TradeId = \'" << order->_order_id << "\'";
		}
	}else{
		return;
	}

	pushSqlSaveBuffer(ss.str());
}

void CMySQLCppConDB::saveTradeOrderLog( CTradeOrder* orderSeller,CTradeOrder* orderBuyer )
{
	string order_tb_name = GetOrderTableName(orderSeller->_game_type);

	string log = orderSeller->_log + orderBuyer->_log;
	std::stringstream ss;
	ss << "update "<< order_tb_name << " set TradeLogs = \"" << log << "\" where TradeId = \'"<<orderSeller->_order_id << "\'";
	pushSqlSaveBuffer(ss.str());
}

void CMySQLCppConDB::GetNoneConfirmOrder( int game_type_id,unsigned int max_size )
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

void CMySQLCppConDB::GetTradeNoneGetOrder( int game_type_id,unsigned int max_size )
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

BOOL CMySQLCppConDB::Init( const CString& strDbName,const CString& strUserName, const CString& strPassword,ADODB_DataBaseType dbType )
{
	if (dbType != E_DB_TYPE_MYSQL)
	{
		AfxMessageBox(L"CMySQLCppConDB 仅支持 mysql 数据库。");
		return FALSE;
	}

	m_cs_save = new CLock_CS();
	m_cs_insert_fatal = new CLock_CS();
	m_save_buffer_counter = 0;
	m_insert_fatal_buffer_counter = 0;

	setDBName(strDbName);
	setPassword(strPassword);
	setUser(strUserName);
	setDbType(dbType);

	//connect to db
	sql::ConnectOptionsMap opt;
	opt["hostName"] = string("tcp://127.0.0.1:3306");
	opt["userName"] = CStringToString(strUserName);
	opt["password"] = CStringToString(strPassword);
	opt["schema"] = CStringToString(strDbName);
	bool login_and_set_char = true;
	try{	
		_driver = sql::mysql::get_mysql_driver_instance();
		if (!_driver)
		{
			return FALSE;
		}
		_con = _driver->connect(opt);
		boost::scoped_ptr<sql::Statement> stmt (_con->createStatement());
		stmt->execute("SET NAMES \"GB2312\"");
	}catch(sql::SQLException& e){
		login_and_set_char = false;
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
		MessageBoxA(NULL,ss.str().c_str(),"数据库登录错误",MB_OK|MB_ICONERROR);
	}

	if (!login_and_set_char)
	{
		return FALSE;
	}

 	m_thread_is_live_flag = 0;
 	CWinThread* ptd = AfxBeginThread(CMySQLCppConDB::threadfun_exec_database,this);
 	if (!ptd)
 	{
 		return FALSE;
 	}
 	m_hWorkThread = ptd->m_hThread;
 	m_nWorkThread = ptd->m_nThreadID;
 
 	m_flushthread_is_live_flag = false;
 	CWinThread* ptd2 = AfxBeginThread(CMySQLCppConDB::threadfun_flush,this);
 	if (!ptd2)
 	{
 		return FALSE;
 	}
 	m_nFlushThread = ptd2->m_nThreadID;



 	m_last_flush_time = GetTickCount();

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
	return TRUE;
}

void CMySQLCppConDB::saveFatalFailedTradeRecord( CTradeOrder* seller,CTradeOrder* buyer )
{
	if (!(seller && buyer))
	{
		LOG_ERROR("程序执行错误，函数不允许出现为NULL的参数！");
		return;
	}
	//保存严重错误到数据库中，例如出现买卖上方一个成功另一个则失败的情况。需要保存处理该订单的机器。
	long seller_result = seller->getTradeResult();
	long buyer_result = buyer->getTradeResult();

	long seller_client_id = -1;
	if (seller->getClient())
		seller_client_id = seller->getClient()->getMachineId();

	long buyer_client_id = -1;
	if (buyer->getClient())
		buyer_client_id = buyer->getClient()->getMachineId();

	//cause mysql do not support datetime column default 'now()';
	std::stringstream ss;
	ss << "INSERT INTO trade_fatal (TradeId,store_time,seller_client_id,buyer_client_id) VALUES (\'"<<seller->_order_id << "\',\""
		<< DateTime().toString() << "\"," << seller_client_id << "," << buyer_client_id << ")";
	{
		CAutoLock locker(*m_cs_insert_fatal);
		m_insert_fatal_buffer.push_back(ss.str());
		m_insert_fatal_buffer_counter ++;
		if (m_insert_fatal_buffer_counter > 200)
		{
			vector<string>* thd_param_buffer = new vector<string>;
			*thd_param_buffer = m_insert_fatal_buffer;
			PostThreadMessage(m_nWorkThread,E_TMSG_saveFatalRec,(WPARAM)this,(LPARAM)thd_param_buffer);

			m_insert_fatal_buffer.clear();
			m_insert_fatal_buffer_counter = 0;
		}
	}
}

void CMySQLCppConDB::flush()
{
	{
		CAutoLock Locer(*m_cs_save);
		if (m_save_buffer.size() == 0)
		{
			goto flush_ins;
		}
		vector<string>* thd_param_buffer = new vector<string>;
		*thd_param_buffer = m_save_buffer;
		PostThreadMessage(m_nWorkThread,E_TMSG_saveOrder,(WPARAM)this,(LPARAM)thd_param_buffer);
		//			saveOrder_exec_sql(m_save_buffer);
		m_save_buffer.clear();
		m_save_buffer_counter=0;
	}
flush_ins:
	{
		CAutoLock locker(*m_cs_insert_fatal);
		
		if (m_insert_fatal_buffer.size() == 0)
		{
			goto flush_end;
		}

		vector<string>* thd_param_buffer = new vector<string>;
		*thd_param_buffer = m_insert_fatal_buffer;

		PostThreadMessage(m_nWorkThread,E_TMSG_saveFatalRec,(WPARAM)this,(LPARAM)thd_param_buffer);
		m_insert_fatal_buffer.clear();
		m_insert_fatal_buffer_counter = 0;
	}
flush_end:
	return;
}

void CMySQLCppConDB::saveTradeOrder( CTradeOrder* order,bool can_be_fetch )
{
	string order_tb_name = GetOrderTableName(order->_game_type);
	if (order_tb_name == "")
	{
		LOG_ERROR("没有找到游戏ID:%d对应的订单表",order->_game_type);
		return;
	}

	std::stringstream ss;
	if (order->_tradeRole == CLIENT_TYPE_BUYER)
	{
		ss << "UPDATE " << order_tb_name << " SET GetState = " << (can_be_fetch?0:1) 
			<< " ,DispatchTime=\"" << order->getDispatchedTime().toString() << "\",TradeTime=\"" << order->getTradeTime().toString() << "\",ResultBuyer=" << order->getTradeResult() << " where TradeId = \'" << order->_order_id << "\'";
	}
	else{
		ss << "UPDATE " << order_tb_name << " SET GetState = " << (can_be_fetch?0:1) << " ,DispatchTime=\""
			<< order->getDispatchedTime().toString() << "\",TradeTime=\"" << order->getTradeTime().toString() << "\",ResultSeller="
			<< order->getTradeResult() << " where TradeId = \'" << order->_order_id << "\'";
	}

	pushSqlSaveBuffer(ss.str());
}

void CMySQLCppConDB::Close()
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
			DebugMessageA("等待数据库缓冲区刷新线程退出!");
		}
		Sleep(100);
	}
	return;
}

void CMySQLCppConDB::saveConfirm( CConfirmOrder* confirm,bool can_be_fetch )
{
	string confirm_tb_name = GetConfirmTableName(confirm->getGameTypeId());
	if (confirm_tb_name == "")
	{
		LOG_ERROR("没有找到游戏ID：%d对应的审核表",confirm->getGameTypeId());
		return;
	}

	std::stringstream ss;
	ss << "UPDATE " << confirm_tb_name << " SET getstate=" << (can_be_fetch?0:1) << ",dispatch_time=\"" << confirm->getDispatchTime().toString()
		<< "\",completetime=\"" << confirm->getCompleteTime().toString() << "\",resultid=" << confirm->getResultId()
		<< " where order_id=\'" << confirm->getOrderId() << "\'";
	pushSqlSaveBuffer(ss.str());
}

void CMySQLCppConDB::exitRecyleConfirmOrder( CConfirmOrder* confirm )
{
	string tb_name = GetConfirmTableName(confirm->getGameTypeId());
	std::stringstream ss;
	if (confirm->getCurState() == ECS_COMPLETE)
	{
		LOG_ERROR("最后清理时发现有完成的审核订单%s",confirm->getOrderId().c_str());
		ss << "UPDATE " << tb_name << " set getstate=0,resultid=" << confirm->getResultId() << " WHERE order_id = \'" << confirm->getOrderId() << "\'";
	}else{
		ss << "UPDATE " << tb_name << " set getstate=0,resultid=0 WHERE order_id=\'"  << confirm->getOrderId() << "\'";
	}
	pushSqlSaveBuffer(ss.str());
}

void CMySQLCppConDB::saveConfirmLog( CConfirmOrder* confirm )
{
	string order_tb_name = GetConfirmTableName(confirm->getGameTypeId());
	if (order_tb_name == "")
	{
		LOG_ERROR("没有找到类型%d对应的审核表！");
		return;
	}
	string log = confirm->getLog();

	std::stringstream ss;
	ss << "UPDATE " << order_tb_name << " set logs=\'" << log << "\' where order_id=\'" << confirm->getOrderId() << "\'";
	pushSqlSaveBuffer(ss.str());
}

UINT CMySQLCppConDB::threadfun_exec_database( LPVOID thread_param )
{
	// Start the message loop. 
	DebugMessageA("database work thread start now!");
	CMySQLCppConDB *pthis = (CMySQLCppConDB*)thread_param;
	pthis->m_thread_is_live_flag = 1;

	MSG msg;
	while( GetMessage( &msg, NULL, 0, 0 ))
	{ 
		switch (msg.message)
		{
		case E_TMSG_EXEC_ONE:
			{
				DebugMessageA("work thread get E_TMSG_EXEC_ONE");
				CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
				string* pExec = (string*)msg.lParam;
				sql::Statement* stmt = NULL;
				try{
					lpDbObject->_con->setAutoCommit(true);
					stmt = lpDbObject->_con->createStatement();
					stmt->execute(*pExec);
				}catch(sql::SQLException& e){
					std::stringstream ss;
					ss << "# ERR: SQLException in " << __FILE__;
					ss << "(" << __FUNCTION__ << ") on line "
						<< __LINE__ << std::endl;
					ss << "# ERR: " << e.what();
					ss << " (MySQL error code: " << e.getErrorCode();
					ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
					DebugMessageA("%s",ss.str().c_str());
					LOG_ERROR("%s",ss.str().c_str());
				}
				if (stmt)
				{
					delete stmt;
				}

				if (pExec)
				{
					delete pExec;
				}
				
			}
			break;
		case E_TMSG_saveOrder:
			{
				DebugMessageA("work thread get E_TMSG_saveOrder");
				CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
				vector<string>* pSqlArray = (vector<string>*)msg.lParam;
				lpDbObject->saveOrder_exec_sql(*pSqlArray);
				delete pSqlArray;
			}
			break;
		case E_TMSG_saveFatalRec:
			{
				DebugMessageA("work thread get E_TMSG_saveFatalRec");
				CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
				vector<string>* pSqlArray = (vector<string>*)msg.lParam;
				lpDbObject->saveFatalRec_exec_sql(*pSqlArray);
				delete pSqlArray;
			}
			break;
		case E_TMSG_EXIT:
			{
				DebugMessageA("数据库工作线程退出!");
				CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
				lpDbObject->m_thread_is_live_flag = 0;
				return 0;
			}
		case E_TMSG_FETCH_CONFIRM:
			{
				DebugMessageA("work thread get E_TMSG_FETCH_CONFIRM");
				CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
				ParamsForGetConfirm* param = (ParamsForGetConfirm*)msg.lParam;
				int game_type_id = param->game_type_id;
				unsigned int max_size = param->max_size;

				list<CConfirmOrder*> clst = lpDbObject->GetNoneConfirmOrderInternal(game_type_id,max_size);
				lpDbObject->_event_confirm_order_loaded(lpDbObject,game_type_id,clst);
				lpDbObject->m_is_load_confirm_in_process = false;
				delete param;
				break;
			}
		case E_TMSG_FETCH_RECORD_ORDERS:
			{
				DebugMessageA("work thread get E_TMSG_FETCH_RECORD_ORDERS");
				CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
				ParamsForGetOrder* params = (ParamsForGetOrder*)msg.lParam;
				int game_type_id = params->game_type_id;
				unsigned int max_size = params->max_size;

				list<CTradeOrder*> li = lpDbObject->GetTradeNoneGetOrderInternal(game_type_id,max_size);
				lpDbObject->_event_trade_order_loaded(lpDbObject,game_type_id,li);
				lpDbObject->m_is_load_order_in_process = false;
				delete params;
				break;
			}
		default:
			break;
		}
		DebugMessageA("leave thread!");
	} 
	return 0;
}

UINT CMySQLCppConDB::threadfun_flush( LPVOID thread_param )
{
	CMySQLCppConDB* pDBO = (CMySQLCppConDB*)thread_param;
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
					DebugMessageA("缓冲区刷新线程退出！");
					CMySQLCppConDB* lpDbObject = (CMySQLCppConDB*)msg.wParam;
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

void CMySQLCppConDB::pushSqlSaveBuffer( const string& strSql )
{
	ASSERT(strSql.length() != 0);
	CAutoLock locker(*m_cs_save);
	m_save_buffer.push_back(strSql);
	m_save_buffer_counter++;
	if (m_save_buffer_counter > 200)
	{
		vector<string>* thd_param_buffer = new vector<string>;
		*thd_param_buffer = m_save_buffer;
		PostThreadMessage(m_nWorkThread,E_TMSG_saveOrder,(WPARAM)this,(LPARAM)thd_param_buffer);
		m_save_buffer.clear();
		m_save_buffer_counter = 0;
	}
}

void CMySQLCppConDB::saveOrder_exec_sql( const vector<string>& saved_sql )
{
	sql::Statement* stmt = NULL;
	bool cmt = true;

	try{
		stmt = _con->createStatement();
		if (!stmt)
		{
			return;
		}
		cmt = _con->getAutoCommit();
		_con->setAutoCommit(false);
	}catch(sql::SQLException& e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
		if(stmt){
			delete stmt;
		}
		return;
	}

	for (int i=0; i!=saved_sql.size(); i++)
	{
		try{
			stmt->executeUpdate(saved_sql[i]);
		}catch(sql::SQLException &e){
			std::stringstream ss;
			ss << "# ERR: SQLException in " << __FILE__;
			ss << "(" << __FUNCTION__ << ") on line "
				<< __LINE__ << std::endl;
			ss << "# ERR: " << e.what();
			ss << " (MySQL error code: " << e.getErrorCode();
			ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
			DebugMessageA("%s",ss.str().c_str());
			LOG_ERROR("%s",ss.str().c_str());
		}
	}
	setLast_flush_time(GetTickCount());

	try{
		_con->commit();
		_con->setAutoCommit(cmt);
	}catch(sql::SQLException& e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
	}

	if (stmt)
	{
		delete stmt;
	}
}

void CMySQLCppConDB::saveFatalRec_exec_sql( const vector<string>& saved_sql )
{
	sql::Statement *stmt = NULL;
	bool acmt = true;

	try{
		stmt = _con->createStatement();
		if (!stmt)
		{
			return;
		}
		acmt = _con->getAutoCommit();
		_con->setAutoCommit(false);
	}catch(sql::SQLException& e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
		return;
	}

	for (int i = 0; i!=saved_sql.size();i++)
	{
		try{

			stmt->execute(saved_sql[i]);
		}catch(sql::SQLException& e){
			std::stringstream ss;
			ss << "# ERR: SQLException in " << __FILE__;
			ss << "(" << __FUNCTION__ << ") on line "
				<< __LINE__ << std::endl;
			ss << "# ERR: " << e.what();
			ss << " (MySQL error code: " << e.getErrorCode();
			ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
			DebugMessageA("%s",ss.str().c_str());
			LOG_ERROR("%s",ss.str().c_str());
		}
	}
	setLast_flush_time(GetTickCount());
	try{
		_con->commit();
		_con->setAutoCommit(acmt);
	}catch(sql::SQLException& e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
	}
	if (stmt)
	{
		delete stmt;
	}
}

string CMySQLCppConDB::GetOrderTableName( int game_type_id ) const
{
	map<int,tbset>::const_iterator it = _trade_order_tb_settings.find(game_type_id);
	if(it == _trade_order_tb_settings.end())
		return string("");
	else
		return it->second.order_table_name;
}

list<CTradeOrder*> CMySQLCppConDB::GetTradeNoneGetOrderInternal( int game_type_id,unsigned int max_size )
{
	//首先确认数量，若为1则先尝试去远程服务器取。
	string strTableName = GetOrderTableName(game_type_id);
	std::stringstream ss;
	ss << "SELECT COUNT(*) AS RecordTotalNum FROM " << strTableName << " WHERE GetState = 0 AND ResultBuyer = 0 AND ResultSeller = 0";

 	int i_test = 0;
 	unsigned long start_tick = GetTickCount();
 
 	list<CTradeOrder*> tolist;

	int iCount = 0;

	try{
		boost::scoped_ptr<sql::Statement> stmt(_con->createStatement());
		boost::scoped_ptr<sql::ResultSet>rs(stmt->executeQuery(ss.str()));
		rs->next();
		iCount = rs->getInt(1);
	}catch(sql::SQLException &e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
		iCount = -1;
	}

	if (iCount == -1)
	{
		return tolist;
	}

	if (iCount == 0)
	{
 		int i = CRemoteDataSource::getInstance().getOrder(game_type_id);
 		if (i == 0)//没有从远程服务器取到任何订单
 		{
 			DebugMessageA("do not get any from remote server interface!");
 			return tolist;
 		}
	}

 	start_tick = GetTickCount();
 
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

 	int get_size = max_size == -1?CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(game_type_id):max_size;
 
 	std::stringstream s0;
	s0 << "SELECT Indexs, TradeId, World, Seller, SellerPswd, SellerRole, Buyer,BuyerPswd, BuyerRole, GetTime, DispatchTime, TradeTime,"
		  << "TradePosition, isMoneyTrade, ResultBuyer,ResultSeller,TradeMoney, Getstate,TradeLogs,sellerParams,buyerParams FROM "
		  << strTableName << " where GetState = 0  AND ResultSeller = 0 AND ResultBuyer = 0 ORDER BY GetTime limit " << get_size;
	try{
		boost::scoped_ptr<sql::Statement> stmt(_con->createStatement());
		boost::scoped_ptr<sql::ResultSet> rs(stmt->executeQuery(s0.str()));
		vector<int> get_indexs_vec;
		while (rs->next())
		{
			get_indexs_vec.push_back(rs->getInt("Indexs"));
			CTradeOrder* order_seller = new CTradeOrder;
			CTradeOrder* order_buyer = new CTradeOrder;
			i_order_new_counter +=2;
 			order_buyer->_tradeRole = CLIENT_TYPE_BUYER;
 			order_seller->_tradeRole = CLIENT_TYPE_SELLER;
			order_seller->_game_type = game_type_id;
			order_buyer->_game_type = game_type_id;

			order_seller->setRetryTimes(defRetryTime);
			order_buyer->setRetryTimes(defRetryTime);

			order_buyer->_order_id = order_seller->_order_id = rs->getString("TradeId");
 			order_seller->setIs_log_to_text(is_log_to_text);
 			order_buyer->setIs_log_to_text(is_log_to_text);
 			if (is_log_to_text)
 			{
 				order_seller->initlog();//_game_type 必须先赋值。
 				order_buyer->initlog();
 			}

			order_buyer->_world = order_seller->_world = rs->getString("World");
			order_seller->_acc = rs->getString("Seller");
			order_seller->_pswd = rs->getString("SellerPswd");
			order_seller->_role_name = rs->getString("SellerRole");
			order_buyer->_acc = rs->getString("Buyer");
			order_buyer->_pswd = rs->getString("BuyerPswd");
			order_buyer->_role_name = rs->getString("BuyerRole");
			order_buyer->setOrderParams(rs->getString("buyerParams"));
			order_seller->setOrderParams(rs->getString("sellerParams"));

 			order_seller->_buddy_role_name = order_buyer->_role_name;
 			order_buyer->_buddy_role_name = order_seller->_role_name;

			DateTime dt_get(1,1,2000,0,0,0);
			DateTime dt_disp(1,1,2000,0,0,0);
			DateTime dt_end(1,1,2000,0,0,0);
			
			DateTime::fromString(rs->getString("GetTime"),dt_get);
			order_seller->setGetTime(dt_get);
			order_buyer->setGetTime(dt_get);
			
			DateTime::fromString(rs->getString("DispatchTime"),dt_disp);
			order_seller->setDispatchedTime(dt_disp);
			order_buyer->setDispatchedTime(dt_disp);

			DateTime::fromString(rs->getString("TradeTime"),dt_end);
			order_buyer->setTradeTime(dt_end);
			order_seller->setTradeTime(dt_end);

			order_buyer->_trade_place = order_seller->_trade_place = rs->getString("TradePosition");

			order_seller->setTradeResult((short)rs->getInt("ResultSeller"));
			order_buyer->setTradeResult((short)rs->getInt("ResultBuyer"));
			
			order_buyer->_type_trade_is_money = order_seller->_type_trade_is_money = rs->getBoolean("isMoneyTrade");
			int imn = rs->getInt("TradeMoney");
			order_seller->setTradeMoney(imn);
			order_buyer->setTradeMoney(imn);

			order_seller->_log = rs->getString("TradeLogs");
			order_buyer->_log = "";

 			order_seller->setBuddyOrder(order_buyer);
 			order_buyer->setBuddyOrder(order_seller);

			tolist.push_back(order_seller);
			tolist.push_back(order_buyer);
		}
		std::stringstream s1;
		s1 << "update " << strTableName << " set getState=1 where indexs = ?;";
		_con->setAutoCommit(false);
		boost::scoped_ptr<sql::PreparedStatement> pstmt(_con->prepareStatement(s1.str()));
		for (int i=0; i!=get_indexs_vec.size(); i++)
		{
			pstmt->setInt(1,get_indexs_vec[i]);
			pstmt->executeUpdate();
		}
		_con->commit();
		_con->setAutoCommit(true);
	}catch(sql::SQLException& e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
		iCount = -1;
	}

	DebugMessageA("load order to list in %d ms",GetTickCount()-start_tick);
	return tolist;
}

list<CConfirmOrder*> CMySQLCppConDB::GetNoneConfirmOrderInternal( int game_type_id,unsigned int max_size )
{
	list<CConfirmOrder*> tolist;
	unsigned long start_tick = GetTickCount();
	string strTableName = GetConfirmTableName(game_type_id);
	if (strTableName == "")
	{
		return tolist;
	}

	int icount = 0;
	try{
		boost::scoped_ptr<sql::Statement>stmt(_con->createStatement());
		std::stringstream ss;
		ss << "SELECT COUNT(*) AS RecordTotalNum FROM " << strTableName << " WHERE GetState = 0 AND ResultId = 0";
		boost::scoped_ptr<sql::ResultSet>rs(stmt->executeQuery(ss.str()));
		rs->next();
		icount = rs->getInt(1);
	}catch(sql::SQLException& e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
		icount = -1;
	}
	
	DebugMessageA("get count in %d ms",GetTickCount() - start_tick);
	start_tick = GetTickCount();

	if (icount == -1)
	{
		return tolist;
	}
	if (icount == 0)
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
	std::stringstream s2;
	s2 << "SELECT idqqhx2confirm, order_id, account, password, gameworld, rolename, params,dispatch_time, gettime, logs, completetime, resultid, getstate FROM "
		<< strTableName << " where getstate = 0  AND resultid = 0 ORDER BY GetTime limit " << get_size;
	
	try{
		boost::scoped_ptr<sql::Statement> stmt(_con->createStatement());
		boost::scoped_ptr<sql::ResultSet> rs(stmt->executeQuery(s2.str()));
		DebugMessageA("select in %d ms",GetTickCount()-start_tick);
		start_tick = GetTickCount();
		vector<int> id_get_vec;
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

		while(rs->next()){
			id_get_vec.push_back(rs->getInt("idqqhx2confirm"));
 			CConfirmOrder* order_confirm = new CConfirmOrder();
 			i_order_new_counter +=1;
			order_confirm->setGameTypeId(game_type_id);
			order_confirm->setOrderId(rs->getString("order_id"));
			order_confirm->setIsLogToText(is_log_to_text);
			if (is_log_to_text)
			{
				order_confirm->initlog();
			}
			order_confirm->setWorld(rs->getString("gameworld"));
			order_confirm->setAccount(rs->getString("account"));
			order_confirm->setPassword(rs->getString("password"));
			order_confirm->setRole(rs->getString("rolename"));
			order_confirm->setParams(rs->getString("params"));

			DateTime dt1(1,1,2000,0,0,0),dt2(1,1,2000,0,0,0),dt3(1,1,2000,0,0,0);
			string str = rs->getString("gettime");
			DateTime::fromString(str,dt1);
			order_confirm->setGetTime(dt1);

			str = rs->getString("dispatch_time");
			DateTime::fromString(str,dt2);
			order_confirm->setDispatchTime(dt2);

			str = rs->getString("completetime");
			DateTime::fromString(str,dt3);
			order_confirm->setCompleteTime(dt3);

			order_confirm->setResultId(rs->getInt("resultid"));

			tolist.push_back(order_confirm);
		}

		if (!tolist.empty())
		{
			_con->setAutoCommit(false);
			std::stringstream ss3;
			ss3 << "update " << strTableName << " set getstate = 1 where idqqhx2confirm = ?";
			boost::scoped_ptr<sql::PreparedStatement>pstmt(_con->prepareStatement(ss3.str()));
			for (int i=0; i!=id_get_vec.size();i++)
			{
				pstmt->setInt(1,id_get_vec[i]);
				pstmt->executeUpdate();
			}
			_con->commit();
			_con->setAutoCommit(true);
		}
	}catch(sql::SQLException &e){
		std::stringstream ss;
		ss << "# ERR: SQLException in " << __FILE__;
		ss << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << std::endl;
		ss << "# ERR: " << e.what();
		ss << " (MySQL error code: " << e.getErrorCode();
		ss << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		DebugMessageA("%s",ss.str().c_str());
		LOG_ERROR("%s",ss.str().c_str());
	}
	
	DebugMessageA("load to list in %d ms",GetTickCount() - start_tick);

	return tolist;
}

string CMySQLCppConDB::GetConfirmTableName( int game_type_id ) const
{
	map<int,tbset>::const_iterator it = _confirm_order_tb_settings.find(game_type_id);
	if (it == _confirm_order_tb_settings.end())
		return string("");
	else
		return it->second.order_table_name;
}

