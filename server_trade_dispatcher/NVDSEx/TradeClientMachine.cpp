#include "stdafx.h"
#include "tradeClientMachine.h"
#include "util/DateTime.h"
#include <ctime>
#include "TradeCodeDef.h"
#include "TradeOrder.h"
#include "../common/AutoLock.h"
#include "util/Logger.h"
#include "../common/DebugMessage.h"
#include "util/StringEx.h"
#include "../common/NetBase.h"
#include <stdexcept>
using std::runtime_error;
#include "TradeNetworkInterface.h"
#include "ObjectFactory.h"
#include "OrderDispatcher.h"
#include "TradeMacDispatcher.h"
#include "DBM.h"
#include "ConfirmOrder.h"

CTradeClient::CTradeClient( int game_type_id, 
									   const string& machine_name, 
									   int machine_id, 
									   const string& ip, 
									   int max_order_count,
									   CTradeNetworkInterface* network)
:_processed_success_count(0),
_state(EClientState_connected_busy),
_processed_count(0),
_seconds_per_success_order(-1),
_request_acc(false),
_mac_burden(-1)
{
	_game_type_id = game_type_id;
	_machine_name = machine_name;
	_machine_id = machine_id;
	_ip = ip;
	_max_order_count = max_order_count;
	_network = network;
	_start_time = new DateTime();
	_order_list_lock = new CLock_CS();
	_confirm_list_lock = new CLock_CS();
	_network_protect_lock = new CLock_CS();
}

string CTradeClient::dump() const
{
	string dump_info;
	dump_info = "\nObject Type = TradeClientMachine";
	dump_info += "\n_game_type_id = ";
	dump_info += String::fromLong(_game_type_id);
	dump_info += "\n_state = ";
	dump_info += String::fromLong((long)_state);
	dump_info += "\n_ip = ";
	dump_info += _ip;
	dump_info += "\n_machine_name = ";
	dump_info += _machine_name;
	dump_info += "\n_game_type_id = ";
	dump_info += String::fromLong(_machine_id);
	dump_info += "\n_start_time = ";
	dump_info += _start_time->toString();
	dump_info += "\n_max_order_count = ";
	dump_info += String::fromLong(_max_order_count);
	dump_info += "\n_request_acc = ";
	dump_info += String::fromBoolean(_request_acc);
	dump_info += "\n_mac_burden = ";
	dump_info += String::fromLong(_mac_burden);
	dump_info += "\n_processed_count = ";
	dump_info += String::fromLong(_processed_count);
	dump_info += "\n_processed_success_count = ";
	dump_info += String::fromLong(_processed_success_count);
	dump_info += "\n_seconds_per_success_order = ";
	dump_info += String::fromLong(_seconds_per_success_order);
	dump_info += "\n当前正在处理的Order个数:";
	dump_info += String::fromLong(_processing_orders.size());
	dump_info += "\n_processing_order:";
	long i=1;
	{
		CAutoLock lock(*_order_list_lock);
		for (list<CTradeOrder*>::const_iterator it = _processing_orders.begin(); it != _processing_orders.end(); ++it,++i){
			dump_info += "\n*******************#orders content $";
			dump_info += String::fromLong(i);
			dump_info += "*******************";
			dump_info += (*it)->dump();
		}
	}
	return dump_info;
}

CTradeClient::~CTradeClient()
{
	DebugMessageA("%s",__FUNCTION__);
	SAFE_DELETE(_start_time);
	SAFE_DELETE(_order_list_lock);
	SAFE_DELETE(_confirm_list_lock);
	SAFE_DELETE(_network_protect_lock);
}

void CTradeClient::clearConfirmOrder()
{
	//@FIXME:在这里添加清理该客户端正在处理的订单
	CAutoLock lock(*_network_protect_lock);
	if(_network){
		_network->PM_CancelAllConfirms();
	}
	return;
}

void CTradeClient::clearOrders()
{
	//@FIXME:在这里添加清理该客户端正在处理的订单
	CAutoLock lock(*_network_protect_lock);
	if(_network){
		_network->PM_CancelAllOrders();
	}
	return;
}

void CTradeClient::force_clear_orders_and_confirms()
{
	KillSession();
}

void CTradeClient::removeConfirm( CConfirmOrder* confirm )
{
	CAutoLock lock(*_confirm_list_lock);
	_processing_confirms.remove(confirm);
}

void CTradeClient::removeOrderFromProcessingList( CTradeOrder* order )
{
	CAutoLock lock(*_order_list_lock);
	_processing_orders.remove(order);
}

void CTradeClient::unexpected_order_state_on_disconnect( CTradeOrder* order )
{
	//注意:在执行unexpected_order_state_on_disconnect前order->TradeClient = NULL;
	string strRoleType;
	if (order->_tradeRole == CLIENT_TYPE_BUYER)
	{
		strRoleType = "buyer";
	}else{
		strRoleType = "seller";
	}
	
	char logs[1024] = {0};
	sprintf_s(logs,sizeof(logs),"订单%s-%s交易因客户端%d断线意外失败,错误代码:%d",
		order->_order_id.c_str(),strRoleType.c_str(),getMachineId(),TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW);
	order->log(logs);

	order->setTradeResult(TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW);
	order->setTradeTime(DateTime());
	//将两保存到数据库
	CTradeOrder* order_buddy = order->getBuddyOrder();
	if (order->_tradeRole == CLIENT_TYPE_SELLER)
		afkGetDbObject()->saveFatalFailedTradeRecord(order,order_buddy);
	else
		afkGetDbObject()->saveFatalFailedTradeRecord(order_buddy,order);

	order->setCurState(EOS_TRADE_COMPLETE);	//跳到标准流程进行处理。包括保存记录。
}

void CTradeClient::recyle_order_on_disconnect( CTradeOrder* order )
{
	string strRoleType;
	if (order->_tradeRole == CLIENT_TYPE_BUYER){
		strRoleType = "buyer";
	}else{
		strRoleType = "seller";
	}
	char logs[1024] = {0};
	sprintf_s(logs,sizeof(logs),"订单:%s-%s交易因客户端%d断线失败，错误代码:%d。",
		order->_order_id.c_str(),strRoleType.c_str(),getMachineId(),TRADE_FAILED_REASON_I_AM_DISCONNECTED_NOT_TRADE);
	order->log(logs);

	order->setTradeResult(TRADE_FAILED_REASON_I_AM_DISCONNECTED_NOT_TRADE);
	order->setTradeTime(DateTime());
	order->setCurState(EOS_TRADE_COMPLETE);
}


void CTradeClient::ProgressConfirmStateChanged( int dispatche_id,EClientConfirmProgress progress,short client_result_code )
{
	if (getMacState() == EClientState_disconnected)
	{
		return ;
	}
	CConfirmOrder* confirm = getConfirmById(dispatche_id);
	if (!confirm)
	{
		DebugMessageA("无法找到审核ID：%d对应的订单",dispatche_id);
		if (progress == ECCP_order_received)
			PM_ConfirmHasGetOrder_X(dispatche_id,E_ERRORCODE_SERVER_REMOVED);
		else
			PM_ConfirmStateChangedAckMsg(dispatche_id,E_ERRORCODE_SERVER_REMOVED);
		return;
	}
	confirm->setClientProgress(progress);
	char logs[1024] = {0};
	sprintf_s(logs,sizeof(logs),"订单:%s-审核当前进度%s",confirm->getOrderId().c_str(),CTradeClient::EConfrimProgressClientToString(progress).c_str());
	confirm->log(logs);
	switch(progress){
		case ECCP_standby:
		case ECCP_order_received:
			PM_ConfirmHasGetOrder_X(dispatche_id,E_ERRORCODE_SUCCESS);
			break;
		case ECCP_game_lunched:
		case ECCP_in_game_role:
			PM_ConfirmStateChangedAckMsg(dispatche_id,E_ERRORCODE_SUCCESS);
			break;
		case ECCP_end:
			confirm->setResultId(client_result_code);
			confirm->setCompleteTime(DateTime());
			confirm->setCurState(ECS_COMPLETE);
			PM_ConfirmStateChangedAckMsg(dispatche_id,E_ERRORCODE_SUCCESS);
			break;
		default:
			break;
	}
	return ;
}

void CTradeClient::ProgressTradeStateChanged( int dispatche_id,EClientOrderProgress progress,short client_result_code /*= 0*/ ){
	//first of all,get order in the processing list;
	if (getMacState() == EClientState_disconnected)
	{
		return;
	}

	CTradeOrder* order = getProcessingOrderById(dispatche_id);
	if (!order){
		DebugMessageA("无法找到订单ID：%d对应的订单",dispatche_id);
		if (progress == EOP_order_received)
		{
			PM_StateChangedAck_HasGetOrder_X(dispatche_id,E_ERRORCODE_SERVER_REMOVED);
		}else{
			PM_StateChangedAckMsg(dispatche_id,E_ERRORCODE_SERVER_REMOVED);
		}
		return;
	}

	order->setProgressClient(progress);

	string strRoleType;
	if (order->_tradeRole == CLIENT_TYPE_BUYER){
		strRoleType = "buyer";
	}else{
		strRoleType = "seller";
	}

	char logs[1024] = {0};
	if (client_result_code == MAC_PROCESSING_SUCCESS){
		sprintf_s(logs,sizeof(logs),"订单:%s-%s当前进度%s。",
			order->_order_id.c_str(),strRoleType.c_str(),CTradeClient::EOrderProgressClientToString(progress).c_str());
	}else{
		sprintf_s(logs,sizeof(logs),"订单:%s-%s交易因错误被中断，错误代码:%d。",order->_order_id.c_str(),strRoleType.c_str(),client_result_code);
	}
	order->log(logs);

	//snd deal with progress.
	switch(progress){
		case EOP_standby:
		case EOP_order_received:
			PM_StateChangedAck_HasGetOrder_X(dispatche_id,E_ERRORCODE_SUCCESS);
			break;
		case EOP_game_lunched:
		case EOP_in_game_role:
		case EOP_at_trade_stage:
		case EOP_trading:
			PM_StateChangedAckMsg(dispatche_id,E_ERRORCODE_SUCCESS);
			break;
		case EOP_ready_to_trade:
			{
				if (order->getProgressClient() == EOP_ready_to_trade)
				{
					order->setCurState(EOS_READY_TO_TRADE);
				}
				PM_StateChangedAckMsg(dispatche_id,E_ERRORCODE_SUCCESS);//可能需要放到CTradeOrderControler::ECbk_OrderStateChanged中
				break;
			}
		case EOP_trade_end:
			{
				order->setTradeResult(client_result_code);
				order->setTradeTime(DateTime());
				order->setCurState(EOS_TRADE_COMPLETE);
				PM_StateChangedAckMsg(dispatche_id,E_ERRORCODE_SUCCESS);//可能需要放到CTradeOrderControler::ECbk_OrderStateChanged中
			}
			break;
		default:
			break;
	}
	return ;
}

CConfirmOrder* CTradeClient::getConfirmById( int dispatchId )
{
	CAutoLock locker(*_confirm_list_lock);
	for (list<CConfirmOrder*>::const_iterator it = _processing_confirms.begin(); it != _processing_confirms.end(); ++it)
	{
		if ((*it)->getDispatchId() == dispatchId)
		{
			return (*it);
		}
	}
	return NULL;
}

CTradeOrder* CTradeClient::getProcessingOrderById( int dispatchId )
{
	CAutoLock lock(*_order_list_lock);

	for (list<CTradeOrder*>::const_iterator it = _processing_orders.begin(); it != _processing_orders.end(); ++it){
		if ((*it)->getDispatchId() == dispatchId){
			return (*it);
		}
	}

	return NULL;
}

string CTradeClient::EConfrimProgressClientToString( EClientConfirmProgress progress )
{
	/*
	ECCP_standby,					//< 待命，准备接收账号密码
	ECCP_order_received,			//< 客户端收到了账号，密码等信息
	ECCP_game_lunched,				//< 客户端游戏已经开启
	ECCP_in_game_role,				//< 客户端进入了游戏角色
	ECCP_end,						//< 确认完成
	*/
	string ret;
	switch(progress){
		case ECCP_standby:
			ret = "待命，准备接收账号密码";
			break;
		case ECCP_order_received:
			ret = "客户端收到了账号，密码等信息";
			break;
		case ECCP_game_lunched:
			ret = "客户端游戏已经启动";
			break;
		case ECCP_in_game_role:
			ret = "客户端进入了游戏角色";
			break;
		case ECCP_end:
			ret = "审核完毕";
			break;
		default:
			break;
	}
	return ret;
}

string CTradeClient::EOrderProgressClientToString( EClientOrderProgress eopc )
{
	string ret;
	switch(eopc){
		case EOP_standby:
			ret = "待命，准备接收账号密码";
			break;
		case EOP_order_received:
			ret = "客户端收到了账号，密码等信息";
			break;
		case EOP_game_lunched:
			ret = "客户端游戏已经启动";
			break;
		case EOP_in_game_role:
			ret = "客户端进入了游戏角色";
			break;
		case EOP_at_trade_stage:
			ret = "角色进入了指定场景";
			break;
		case EOP_ready_to_trade:
			ret = "客户端已经准备好交易";
			break;
		case EOP_trading:
			ret = "正在交易";
			break;
		case EOP_trade_end:
			ret = "交易完成";
			break;
		default:
			break;
	}
	return ret;
}

void CTradeClient::addOrderToProcessingList( CTradeOrder* order )
{
	CAutoLock lock(*_order_list_lock);
	_processing_orders.push_back(order); 
}

void CTradeClient::addConfirm( CConfirmOrder* confirm_order )
{
	CAutoLock lock(*_confirm_list_lock);
	_processing_confirms.push_back(confirm_order);
}
//OnDisconnect()仅在连接断开时被调用
void CTradeClient::OnDisconnect()
{
	DebugMessageA("%s",__FUNCTION__);
	{
		CAutoLock lock(*_order_list_lock);
		for (list<CTradeOrder*>::const_iterator it = _processing_orders.begin(); it != _processing_orders.end(); ++it)
		{
			CTradeOrder* order = *it;
			order->setClient(NULL);
			if (order->getCurState() == EOS_READY_TO_TRADE)
			{
				unexpected_order_state_on_disconnect(order);
				//ProgressStateChanged(order->getDispatchId(),EOP_trade_end,TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW);
			}else if (order->getCurState() == EOS_TRADE_COMPLETE){
				unexpected_order_state_on_disconnect(order);
				//ProgressStateChanged(order->getDispatchId(),EOP_trade_end,TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW);
			}else{
				recyle_order_on_disconnect(order);
				//ProgressStateChanged(order->getDispatchId(),EOP_trade_end,TRADE_FAILED_REASON_I_AM_DISCONNECTED_NOT_TRADE);
			}
		}
		_processing_orders.clear();
	}


	{
		CAutoLock lock2(*_confirm_list_lock);
		for (list<CConfirmOrder*>::const_iterator it = _processing_confirms.begin(); it!=_processing_confirms.end(); ++it)
		{
			CConfirmOrder* confirm = *it;
			confirm->setClient(NULL);
			if (confirm->getResultId() == 0){
				confirm->setCurState(ECS_GETFROM_DB);
			}else{
				confirm->setCurState(ECS_COMPLETE);
			}
		}
		_processing_confirms.clear();
	}

}

BOOL CTradeClient::PM_AssignOrder( order_struct* order )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		return _network->PM_AssignOrder(order);
	}
	return FALSE;
}

BOOL CTradeClient::PM_AssignConfirm( order_struct* confirm_order )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		return _network->PM_AssignConfirm(confirm_order);
	}
	return FALSE;
}

void CTradeClient::PM_AssignOrderFailed( EDispatchError errCode )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		return _network->PM_AssignOrderFailed(errCode);
	}
}


void CTradeClient::KillSession()
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		return _network->KillSession();
	}
}

void CTradeClient::PM_StateChangedAck_HasGetOrder_X( int order_dispatch_id,int error_code )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		_network->PM_StateChangedAck_HasGetOrder_X(order_dispatch_id,error_code);
	}
}

void CTradeClient::PM_StateChangedAckMsg( int order_dispatch_id,int error_code )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		_network->PM_StateChangedAckMsg(order_dispatch_id,error_code);
	}
}

void CTradeClient::PM_MyBuddyFailed( int order_dispatch_id,int error_code )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		_network->PM_MyBuddyFailed(order_dispatch_id,error_code);
	}
}

void CTradeClient::PM_ConfirmHasGetOrder_X( int order_dispatch_id,int error_code )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		_network->PM_ConfirmHasGetOrder_X(order_dispatch_id,error_code);
	}
}

void CTradeClient::PM_ConfirmStateChangedAckMsg( int order_dispatch_id,int error_code )
{
	CAutoLock lock(*_network_protect_lock);
	if (_network)
	{
		_network->PM_ConfirmStateChangedAckMsg(order_dispatch_id,error_code);
	}
}

CClientMacControler::CClientMacControler( int game_type_id,const string& game_type_describe,bool is_muti_client_game )
:_game_type_id(game_type_id),
_game_type_describe(game_type_describe),
_is_muti_client_game(is_muti_client_game),
_exit_flag(false)
{
	_mac_list_locker = new CLock_CS();
}

CClientMacControler::~CClientMacControler()
{
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it){
		delete (*it);
	}
	_client_machine_list.clear();

	if (_mac_list_locker)
	{
		delete _mac_list_locker;
		_mac_list_locker = NULL;
	}
}


bool CClientMacControler::addMac( CTradeClient* mac )
{
	CAutoLock locker(*_mac_list_locker);
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it){
		if ((*it)->getMachineId() == mac->getMachineId()){
			return false;
		}
	}
	_client_machine_list.push_back(mac);
	return true;
}

string CClientMacControler::dump() const
{
	string dump_info = "\nObject Type:CMacsControlerCommon\n game_type_id = ";
	dump_info += String::fromLong(_game_type_id);
	dump_info += "\ngame_type_describe = ";
	dump_info += _game_type_describe;
	dump_info += "\nis_enable = ";
	dump_info += String::fromBoolean(_exit_flag);
	dump_info += "\nis_muti_client_game = ";
	dump_info += String::fromBoolean(_is_muti_client_game);
	dump_info += "\n##count of current connected client machines = ";
	dump_info += String::fromLong(_client_machine_list.size());
	int i = 0;
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it,++i)
	{
		dump_info += "####################### machine $";
		dump_info += String::fromLong(i);
		dump_info += " #######################";
		dump_info += (*it)->dump();
	}
	return dump_info;
}

void CClientMacControler::force_exit()
{
	ASSERT(_exit_flag);
	CAutoLock lock(*_mac_list_locker);
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it)
	{
		(*it)->force_clear_orders_and_confirms();
	}
}

void CClientMacControler::exit()
{
	ASSERT(!_exit_flag);

	_exit_flag = true;
	/**
	 * @FIXME:在这里添加代码，处理“断开连接”，“发送退出消息”，“清理机器orders”等。
	 */
	{
		CAutoLock lock(*_mac_list_locker);
		for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it)
		{
			(*it)->clearOrders();
			(*it)->clearConfirmOrder();
		}
	}

	return;
}

int CClientMacControler::getCurrentProcessOrderCount() const
{
	CAutoLock lock(*_mac_list_locker);
	int remain = 0;
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end();++it)
	{
		remain += (*it)->getCurOrderCount();
	}
	return remain;
}

CTradeClient* CClientMacControler::getMac( int c_id, const string& name )
{
	CAutoLock lock(*_mac_list_locker);
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it)
	{
		if ((*it)->getMachineId() == c_id)
		{
			return *it;
		}
	}
	return NULL;
}

int CClientMacControler::getCurrentProcessConfirmCount() const
{
	CAutoLock lock(*_mac_list_locker);
	int remain = 0;
	for (list<CTradeClient*>::const_iterator it = _client_machine_list.begin(); it != _client_machine_list.end(); ++it)
	{
		remain += (*it)->getCurConfirmCount();
	}
	return remain;
}
