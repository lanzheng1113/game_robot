#pragma once

#include <list>
#include <string>
#include <map>

using std::string;
using std::list;
using std::map;

enum ADODB_DataBaseType{
	E_DB_TYPE_ACCESS,
	E_DB_TYPE_MYSQL,
	E_DB_TYPE_MSSQL,
};

class CTradeOrder;
class CConfirmOrder;
class CLock_CS;
namespace sql{
	namespace mysql{
		class MySQL_Driver;
	}
	class Connection;
};
class CTDatabaseObject
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
	CTDatabaseObject();
	/**
	* @FIXME:当m_pConnect已经创建，但未连接数据库时，delete m_pConnect导致异常，该异常由::CoUninitialize();引起。
	*/
	~CTDatabaseObject();
public:
	virtual void					Close();
	/**
	 * 将order收回数据库，根据order不同的状态执行不同的行动
	 * EOS_GETFROM_DB || EOS_SEND_TO_DISPATCHER 未被派发到客户机，故而只需要修改getState字段
	 * EOS_DISPATCHED || EOS_READY_TO_TRADE 未交易，修改getstate和dispatchtime
	 * EOS_TRADE_COMPLETE,交易完成，修改result,getstate和dispatchtime字段。
	 */
	virtual void					exitRecyleTradeOrder( CTradeOrder* order );
	
	virtual void					exitRecyleConfirmOrder(CConfirmOrder* confirm);

	virtual void					flush();
	/**
	 * 获得没有取出过的订单，每从数据库取出一个订单，都设置其状态为已取出，避免多次取出同一个订单
	 * @param game_type_id,指定游戏类型
	 * @param max_size,指定本次最多取出多少个。
	 * @return 返回取出的列表
	 */
	virtual list<CTradeOrder*>		GetTradeNoneGetOrder(int game_type_id,unsigned int max_size);

	virtual list<CConfirmOrder*>	GetNoneConfirmOrder(int game_type_id,unsigned int max_size);


	virtual BOOL					Init(	const CString& strDbName,const CString& strUserName,
									const CString& strPassword,ADODB_DataBaseType dbType);

	virtual int						InsertTradeRecord( const CTradeOrder& order);
	
	virtual int						InsertConfirmOrder(const CConfirmOrder& confirm);
	/**
	 * 保存order信息到数据库，can_be_fetch指定该订单是否能被再次取出【getState=0或1】。
	 */
	virtual void					saveTradeOrder(CTradeOrder* order,bool can_be_fetch);

	virtual void					saveConfirm(CConfirmOrder* order,bool can_be_fetch);
	/**
	 * 保存order的log信息到数据库
	 */
	virtual void					saveTradeOrderLog(CTradeOrder* orderSeller,CTradeOrder* orderBuyer);
	
	virtual void					saveConfirmLog(CConfirmOrder* confirm);
	/**
	 * 保存故障信息到数据库。 
	 */
	virtual void					saveFatalFailedTradeRecord(CTradeOrder* seller,CTradeOrder* buyer);
public:
protected:
	static UINT				threadfun_exec_database(LPVOID thread_param);
	static UINT				threadfun_flush(LPVOID thread_param);
private:

	string					GetRecordTableName(int game_type_id) const;
	string					GetOrderTableName(int game_type_id) const;
	string					GetConfirmTableName(int game_type_id) const;
	list<CTradeOrder*>		GetTradeNoneGetOrder_in_db_thread(int game_type_id,unsigned int max_size);
	list<CConfirmOrder*>	GetNoneConfirmOrderInternal(int game_type_id,unsigned int max_size);
	void					pushSqlSaveBuffer(const CString& strSql);
	void					saveOrder_exec_sql(const CStringArray& m_saved_sql);
	void					saveFatalRec_exec_sql(const CStringArray& m_saved_sql);
	void					test1();

private:

	map<int,tbset>			_trade_order_tb_settings;
	map<int,tbset>			_confirm_order_tb_settings;

	sql::mysql::MySQL_Driver*	m_driver;
	sql::Connection*		m_connection;
	
	CString					m_strEventTableName;

	CStringArray			m_save_buffer;
	int						m_save_buffer_counter;
	CLock_CS*				m_cs_save;

	CStringArray			m_insert_fatal_buffer;
	int						m_insert_fatal_buffer_counter;
	CLock_CS*				m_cs_insert_fatal;

	HANDLE					m_hEventGetNoneRecord;
	HANDLE					m_hWorkThread;
	DWORD					m_nWorkThread;
	int						m_thread_is_live_flag;

	HANDLE					m_hEventGetConfirm;
	
	DWORD					m_nFlushThread;
	bool					m_flushthread_is_live_flag;
	list<CTradeOrder*>		m_orderList_for_workthread_return;
	list<CConfirmOrder*>	m_confirm_list_for_workthread_return;
};
