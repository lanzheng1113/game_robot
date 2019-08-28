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
	virtual bool PM_HasGetOrder(int order_dispatch_id) = 0; //< �����������ȷ����Ϣ�����Ѿ����յ��˶���������Ϊorder_id����Ϊ�Ľ�ɫ��is_as_seller?���۷�:����
	virtual bool PM_EnterRole(int order_dispatch_id) = 0; //< ���ѽ�����Ϸ��ɫ��
	virtual bool PM_EnterTradeStage(int order_dispatch_id) = 0; //< ���뽻�׵ص�
	virtual bool PM_ReadyToTrade(int order_dispatch_id)=0; //< ׼���ý���
	virtual bool PM_Trading(int order_dispatch_id)=0;//���ڽ���
	virtual bool PM_TradeSuccess(int order_dispatch_id)=0;	//<���׳ɹ�
	virtual bool PM_TradeFailed(int order_dispatch_id,int fail_reason)=0; //< ����ʧ��
	virtual bool PM_INeedAOrder(int burden)=0;									//< �����ڿ��У�����Ҫһ�������������ҵĸ�����Ϣ��

};
