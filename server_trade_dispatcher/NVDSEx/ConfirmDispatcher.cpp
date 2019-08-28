#include "StdAfx.h"
#include "ConfirmDispatcher.h"
#include "ConfirmOrder.h"
#include "../common/NetBase.h"
#include "../common/AutoLock.h"
#include "DBM.h"
#include "Settings.h"
#include "util/Logger.h"
#include "tradeClientMachine.h"
extern int i_order_del_counter;
CConfirmDispatcher::CConfirmDispatcher(void)
{
	_cs_list_locker = new CLock_CS();
	_enable_flag = true;
	_exit_flag = false;
}

CConfirmDispatcher::~CConfirmDispatcher(void)
{
	_exit_flag = true;
	_enable_flag = false;
	SAFE_DELETE(_cs_list_locker);
}

void CConfirmDispatcher::LoadConfirm( int game_type )
{
	list<CConfirmOrder*> listRet;
	if (!_enable_flag)
	{
		return ;
	}

	if (_exit_flag)
	{
		return ;
	}
	afkGetDbObject()->GetNoneConfirmOrder(game_type,CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(game_type));
	return;
}

CConfirmOrder* CConfirmDispatcher::getNextConfirmOrder(int game_type)
{
	if (!_enable_flag || _exit_flag)
	{
		return NULL;
	}

	list<CConfirmOrder*>* plist = _confirm_orders[game_type];
	if (plist->empty()){
		LoadConfirm(game_type);
		return NULL;
	}

	for (list<CConfirmOrder*>::const_iterator it = (*plist).begin(); it != (*plist).end(); ++it)
	{
		if ((*it)->getCurState() == ECS_GETFROM_DB)
		{
			return *it;
		}
	}
	//不空但是没有未分配的
	LoadConfirm(game_type);
	return NULL;
}

void CConfirmDispatcher::initOrderList( const vector<int>& game_type_support )
{
	_enable_flag = true;
	_exit_flag = false;
	for (vector<int>::const_iterator it = game_type_support.begin(); it != game_type_support.end(); ++it){
		int game_type_id = *it;
		list<CConfirmOrder*>* plist = new list<CConfirmOrder*>;
		_confirm_orders[game_type_id] = plist;
	}
}

void CConfirmDispatcher::clear()
{
	for (map<int,list<CConfirmOrder*>*>::const_iterator it = _confirm_orders.begin(); it != _confirm_orders.end(); ++it){
		list<CConfirmOrder*>* plist = it->second;
		plist->clear();
		delete plist;
	}
	_confirm_orders.clear();
	SAFE_DELETE(_cs_list_locker);
}

void CConfirmDispatcher::ECbkConfirmStateChanged( CConfirmOrder* confirm,EConfirmState state )
{
	switch(state){
		case ECS_GETFROM_DB:
			{
				char logs[1024] = {0};
				sprintf_s(logs,sizeof(logs),"订单%s的当前状态被设置为初始状态。",confirm->getOrderId().c_str());
				confirm->log(logs);
				confirm->setCompleteTime(DateTime(1,1,2000,0,0,0));
				confirm->setDispatchTime(DateTime(1,1,2000,0,0,0));
				SaveConfirm(confirm);
				break;
			}
		case ECS_SEND_TO_DISPATCHER:
			{
				char logs[1024] = {0};
				sprintf_s(logs,sizeof(logs),"订单%s被取出队列。",confirm->getOrderId().c_str());
				confirm->log(logs);
				confirm->setCompleteTime(DateTime(1,1,2000,0,0,0));
				SaveConfirm(confirm);
				break;
			}
		case ECS_DISPATCHED:
			{
				if(!confirm->getClient()){
					LOG_ERROR("confirm:%s处理ECS_DISPATCHED消息时confirm.getclient返回NULL",confirm->getOrderId().c_str());
					break;
				}
				char logs[1024] = {0};
				sprintf_s(logs,sizeof(logs),"订单%s已经派发给客户机%d",confirm->getOrderId().c_str(),confirm->getClient()->getMachineId());
				confirm->setCompleteTime(DateTime(1,1,2000,0,0,0));
				confirm->log(logs);
				SaveConfirm(confirm);
				break;
			}
		case ECS_COMPLETE:
			{
				ASSERT(confirm->getResultId() != 0);
				int result = confirm->getResultId();
				if (result != MAC_PROCESSING_SUCCESS && ConfirmCanRetry(confirm))//失败但是可以重试
				{
					CTradeClient* client = confirm->getClient();
					if (client){
						client->removeConfirm(confirm);//从处理机中移除
					}
					confirm->setCurState(ECS_GETFROM_DB);//重设状态
				}else{
					char logs[1024] = {0};
					sprintf_s(logs,sizeof(logs),"订单%s处理完毕，处理结果为%d",confirm->getOrderId().c_str(),confirm->getResultId());
					confirm->log(logs);
					_fun_new_log(confirm->getOrderId(),confirm->getCompleteTime().toString(),confirm->getResultId(),-2);
					SaveConfirm(confirm,true);
					CTradeClient* client = confirm->getClient();
					if (client)
					{
						client->removeConfirm(confirm);
					}
					delConfirm(confirm);
				}
				break;
			}
		default:
			break;
	}
}

void CConfirmDispatcher::delConfirm( CConfirmOrder* confirm )
{
	CAutoLock locker(*_cs_list_locker);
	list<CConfirmOrder*>* plist = _confirm_orders[confirm->getGameTypeId()];
	plist->remove(confirm);
	SaveConfirmLog(confirm);
	delete confirm;
	i_order_del_counter ++;
}

void CConfirmDispatcher::SaveConfirm( CConfirmOrder* confirm,bool getstate/*=false*/ )
{
	afkGetDbObject()->saveConfirm(confirm,getstate);
}

void CConfirmDispatcher::SaveConfirmLog( CConfirmOrder* confirm )
{
	afkGetDbObject()->saveConfirmLog(confirm);
}

bool CConfirmDispatcher::ConfirmCanRetry( CConfirmOrder* confirm )
{
	int result_id = confirm->getResultId();
	bool result_can_retry = false;
	switch(result_id){
		case ECC_GAME_COLLAPSE:
		case ECC_LOGIN_SERVER_ERROR:
		case TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL:
			result_can_retry = true;
			break;
		default:
			break;
	}

	if (result_can_retry && confirm->getRetryTimes() >= 0 )
	{
		confirm->setRetryTimes(confirm->getRetryTimes()-1);
		return true;
	}else{
		return false;
	}
	
}

void CConfirmDispatcher::exit()
{
	if (_exit_flag)
	{
		return;
	}

	_exit_flag = true;
	_enable_flag = false;

	for (map<int,list<CConfirmOrder*>*>::const_iterator it = _confirm_orders.begin(); it != _confirm_orders.end(); ++it)
	{
		list<CConfirmOrder*>* plist = it->second;
		for (list<CConfirmOrder*>::const_iterator it = plist->begin(); it != plist->end(); ++it)
		{
			ExitRecyleConfirm(*it);
		}
		plist->clear();
	}
}

int CConfirmDispatcher::getCurConfirmCount()
{
	int ret = 0;
	for(map<int,list<CConfirmOrder*>*>::const_iterator it = _confirm_orders.begin(); it != _confirm_orders.end(); ++it){
		ret += it->second->size();
	}
	return ret;
}

void CConfirmDispatcher::disable()
{
	_enable_flag = false;
}

void CConfirmDispatcher::ExitRecyleConfirm( CConfirmOrder* confirm )
{
	afkGetDbObject()->exitRecyleConfirmOrder(confirm);
	SaveConfirmLog(confirm);
	delete confirm;
	i_order_del_counter ++;
}

void CConfirmDispatcher::preLoadConfirm()
{
	for (map<int,list<CConfirmOrder*>*>::const_iterator it = _confirm_orders.begin(); it != _confirm_orders.end(); ++it)
	{
		LoadConfirm(it->first);
	}
}

void CConfirmDispatcher::ECBK_ConfirmsLoaded( IDatabaseObject* sender,int game_type,list<CConfirmOrder*> confirmList )
{
	if (confirmList.size() == 0)
	{
		return;
	}else{
		CAutoLock locker(*_cs_list_locker);
		list<CConfirmOrder*>* plist = _confirm_orders[game_type];
		for (list<CConfirmOrder*>::const_iterator it = confirmList.begin(); it != confirmList.end(); ++it)
		{
			(*it)->_event_confirm_order_state_changed += boost::bind(&CConfirmDispatcher::ECbkConfirmStateChanged,this,_1,_2);
			plist->push_back(*it);
		}
	}
}
