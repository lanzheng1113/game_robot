#pragma once
#include "util/NonCopyable.h"
#include "tradeClientMachine.h"
#include "type_game_setting.h"
#include "../common/shareStruct.h"
class DateTime;
class COrderDispatcher;
struct order_struct;

#define __cbk

class CTradeMacDispatcher : public NonCopyable
{
public:
	CTradeMacDispatcher();
	~CTradeMacDispatcher();
public:
	void AddClientMacControler(CClientMacControler* controler);

	COrderDispatcher* GetOrderDispatcher() const { return order_dispatcher; }
	void SetOrderDispatcher(COrderDispatcher* val) { order_dispatcher = val; }

	bool addMac(int game_type,CTradeClient* mac);

	/**
	 * 收到一个客户端希望获得订单的消息
	 */
	int DealTradeRequest(CTradeClient* mac,int mac_burden);

	/**
	 * 获得当前正在处理的订单总数
	 */
	int getCurrentProcessOrderCount() const;

	int GetCurrentProcessConfirmCount() const;
	/**
	 *
	 */
	CTradeClient* getMac(int game_id, int c_id, const string& name);

	string dump()const;

	void exit();			//温柔退出
	void force_exit();		//暴力退出
private:
	/**
	 * 派发一个订单，将其派发给两台机器。
	 * @remark:当允许多开时，允许将订单分配给同一台机器。
	 */
	int dispatchOrder(CTradeOrder* order,CTradeClient* client);
	void corder_to_order_struct(IN CTradeOrder* order,OUT order_struct* o_struct);
	/**/
	int dispatchConfirm(CConfirmOrder* confirm,CTradeClient* client);
	void confirm_to_order_struct(CConfirmOrder* confirm,order_struct* o_struct);
protected:
private:
	COrderDispatcher* order_dispatcher;						//< 账号派发器，当有空闲的客户端存在时，从它这里取账号。
	vector<CClientMacControler*> _client_mac_controlers;	//< 游戏集合列表
};

