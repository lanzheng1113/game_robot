#pragma once

#include "util/Interface.h"
#include "TradeCodeDef.h"
#include <string>
using std::string;
struct order_struct;
class CTradeOrder;
/**
 * @class CTradeNetworkInterface
 * �����������ͻ��˷�����Ϣ�Ľӿڡ�
 */
class CTradeNetworkInterface :
	public Interface
{
public:
	CTradeNetworkInterface(void);
	virtual ~CTradeNetworkInterface(void);
public:
	virtual BOOL PM_AssignOrder(order_struct* order) = 0;				//< ���䶩�������ӵĿͻ���
	virtual void PM_AssignOrderFailed(EDispatchError errCode) = 0;		//< �ܾ��ͻ��˵Ķ�������	
	virtual BOOL PM_CancelAllOrders() = 0;								//< ��������ֹͣ�ͻ��˵����ж���������
	virtual void KillSession() = 0;										//< �رջỰ�����ӣ�
	virtual void PM_StateChangedAckMsg(int order_dispatch_id,int error_code)=0;	//< ���ͻ��˷��ͽ��ȸı���Ϣʱ�����ͶԸ���Ϣ����Ӧ��
	virtual void PM_StateChangedAck_HasGetOrder_X(int order_dispatch_id,int error_code) = 0;
	virtual void PM_MyBuddyFailed(int order_dispatch_id,short buddy_error_code) = 0;

	virtual BOOL PM_AssignConfirm(order_struct* order) = 0;
	virtual void PM_ConfirmStateChangedAckMsg(int order_dispatch_id,int error_code) = 0;
	virtual void PM_ConfirmHasGetOrder_X(int order_dispatch_id,int error_code) = 0;
	virtual void PM_CancelAllConfirms() = 0;
};
