#pragma once
#include "noncopyable.h"
#include <string>
using std::string;

class INetworkMsgPoster :
	public NonCopyable
{
public:

	INetworkMsgPoster(void){
	}

	virtual ~INetworkMsgPoster(void){
	}
public:
	virtual bool PM_HasGetOrder(int order_dispatch_id) = 0; //< 向服务器报告确认信息：我已经接收到了订单。订单为order_id，作为的角色是is_as_seller?出售方:购买方
	virtual bool PM_EnterRole(int order_dispatch_id) = 0; //< 我已进入游戏角色。
	virtual bool PM_EnterTradeStage(int order_dispatch_id) = 0; //< 进入交易地点
	virtual bool PM_ReadyToTrade(int order_dispatch_id)=0; //< 准备好交易
	virtual bool PM_Trading(int order_dispatch_id)=0;//正在交易
	virtual bool PM_TradeSuccess(int order_dispatch_id)=0;	//<交易成功
	virtual bool PM_TradeFailed(int order_dispatch_id,int fail_reason)=0; //< 交易失败
	virtual bool PM_INeedAOrder(int burden)=0;									//< 我现在空闲，我需要一个订单，这是我的负载信息。

};
