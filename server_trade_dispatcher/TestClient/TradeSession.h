#pragma once
#include "noncopyable.h"
#include "../common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

class INetworkMsgPoster;

#define CLIENT_OP_ERROR_CODE_SUCCESS	0		//< 仅用于客户端

class CTradeSession :
	public NonCopyable
{
public:
	CTradeSession(const order_struct& order/*,INetworkMsgPoster* network*/);
	~CTradeSession(void);
public:
/*	INetworkMsgPoster* getNetwork() const { return _network; }*/
	void setProgressInfors( EClientOrderProgress val, bool last_op_result = true, int Failed_reason=CLIENT_OP_ERROR_CODE_SUCCESS );

	//< 禁止单独设置上次操作结果和失败原因，二者必须只由setProgressInfors设置。
	bool getLastOperationResult() const { return _last_operation_result; }
	int getFailedReason() const { return _failed_reason; }
	EClientOrderProgress getProgress() const { return _progress; }
	
	//< 禁止客户端在使用过程中修改订单信息
	order_struct getOrderInfo() const { return _order_info; }
	
	void server_permit_to_trade();
	void server_deny_to_trade();
	bool cancel_trade_imm(int reason);

	boost::signal<void (CTradeSession* /*sender*/,EClientOrderProgress /*now*/,EClientOrderProgress /*prevous*/)> _event_progress_changed;
private:
	order_struct _order_info;			//< 当前处理的订单信息
	EClientOrderProgress _progress;		//< 当前进度
	//INetworkMsgPoster* _network;		//< 用于越级发消息的接口，一般不用它。
	bool _last_operation_result;		//<	记录上次操作成功与否的标志位
	int _failed_reason;					//< 如果上次操作失败了，将失败的原因ID保存到这里。
	bool _permission_to_trade;			//< 是否被允许交易

};
