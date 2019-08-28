#pragma once
#include "noncopyable.h"
#include "../common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

class INetworkMsgPoster;

#define CLIENT_OP_ERROR_CODE_SUCCESS	0		//< �����ڿͻ���

class CTradeSession :
	public NonCopyable
{
public:
	CTradeSession(const order_struct& order/*,INetworkMsgPoster* network*/);
	~CTradeSession(void);
public:
/*	INetworkMsgPoster* getNetwork() const { return _network; }*/
	void setProgressInfors( EClientOrderProgress val, bool last_op_result = true, int Failed_reason=CLIENT_OP_ERROR_CODE_SUCCESS );

	//< ��ֹ���������ϴβ��������ʧ��ԭ�򣬶��߱���ֻ��setProgressInfors���á�
	bool getLastOperationResult() const { return _last_operation_result; }
	int getFailedReason() const { return _failed_reason; }
	EClientOrderProgress getProgress() const { return _progress; }
	
	//< ��ֹ�ͻ�����ʹ�ù������޸Ķ�����Ϣ
	order_struct getOrderInfo() const { return _order_info; }
	
	void server_permit_to_trade();
	void server_deny_to_trade();
	bool cancel_trade_imm(int reason);

	boost::signal<void (CTradeSession* /*sender*/,EClientOrderProgress /*now*/,EClientOrderProgress /*prevous*/)> _event_progress_changed;
private:
	order_struct _order_info;			//< ��ǰ����Ķ�����Ϣ
	EClientOrderProgress _progress;		//< ��ǰ����
	//INetworkMsgPoster* _network;		//< ����Խ������Ϣ�Ľӿڣ�һ�㲻������
	bool _last_operation_result;		//<	��¼�ϴβ����ɹ����ı�־λ
	int _failed_reason;					//< ����ϴβ���ʧ���ˣ���ʧ�ܵ�ԭ��ID���浽���
	bool _permission_to_trade;			//< �Ƿ�������

};
