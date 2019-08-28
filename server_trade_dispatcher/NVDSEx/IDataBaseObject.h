#pragma once

#include "util/Interface.h"
#include "../common/dbtypedef.h"
#include "util/Event.h"
#include <list>
using std::list;
class CTradeOrder;
class CConfirmOrder;

class IDatabaseObject: public Interface
{
public:
	IDatabaseObject(){;}
	virtual ~IDatabaseObject(){;}
public:
	virtual void			Close() = 0;
	/**
	 * 将order收回数据库，根据order不同的状态执行不同的行动
	 * EOS_GETFROM_DB || EOS_SEND_TO_DISPATCHER 未被派发到客户机，故而只需要修改getState字段
	 * EOS_DISPATCHED || EOS_READY_TO_TRADE 未交易，修改getstate和dispatchtime
	 * EOS_TRADE_COMPLETE,交易完成，修改result,getstate和dispatchtime字段。
	 */
	virtual void			exitRecyleTradeOrder( CTradeOrder* order ) = 0;
	
	virtual void			exitRecyleConfirmOrder(CConfirmOrder* confirm) = 0;

	virtual void			flush() = 0;
	/**
	 * 获得没有取出过的订单，每从数据库取出一个订单，都设置其状态为已取出，避免多次取出同一个订单
	 * @param game_type_id,指定游戏类型
	 * @param max_size,指定本次最多取出多少个。
	 * @return 返回取出的列表
	 */
	virtual void			GetTradeNoneGetOrder(int game_type_id,unsigned int max_size) = 0;

	virtual void			GetNoneConfirmOrder(int game_type_id,unsigned int max_size) = 0;


	virtual BOOL			Init(	const CString& strDbName,const CString& strUserName,
									const CString& strPassword,ADODB_DataBaseType dbType) = 0;

	virtual int				InsertTradeRecord( const CTradeOrder& order) = 0;
	
	virtual int				InsertConfirmOrder(const CConfirmOrder& confirm) = 0;
	/**
	 * 保存order信息到数据库，can_be_fetch指定该订单是否能被再次取出【getState=0或1】。
	 */
	virtual void			saveTradeOrder(CTradeOrder* order,bool can_be_fetch) = 0;

	virtual void			saveConfirm(CConfirmOrder* order,bool can_be_fetch) = 0;
	/**
	 * 保存order的log信息到数据库
	 */
	virtual void			saveTradeOrderLog(CTradeOrder* orderSeller,CTradeOrder* orderBuyer) = 0;
	
	virtual void			saveConfirmLog(CConfirmOrder* confirm) = 0;	
	/**
	 * 保存故障信息到数据库。 
	 */
	virtual void			saveFatalFailedTradeRecord(CTradeOrder* seller,CTradeOrder* buyer) = 0;	

	ADODB_DataBaseType		getDbType() const { return m_dbType; }

	CString					getDBName() const { return m_strDBName; }

	CString					getUser() const { return m_strUser; }

	CString					getPassword() const { return m_strPassword; }

	DWORD					getLast_flush_time() const { return m_last_flush_time; }

	void					setDbType(ADODB_DataBaseType val) { m_dbType = val; }

	void					setDBName(CString val) { m_strDBName = val; }

	void					setUser(CString val) { m_strUser = val; }

	void					setPassword(CString val) { m_strPassword = val; }

	void					setLast_flush_time(DWORD val) { InterlockedExchange((LONG*)&m_last_flush_time , val); }

	Event<void (IDatabaseObject*,int,list<CTradeOrder*>)> _event_trade_order_loaded;
	Event<void (IDatabaseObject*,int,list<CConfirmOrder*>)> _event_confirm_order_loaded;
private:

	ADODB_DataBaseType		m_dbType;
	CString					m_strDBName;
	CString					m_strUser;
	CString					m_strPassword;

	DWORD					m_last_flush_time;
};

IDatabaseObject *			afkGetDbObject();