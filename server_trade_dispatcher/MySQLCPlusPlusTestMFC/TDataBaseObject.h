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
	* @FIXME:��m_pConnect�Ѿ���������δ�������ݿ�ʱ��delete m_pConnect�����쳣�����쳣��::CoUninitialize();����
	*/
	~CTDatabaseObject();
public:
	virtual void					Close();
	/**
	 * ��order�ջ����ݿ⣬����order��ͬ��״ִ̬�в�ͬ���ж�
	 * EOS_GETFROM_DB || EOS_SEND_TO_DISPATCHER δ���ɷ����ͻ������ʶ�ֻ��Ҫ�޸�getState�ֶ�
	 * EOS_DISPATCHED || EOS_READY_TO_TRADE δ���ף��޸�getstate��dispatchtime
	 * EOS_TRADE_COMPLETE,������ɣ��޸�result,getstate��dispatchtime�ֶΡ�
	 */
	virtual void					exitRecyleTradeOrder( CTradeOrder* order );
	
	virtual void					exitRecyleConfirmOrder(CConfirmOrder* confirm);

	virtual void					flush();
	/**
	 * ���û��ȡ�����Ķ�����ÿ�����ݿ�ȡ��һ����������������״̬Ϊ��ȡ����������ȡ��ͬһ������
	 * @param game_type_id,ָ����Ϸ����
	 * @param max_size,ָ���������ȡ�����ٸ���
	 * @return ����ȡ�����б�
	 */
	virtual list<CTradeOrder*>		GetTradeNoneGetOrder(int game_type_id,unsigned int max_size);

	virtual list<CConfirmOrder*>	GetNoneConfirmOrder(int game_type_id,unsigned int max_size);


	virtual BOOL					Init(	const CString& strDbName,const CString& strUserName,
									const CString& strPassword,ADODB_DataBaseType dbType);

	virtual int						InsertTradeRecord( const CTradeOrder& order);
	
	virtual int						InsertConfirmOrder(const CConfirmOrder& confirm);
	/**
	 * ����order��Ϣ�����ݿ⣬can_be_fetchָ���ö����Ƿ��ܱ��ٴ�ȡ����getState=0��1����
	 */
	virtual void					saveTradeOrder(CTradeOrder* order,bool can_be_fetch);

	virtual void					saveConfirm(CConfirmOrder* order,bool can_be_fetch);
	/**
	 * ����order��log��Ϣ�����ݿ�
	 */
	virtual void					saveTradeOrderLog(CTradeOrder* orderSeller,CTradeOrder* orderBuyer);
	
	virtual void					saveConfirmLog(CConfirmOrder* confirm);
	/**
	 * ���������Ϣ�����ݿ⡣ 
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
