#pragma once
#include "idatabaseobject.h"
#include <string>
using std::string;
#include <map>
using std::map;
#include <list>
using std::list;
#include <vector>
using std::vector;

namespace sql{
	namespace mysql{
		class MySQL_Driver;
	}
	class Connection;
}
class CLock_CS;

class CMySQLCppConDB :
	public IDatabaseObject
{
public:
	enum{
		E_TMSG_saveOrder = (WM_USER + 300),
		E_TMSG_saveFatalRec = (WM_USER + 301),
		E_TMSG_EXEC_ONE = (WM_USER + 302),
		E_TMSG_FETCH_RECORD_ORDERS = (WM_USER + 303),
		E_TMSG_EXIT = (WM_USER+304),
		E_TMSG_FETCH_CONFIRM = (WM_USER + 305)
	};	

	struct ParamsForGetOrder{
		int game_type_id;
		int max_size;
	};
	typedef ParamsForGetOrder ParamsForGetConfirm;	

	class tbset{
	public:
		string order_table_name;
		string record_table_name;
	};
public:
	CMySQLCppConDB(void);
	~CMySQLCppConDB(void);

	virtual void Close();

	virtual void exitRecyleConfirmOrder( CConfirmOrder* confirm );

	virtual void exitRecyleTradeOrder( CTradeOrder* order );
	
	virtual void flush();

	virtual void GetNoneConfirmOrder( int game_type_id,unsigned int max_size );

	virtual void GetTradeNoneGetOrder( int game_type_id,unsigned int max_size );

	virtual BOOL Init( const CString& strDbName,const CString& strUserName, const CString& strPassword,ADODB_DataBaseType dbType );

	virtual int InsertConfirmOrder( const CConfirmOrder& confirm );

	virtual int InsertTradeRecord( const CTradeOrder& order );

	virtual void saveTradeOrderLog( CTradeOrder* orderSeller,CTradeOrder* orderBuyer );

	virtual void saveFatalFailedTradeRecord( CTradeOrder* seller,CTradeOrder* buyer );

	virtual void saveTradeOrder( CTradeOrder* order,bool can_be_fetch );

	virtual void saveConfirm( CConfirmOrder* order,bool can_be_fetch );

	virtual void saveConfirmLog( CConfirmOrder* confirm );
protected:
	static UINT				threadfun_exec_database(LPVOID thread_param);
	static UINT				threadfun_flush(LPVOID thread_param);
private:
	string					GetOrderTableName(int game_type_id) const;
	string					GetConfirmTableName(int game_type_id) const;
	list<CTradeOrder*>		GetTradeNoneGetOrderInternal(int game_type_id,unsigned int max_size);
	list<CConfirmOrder*>	GetNoneConfirmOrderInternal(int game_type_id,unsigned int max_size);
	void					pushSqlSaveBuffer(const string& strSql);
	void					saveOrder_exec_sql(const vector<string>& m_saved_sql);
	void					saveFatalRec_exec_sql(const vector<string>& m_saved_sql);
private:
	sql::mysql::MySQL_Driver *_driver;
	sql::Connection *_con;

	map<int,tbset>			_trade_order_tb_settings;
	map<int,tbset>			_confirm_order_tb_settings;

	vector<string>			m_save_buffer;
	int						m_save_buffer_counter;
	CLock_CS*				m_cs_save;

	vector<string>			m_insert_fatal_buffer;
	int						m_insert_fatal_buffer_counter;
	CLock_CS*				m_cs_insert_fatal;

	HANDLE					m_hWorkThread;
	DWORD					m_nWorkThread;
	int						m_thread_is_live_flag;

	volatile bool			m_is_load_confirm_in_process;
	volatile bool			m_is_load_order_in_process;

	DWORD					m_last_flush_time;
	DWORD					m_nFlushThread;
	bool					m_flushthread_is_live_flag;
};
