#include "stdafx.h"
#include "TradeMacDispatcher.h"
#include "TradeOrder.h"
#include "OrderDispatcher.h"
#include "TradeCodeDef.h"
#include "../common/shareStruct.h"
#include "../common/DebugMessage.h"
#include "TradeNetworkInterface.h"
#include "ConfirmDispatcher.h"
#include "ConfirmOrder.h"
#include "ObjectFactory.h"

bool CTradeMacDispatcher::addMac( int game_type,CTradeClient* mac )
{
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end();++it){
		if ((*it)->getGameTypeId() == game_type){
			return (*it)->addMac(mac);
		}
	}
	return false;
}

void CTradeMacDispatcher::AddClientMacControler( CClientMacControler* controler )
{
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it)
	{
		if ((*it)->getGameTypeId() == controler->getGameTypeId())
		{
			return;
		}
	}
	_client_mac_controlers.push_back(controler);
}


int CTradeMacDispatcher::DealTradeRequest(CTradeClient* mac,int mac_burden )
{
	CClientMacControler* collection = NULL;
	
	int game_type = mac->getGameTypeId();

	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it){
		if ((*it)->getGameTypeId() == game_type){
			collection = *(it);
			break;
		}			
	}

	if (collection == NULL)
	{
		//找不到game_id对应的机群，不支持该类型的游戏
		return EDE_DISPATCH_ERROR_GAME_NOT_FOUND;
	}

	mac->setMacBurden(mac_burden);
	mac->setMacState(CTradeClient::EClientState_connected_idle);
	
	CTradeOrder *order = order_dispatcher->GetNextOrder(game_type);
	if(order){
	
		//ASSERT(mac->getMacState() == CTradeClientMachine::EMachineState_connected_busy);

		/*@FIXME:发送派发命令*/
		int iRet = dispatchOrder(order,mac);
		if (iRet != EDE_DISPATCH_SUCCESS)
		{
			//订单发送失败，将机器回滚回idle状态
			mac->setMacState(CTradeClient::EClientState_connected_idle);
			//将订单回滚为初始状态
			order->setCurState(EOS_GETFROM_DB);
		}

		return iRet;
	}else{	
		//没有可取的交易订单，尝试获取确认订单
		CConfirmOrder* confirmOrder = CObjectFactory::getInstance().GetConfirmDispatcher()->getNextConfirmOrder(game_type);
		if (confirmOrder)
		{
			int iret = dispatchConfirm(confirmOrder,mac);
			if (iret != EDE_DISPATCH_SUCCESS)
			{
				mac->setMacState(CTradeClient::EClientState_connected_idle);
				confirmOrder->setCurState(ECS_GETFROM_DB);
			}
			return iret;
		}else
			return EDE_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN;
	}
}

string CTradeMacDispatcher::dump() const
{
	string dump_info;
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin();it!=_client_mac_controlers.end();++it)
	{
		dump_info += (*it)->dump();
	}
	return dump_info;
}

CTradeMacDispatcher::CTradeMacDispatcher()
{
	_client_mac_controlers.clear();
}

CTradeMacDispatcher::~CTradeMacDispatcher()
{
	DebugMessageA("%s",__FUNCTION__);
	for (int i=0; i!=_client_mac_controlers.size();i++)
	{
		delete _client_mac_controlers[i];
	}
}

int CTradeMacDispatcher::dispatchOrder( CTradeOrder* order,CTradeClient* client)
{
	order_struct os;

	corder_to_order_struct(order,&os);
	
	if(!client->PM_AssignOrder(&os)){
		return EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR;
	}

	/*修改订单的当前状态，因为订单是NonCopyable的所以直接修改即可*/
	order->setClient(client);
	order->setDispatchedTime(DateTime());
	order->setCurState(EOS_DISPATCHED);

	/*将订单加入机器的正在处理列表*/
	client->addOrderToProcessingList(order);

	return EDE_DISPATCH_SUCCESS;
}

int CTradeMacDispatcher::dispatchConfirm( CConfirmOrder* confirm,CTradeClient* client )
{
	order_struct os;
	confirm_to_order_struct(confirm,&os);
	if (!client->PM_AssignConfirm(&os))
	{
		return EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR;
	}
	
	confirm->setClient(client);
	confirm->setDispatchTime(DateTime());
	confirm->setCurState(ECS_DISPATCHED);

	client->addConfirm(confirm);
	return EDE_DISPATCH_SUCCESS;
}

void CTradeMacDispatcher::confirm_to_order_struct( CConfirmOrder* confirm,order_struct* o_struct )
{
	o_struct->order_dispatch_id = confirm->getDispatchId();
	o_struct->is_trade_money = confirm->getTypeTradeIsMoney();
	o_struct->game_type = confirm->getGameTypeId();
	strcpy_s(o_struct->order_id,sizeof(o_struct->order_id),confirm->getOrderId().c_str());
	strcpy_s(o_struct->world,sizeof(o_struct->world),confirm->getWorld().c_str());
	strcpy_s(o_struct->trade_place,sizeof(o_struct->trade_place),"");
	o_struct->order_type = OrderTypeConfirm;
	o_struct->retry_times_remain = 3;
	o_struct->time_out_second = 60;
	memset(&(o_struct->trade_ware_detail),0,sizeof(o_struct->trade_ware_detail));
	o_struct->trade_money = confirm->getMoneyConfirm();
	string order_params = confirm->getParams();
	int poslm = order_params.find("&lm=");
	int posrp = order_params.find("&rp=");
	int poswp = order_params.find("&wp=");
	if (poslm == string::npos || posrp == string::npos || poswp == string::npos){
		strcpy_s(o_struct->ware_pswd,sizeof(o_struct->ware_pswd),"");
		strcpy_s(o_struct->role_pswd,sizeof(o_struct->role_pswd),"");
		o_struct->login_model = 0;
	}else{
		strcpy_s(o_struct->ware_pswd,sizeof(o_struct->ware_pswd), order_params.substr(poswp+4,order_params.length() - poswp - 4).c_str());
		strcpy_s(o_struct->role_pswd,sizeof(o_struct->role_pswd), order_params.substr(posrp+4,poswp-posrp-4).c_str());
		o_struct->login_model = atoi(order_params.substr(poslm+4,posrp-poslm-4).c_str());
	}
	strcpy_s(o_struct->acc,sizeof(o_struct->acc),confirm->getAccount().c_str());
	strcpy_s(o_struct->pswd,sizeof(o_struct->pswd),confirm->getPassword().c_str());
	strcpy_s(o_struct->role,sizeof(o_struct->role),confirm->getRole().c_str());
	memset(&(o_struct->buddy_role),0,sizeof(o_struct->buddy_role));

	o_struct->trade_role = CLIENT_TYPE_BUYER;
}
void CTradeMacDispatcher::corder_to_order_struct( IN CTradeOrder* order,OUT order_struct* o_struct )
{
	o_struct->order_dispatch_id = order->getDispatchId();
	o_struct->is_trade_money = order->_type_trade_is_money;
	o_struct->game_type = order->_game_type;
	strcpy_s(o_struct->order_id,sizeof(o_struct->order_id),order->_order_id.c_str());
	strcpy_s(o_struct->world,sizeof(o_struct->world),order->_world.c_str());
	strcpy_s(o_struct->trade_place,sizeof(o_struct->trade_place),order->_trade_place.c_str());
	o_struct->order_type = OrderTypeTrade;
	o_struct->retry_times_remain = order->getRetryTimes();
	o_struct->time_out_second = 60;
	o_struct->trade_ware_detail = order->_trade_good_detail;
	o_struct->trade_money = order->getTradeMoney();
	string order_params = order->getOrderParams();
	int poslm = order_params.find("&lm=");
	int posrp = order_params.find("&rp=");
	int poswp = order_params.find("&wp=");
	if (poslm == string::npos || posrp == string::npos || poswp == string::npos){
		strcpy_s(o_struct->ware_pswd,sizeof(o_struct->ware_pswd),"");
		strcpy_s(o_struct->role_pswd,sizeof(o_struct->role_pswd),"");
		o_struct->login_model = 0;
	}else{
		strcpy_s(o_struct->ware_pswd,sizeof(o_struct->ware_pswd), order_params.substr(poswp+4,order_params.length() - poswp - 4).c_str());
		strcpy_s(o_struct->role_pswd,sizeof(o_struct->role_pswd), order_params.substr(posrp+4,poswp-posrp-4).c_str());
		o_struct->login_model = atoi(order_params.substr(poslm+4,posrp-poslm-4).c_str());
	}
	strcpy_s(o_struct->acc,sizeof(o_struct->acc),order->_acc.c_str());
	strcpy_s(o_struct->pswd,sizeof(o_struct->pswd),order->_pswd.c_str());
	strcpy_s(o_struct->role,sizeof(o_struct->role),order->_role_name.c_str());
	strcpy_s(o_struct->buddy_role,sizeof(o_struct->buddy_role),order->_buddy_role_name.c_str());

	o_struct->trade_role = order->_tradeRole;

}

void CTradeMacDispatcher::exit()
{
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin();it!=_client_mac_controlers.end();++it){
		(*it)->exit();
	}
}

void CTradeMacDispatcher::force_exit()
{
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin();it!=_client_mac_controlers.end();++it)
	{
		(*it)->force_exit();
	}
}

int CTradeMacDispatcher::GetCurrentProcessConfirmCount() const
{
	int iRemian = 0;
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it)
	{
		iRemian += (*it)->getCurrentProcessConfirmCount();
	}
	return iRemian;
}


int CTradeMacDispatcher::getCurrentProcessOrderCount() const
{
	int iRemain = 0;
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it)
	{
		iRemain += (*it)->getCurrentProcessOrderCount();
	}
	return iRemain;
}

CTradeClient* CTradeMacDispatcher::getMac( int game_id, int c_id, const string& name )
{
	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it)
	{
		if ((*it)->getGameTypeId() == game_id)
		{
			return (*it)->getMac(c_id,name);
		}
	}
	return NULL;
}

// void CTradeMacDispatcher::DoOrderComplete( CTradeOrder* order )
// {
// 	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it)
// 	{
// 		if (order->_game_type == (*it)->getGameTypeId())
// 		{
// 			(*it)->DoOrderComplete(order);
// 		}
// 	}
// }
// 
// void CTradeMacDispatcher::DoOrderCancel( CTradeOrder* order ,EClientRoleType roleType)
// {
// 	for (vector<CClientMacControler*>::const_iterator it = _client_mac_controlers.begin(); it != _client_mac_controlers.end(); ++it)
// 	{
// 		if (order->_game_type == (*it)->getGameTypeId())
// 		{
// 			(*it)->DoOrderCanceled(order,roleType);
// 		}
// 	}
// }
