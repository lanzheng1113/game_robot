#ifndef _DBM_H
#define _DBM_H
#include "ATLCOMTIME.H"
#include "../common/wares.h"
#include "../common/shareStruct.h"
#include <vector>
#include <map>
#include <list>
#include <string>
using std::vector;
using std::map;
using std::list;
using std::string;
#include "IDataBaseObject.h"

class CADOConnection;
enum ADODB_DataBaseType;
class CTradeOrder;
class CConfirmOrder;
class CLock_CS;

class CTDatabaseObject : public IDatabaseObject
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
	CTDatabaseObject(ADODB_DataBaseType dbType,LPCTSTR DBName,LPCTSTR User,LPCTSTR Password);
	CTDatabaseObject(ADODB_DataBaseType dbType,LPCTSTR DBName);
	/**
	* @FIXME:当m_pConnect已经创建，但未连接数据库时，delete m_pConnect导致异常，该异常由::CoUninitialize();引起。
	*/
	~CTDatabaseObject();
public:
	void					Close();
	/**
	 * 将order收回数据库，根据order不同的状态执行不同的行动
	 * EOS_GETFROM_DB || EOS_SEND_TO_DISPATCHER 未被派发到客户机，故而只需要修改getState字段
	 * EOS_DISPATCHED || EOS_READY_TO_TRADE 未交易，修改getstate和dispatchtime
	 * EOS_TRADE_COMPLETE,交易完成，修改result,getstate和dispatchtime字段。
	 */
	void					exitRecyleTradeOrder( CTradeOrder* order );
	
	void					exitRecyleConfirmOrder(CConfirmOrder* confirm);

	void					flush();
	/**
	 * 获得没有取出过的订单，每从数据库取出一个订单，都设置其状态为已取出，避免多次取出同一个订单
	 * @param game_type_id,指定游戏类型
	 * @param max_size,指定本次最多取出多少个。
	 * @return 返回取出的列表
	 */
	void					GetTradeNoneGetOrder(int game_type_id,unsigned int max_size);

	void					GetNoneConfirmOrder(int game_type_id,unsigned int max_size);
	
	ADODB_DataBaseType		getDbType() const { return m_dbType; }
	
	CString					getDBName() const { return m_strDBName; }
	
	CString					getUser() const { return m_strUser; }
	
	CString					getPassword() const { return m_strPassword; }
	
	DWORD					getLast_flush_time() const { return m_last_flush_time; }

	BOOL					Init(	const CString& strDbName,const CString& strUserName,
									const CString& strPassword,ADODB_DataBaseType dbType);
	//void Connect(CADOConnection* val) { m_pConnect = val; }
	int						InsertTradeRecord( const CTradeOrder& order);
	
	int						InsertConfirmOrder(const CConfirmOrder& confirm);
	/**
	 * 保存order信息到数据库，can_be_fetch指定该订单是否能被再次取出【getState=0或1】。
	 */
	void					saveTradeOrder(CTradeOrder* order,bool can_be_fetch);

	void					saveConfirm(CConfirmOrder* order,bool can_be_fetch);
	/**
	 * 保存order的log信息到数据库
	 */
	void					saveTradeOrderLog(CTradeOrder* orderSeller,CTradeOrder* orderBuyer);
	
	void					saveConfirmLog(CConfirmOrder* confirm);
	/**
	 * 保存故障信息到数据库。 
	 */
	void					saveFatalFailedTradeRecord(CTradeOrder* seller,CTradeOrder* buyer);

	void					setDbType(ADODB_DataBaseType val) { m_dbType = val; }

	void					setDBName(CString val) { m_strDBName = val; }

	void					setUser(CString val) { m_strUser = val; }

	void					setPassword(CString val) { m_strPassword = val; }

	void					setLast_flush_time(DWORD val) { InterlockedExchange((LONG*)&m_last_flush_time , val); }
public:
protected:
	static UINT				threadfun_exec_database(LPVOID thread_param);
	static UINT				threadfun_flush(LPVOID thread_param);
private:

	string					GetRecordTableName(int game_type_id) const;
	string					GetOrderTableName(int game_type_id) const;
	string					GetConfirmTableName(int game_type_id) const;
	list<CTradeOrder*>		GetTradeNoneGetOrderInternal(int game_type_id,unsigned int max_size);
	list<CConfirmOrder*>	GetNoneConfirmOrderInternal(int game_type_id,unsigned int max_size);
	void					pushSqlSaveBuffer(const CString& strSql);
	void					saveOrder_exec_sql(const CStringArray& m_saved_sql);
	void					saveFatalRec_exec_sql(const CStringArray& m_saved_sql);
	void					test1();

private:

	map<int,tbset>			_trade_order_tb_settings;
	map<int,tbset>			_confirm_order_tb_settings;
	CADOConnection*			m_pConnect;
	CString					m_strEventTableName;
	ADODB_DataBaseType		m_dbType;
	CString					m_strDBName;
	CString					m_strUser;
	CString					m_strPassword;

	CStringArray			m_save_buffer;
	int						m_save_buffer_counter;
	CLock_CS*				m_cs_save;

	CStringArray			m_insert_fatal_buffer;
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


#endif