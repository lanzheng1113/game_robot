#pragma once

#include "util/Interface.h"
#include "TradeCodeDef.h"
#include <string>
using std::string;
struct order_struct;
class CTradeOrder;
/**
 * @class CTradeNetworkInterface
 * 定义服务器向客户端发送消息的接口。
 */
class CTradeNetworkInterface :
	public Interface
{
public:
	CTradeNetworkInterface(void);
	virtual ~CTradeNetworkInterface(void);
public:
	virtual BOOL PM_AssignOrder(order_struct* order) = 0;				//< 分配订单给连接的客户端
	virtual void PM_AssignOrderFailed(EDispatchError errCode) = 0;		//< 拒绝客户端的订单请求	
	virtual BOOL PM_CancelAllOrders() = 0;								//< 发送立即停止客户端的所有订单的命令
	virtual void KillSession() = 0;										//< 关闭会话（连接）
	virtual void PM_StateChangedAckMsg(int order_dispatch_id,int error_code)=0;	//< 当客户端发送进度改变消息时，发送对该消息的响应。
	virtual void PM_StateChangedAck_HasGetOrder_X(int order_dispatch_id,int error_code) = 0;
	virtual void PM_MyBuddyFailed(int order_dispatch_id,short buddy_error_code) = 0;

	virtual BOOL PM_AssignConfirm(order_struct* order) = 0;
	virtual void PM_ConfirmStateChangedAckMsg(int order_dispatch_id,int error_code) = 0;
	virtual void PM_ConfirmHasGetOrder_X(int order_dispatch_id,int error_code) = 0;
	virtual void PM_CancelAllConfirms() = 0;
};
