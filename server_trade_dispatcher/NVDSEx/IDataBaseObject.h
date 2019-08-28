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
	 * ��order�ջ����ݿ⣬����order��ͬ��״ִ̬�в�ͬ���ж�
	 * EOS_GETFROM_DB || EOS_SEND_TO_DISPATCHER δ���ɷ����ͻ������ʶ�ֻ��Ҫ�޸�getState�ֶ�
	 * EOS_DISPATCHED || EOS_READY_TO_TRADE δ���ף��޸�getstate��dispatchtime
	 * EOS_TRADE_COMPLETE,������ɣ��޸�result,getstate��dispatchtime�ֶΡ�
	 */
	virtual void			exitRecyleTradeOrder( CTradeOrder* order ) = 0;
	
	virtual void			exitRecyleConfirmOrder(CConfirmOrder* confirm) = 0;

	virtual void			flush() = 0;
	/**
	 * ���û��ȡ�����Ķ�����ÿ�����ݿ�ȡ��һ����������������״̬Ϊ��ȡ����������ȡ��ͬһ������
	 * @param game_type_id,ָ����Ϸ����
	 * @param max_size,ָ���������ȡ�����ٸ���
	 * @return ����ȡ�����б�
	 */
	virtual void			GetTradeNoneGetOrder(int game_type_id,unsigned int max_size) = 0;

	virtual void			GetNoneConfirmOrder(int game_type_id,unsigned int max_size) = 0;


	virtual BOOL			Init(	const CString& strDbName,const CString& strUserName,
									const CString& strPassword,ADODB_DataBaseType dbType) = 0;

	virtual int				InsertTradeRecord( const CTradeOrder& order) = 0;
	
	virtual int				InsertConfirmOrder(const CConfirmOrder& confirm) = 0;
	/**
	 * ����order��Ϣ�����ݿ⣬can_be_fetchָ���ö����Ƿ��ܱ��ٴ�ȡ����getState=0��1����
	 */
	virtual void			saveTradeOrder(CTradeOrder* order,bool can_be_fetch) = 0;

	virtual void			saveConfirm(CConfirmOrder* order,bool can_be_fetch) = 0;
	/**
	 * ����order��log��Ϣ�����ݿ�
	 */
	virtual void			saveTradeOrderLog(CTradeOrder* orderSeller,CTradeOrder* orderBuyer) = 0;
	
	virtual void			saveConfirmLog(CConfirmOrder* confirm) = 0;	
	/**
	 * ���������Ϣ�����ݿ⡣ 
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