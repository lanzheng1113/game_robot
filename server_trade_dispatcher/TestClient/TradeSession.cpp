#include "StdAfx.h"
#include "TradeSession.h"
#include "trader.h"
#include "Logs.h"

CTradeSession::CTradeSession(const order_struct& order/*,INetworkMsgPoster* network*/)
{
	_progress = EOP_standby;
	_order_info = order;
	/*_network = network;*/
	_failed_reason = 0;
	_last_operation_result = false;
	_permission_to_trade = false;
}

CTradeSession::~CTradeSession(void)
{
	
}

void CTradeSession::setProgressInfors( EClientOrderProgress val ,bool last_op_result/* = true*/, int Failed_reason/*=CLIENT_OP_ERROR_CODE_SUCCESS*/ )
{
	if (val != _progress)
	{
		EClientOrderProgress PreviousProgress = _progress;
		
		_progress = val;
		_failed_reason = Failed_reason;
		_last_operation_result = last_op_result;

		_event_progress_changed(this,val,PreviousProgress);
	}
}

void CTradeSession::server_permit_to_trade()
{	
	if (!_permission_to_trade)
	{
		_permission_to_trade = true;
	}else{
		C_log("已经确定被允许交易，不能再次发送允许交易消息！");
	}
}

void CTradeSession::server_deny_to_trade()
{
	CTrader::getInstance().ExecCancelTrade(this,TRADE_FAILED_REASON_SERVER_COMMAND_DENY);
}

bool CTradeSession::cancel_trade_imm(int reason)
{
	if (_progress == EOP_trading)
	{//正在进行的交易不允许被中断
		printf("\t%s正在进行的交易不允许被中断",__FUNCTION__);
		return false;
	}
	
	if (_progress == EOP_trade_end)
	{//已经交易完的订单需要等待上报服务器。
		printf("\t%s已经交易完的订单需要等待上报服务器。",__FUNCTION__);
		return false;
	}
	
	//@fixme:在这里添加代码，处理立即交易的代码。
	CTrader::getInstance().ExecCancelTrade(this,reason);
	return true;
}
