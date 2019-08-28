#include "stdafx.h"
#include "tradeorder.h"
#include "util/DateTime.h"
#include "../common/AutoLock.h"
#include "../common/NetBase.h"
#include "util/StringEx.h"
#include "util/Logger.h"
#include "Settings.h"
#include "DBM.h"
#include "../common/DebugMessage.h"
#include "OrderRecorder.h"
#include "tradeClientMachine.h"
#include "util/Path.h"
#include <map>
using std::map;

int i_order_del_counter = 0;
CLock_CS cs_globa_dispatch_id;
int getNextGlobalOrderId(){
	static int global_order_dispatche_id = 0;
	CAutoLock lock(cs_globa_dispatch_id);
	return global_order_dispatche_id++;
}

CTradeOrder::CTradeOrder()
:_order_id("bad_id")
,_game_type(-1)
,_client(NULL)
,_trade_result(0)
,_cur_state(EOS_GETFROM_DB)
,_emergency_level(EEL_NORMAL)
,_retry_times(3)
,_type_trade_is_money(true)
{
	_get_time = new DateTime(1,1,2000,0,0,0);
	_dispatched_time = new DateTime(1,1,2000,0,0,0);
	_trade_time = new DateTime(1,1,2000,0,0,0);
	_recorder = NULL;
	_dispatch_id = getNextGlobalOrderId();
	_isRemove = FALSE;
}

CTradeOrder::~CTradeOrder()
{
	SAFE_DELETE(_get_time);
	SAFE_DELETE(_dispatched_time);
	SAFE_DELETE(_trade_time);
	SAFE_DELETE(_recorder);
}

string CTradeOrder::dump() const
{
	string dump_info = "\nObjectType = CTradeOrder";
	dump_info += "\n_game_type = ";
	dump_info += String::fromLong(_game_type);
	dump_info += "\n_order_id = ";
	dump_info += _order_id;
	dump_info += "\n_client id = ";
	dump_info += String::fromLong(_client->getMachineId());
	dump_info += "\n_world = ";
	dump_info += _world;
	dump_info += "\n_trade_place = ";
	dump_info += _trade_place;
	dump_info += "\n_acc = ";
	dump_info += _acc;
	dump_info += "\n_pswd = ";
	dump_info += _pswd;
	dump_info += "\n_role_name = ";
	dump_info += _role_name;
	dump_info += "\n_acc_buyer = ";
	dump_info += "\n_emergency_level = ";
	dump_info += CTradeOrder::EEmgcLevelToString(_emergency_level);
	dump_info += "\n_trade_result = ";
	dump_info += String::fromLong(_trade_result);
	dump_info += "\n_get_time = ";
	dump_info += _get_time->toString();
	dump_info += "\n_dispatched_time = ";
	dump_info += _dispatched_time->toString();
	dump_info += "\n_trade_time = ";
	dump_info += _trade_time->toString();
	dump_info += "\n_cur_state = ";
	dump_info += CTradeOrder::EOrderStateToString(_cur_state);
	dump_info += "\n_retry_time = ";
	dump_info += String::fromLong(_retry_times);
	return dump_info;
}

string CTradeOrder::EOrderStateToString( EOrderState stat )
{
	string str;
	switch (stat)
	{
	case EOS_GETFROM_DB:
		str = "从数据库中取出，初始状态";
		break;
	case EOS_SEND_TO_DISPATCHER:
		str = "被取出给派发器";
		break;
	case EOS_DISPATCHED:
		str = "已经被派发给交易客户端";
		break;
	case EOS_READY_TO_TRADE:
		str = "两边的客户端都已经开启已准备好交易";
		break;
	case EOS_TRADE_COMPLETE:
		str = "交易已经完成";
		break;
	default:
		str = "未定义的进度状态！";
		break;
	}
	return str;
}

string CTradeOrder::EEmgcLevelToString( EEmgcLevel elv )
{
	string str;
	switch(elv){
		case EEL_NORMAL:
			str = "普通";
			break;
		case EEL_FAST:
			str = "加急";
			break;
		case EEL_FASTER:
			str = "十万火急";
			break;
		case EEL_FASTEST:
			str = "世界末日";
			break;
		default:
			str = "不认识的紧急度";
			break;
		};
	return str;
}

void CTradeOrder::log( const string& val )
{
	DateTime dtNow;
	string strToLog = dtNow.toString() + " " + val + "\r\n";;
	
	_log += strToLog;
	if (_is_log_to_text){
		_recorder->Log(strToLog); //< 记录订单发生的事件
	}
}

void CTradeOrder::setCurState( EOrderState val )
{
	if (_cur_state != val)
	{
		InterlockedExchange((long*)&_cur_state,val);
		_event_order_state_changed(this,val);
	}
}

void CTradeOrder::initlog()
{
	ASSERT(_game_type != -1);
	if (!_recorder)
	{
		string logFileName = CSettings::getInstance().getDefOrderLogPath(_game_type);
		logFileName += "\\";
		logFileName += _order_id;
		_recorder = new COrderRecorder(logFileName);
	}
}

CTradeOrderControler::~CTradeOrderControler()
{
	SAFE_DELETE(_list_process_locker);
}

CTradeOrderControler::CTradeOrderControler(int game_type_id,const string& game_type_describe,dispatch_stage dispatchstage)
{
	_exit_flag = false;
	_isEnable = TRUE;
	_game_type_id = game_type_id;
	_game_type_describe = game_type_describe;
	_stage = dispatchstage;
	_list_process_locker = new CLock_CS();
}

bool CTradeOrderControler::addOrder( CTradeOrder* order )
{
	CAutoLock locker(*_list_process_locker);
 	for (list<CTradeOrder*>::const_iterator it = _trade_order_list.begin(); it!=_trade_order_list.end();++it){
 		if (order->_order_id == (*it)->_order_id){
 			return false;
 		}
 	}
	order->_event_order_state_changed += boost::bind(&CTradeOrderControler::ECbk_OrderStateChanged,this,_1,_2);
	_trade_order_list.push_back(order);
	return true;
}

bool CTradeOrderControler::addOrder( list<CTradeOrder*> order_list )
{
	//@FIXME:在这里添加检查重复元素的代码
	CAutoLock locker(*_list_process_locker);
	for (list<CTradeOrder*>::iterator it = order_list.begin(); it != order_list.end(); ++it){
		(*it)->_event_order_state_changed += boost::bind(&CTradeOrderControler::ECbk_OrderStateChanged,this,_1,_2);
		_trade_order_list.push_back(*it);
	}
	return true;
}

void CTradeOrderControler::delOrder( CTradeOrder* order)
{
	CTradeOrder* order_buddy = order->getBuddyOrder();

	//处理交易双方一个结果是成功，另一边不成功的情况。将他们保存到意外列表中。
	if (order_buddy && order_buddy->ShouldBeRemoved())
	{
		//LOG_INFO("测试记录删除订单%s步骤1one:%d,another:%d",order->_order_id.c_str(),order->getTradeResult(),order_buddy->getTradeResult());
		if (order->getTradeResult() == MAC_PROCESSING_SUCCESS && order_buddy->getTradeResult() != MAC_PROCESSING_SUCCESS){
			//LOG_INFO("测试记录删除订单%s步骤2",order->_order_id.c_str());
			if (order->_tradeRole == CLIENT_TYPE_SELLER)
				afkGetDbObject()->saveFatalFailedTradeRecord(order,order_buddy);
			else
				afkGetDbObject()->saveFatalFailedTradeRecord(order_buddy,order);
		}

		if (order_buddy->getTradeResult() == MAC_PROCESSING_SUCCESS && order->getTradeResult() != MAC_PROCESSING_SUCCESS){
			//LOG_INFO("测试记录删除订单%s步骤3",order->_order_id.c_str());
			if (order->_tradeRole == CLIENT_TYPE_SELLER)
				afkGetDbObject()->saveFatalFailedTradeRecord(order,order_buddy);
			else
				afkGetDbObject()->saveFatalFailedTradeRecord(order_buddy,order);
		}

		if (order->_tradeRole == CLIENT_TYPE_SELLER)
		{
			saveOrderLog(order,order->getBuddyOrder());
			_fun_new_log(order->_order_id,order->getTradeTime().toString(),order->getTradeResult(),order_buddy->getTradeResult());
		}else{
			saveOrderLog(order->getBuddyOrder(),order);
			_fun_new_log(order->_order_id,order->getTradeTime().toString(),order_buddy->getTradeResult(),order->getTradeResult());
		}
	}
	
	{
		CAutoLock locker(*_list_process_locker);
		_trade_order_list.remove(order);
		order->Remove();
		if (order_buddy){
			if (order_buddy->ShouldBeRemoved())
			{
//				_trade_order_list.remove(order_buddy);
//				DebugMessageA("func %s delete order with id :%s",__FUNCTION__,order->_order_id.c_str());
				delete order_buddy;
				delete order;
				i_order_del_counter += 2;
			}
		}
	}
}

CTradeOrder* CTradeOrderControler::GetNextOrder(){

	if (!_isEnable)
	{
		DebugMessageA("Trade order controler 已经关闭，拒绝要求订单的请求！");
		return NULL;
	}

	if (_exit_flag)
	{
		return NULL;
	}

	bool reloaded = false;

	if (_trade_order_list.empty()){//如果整个为空,则尝试导入
		list<CTradeOrder*> oli1 = loadOrders(CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(_game_type_id));
//		DebugMessageA("end if loadOrders");
		if (oli1.empty()){
			return NULL;
		}
		reloaded = true;
	}
	
	//现在订单列表肯定不为空了，查找未处理的订单，即状态为EOS_GETFROM_DB的订单
	{
		CAutoLock locker(*_list_process_locker);
		for (list<CTradeOrder*>::iterator it = _trade_order_list.begin(); it!=_trade_order_list.end();++it){
			if ((*it)->getCurState() == EOS_GETFROM_DB){
				(*it)->setCurState(EOS_SEND_TO_DISPATCHER);//设置order为已取出状态
				return *it;
			}else{
				if ((*it)->getBuddyOrder()->getCurState() == EOS_GETFROM_DB)//不是初始状态也没关系，查看它的伙伴订单是不是处于初始状态。
				{
					(*it)->getBuddyOrder()->setCurState(EOS_SEND_TO_DISPATCHER);
					return (*it)->getBuddyOrder();
				}
			}
		}
	}

	//没有找到未处理的订单，如果已经尝试过重新导入订单【即刚开始列表为空的情况】
	//已经载入了账号，却找不到可用的账号，标记一下错误返回false;
	if (reloaded){
		LOG_ERROR("从服务器载入了账号，却找不到可用的账号1!");
		return false;
	}else{
		//没有重新载入订单[说明所有的订单都已经处于非初始状态]，那就尝试从数据库和远程接口取一次订单。
		list<CTradeOrder*> oli2 = loadOrders(CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(_game_type_id));
		if (oli2.empty())
		{
			return false;
		}else{
			//如果loadOrders不返回空列表则已经将新读取的订单写入到列表中了,重试一次.
			CAutoLock locker2(*_list_process_locker);
			for (list<CTradeOrder*>::iterator it = _trade_order_list.begin(); it != _trade_order_list.end(); ++it){
				if ((*it)->getCurState() == EOS_GETFROM_DB){
					(*it)->setCurState(EOS_SEND_TO_DISPATCHER);//设置order为已取出状态
					return *it;
				}else{
					if ((*it)->getBuddyOrder()->getCurState() == EOS_GETFROM_DB)//不是初始状态也没关系，查看它的伙伴订单是不是处于初始状态。
					{
						(*it)->getBuddyOrder()->setCurState(EOS_SEND_TO_DISPATCHER);
						return (*it)->getBuddyOrder();
					}
				}
			}

			LOG_ERROR("从服务器载入了账号，却找不到可用的账号1!");
			return NULL;
		}
	}

	return NULL;
}

bool CTradeOrderControler::CheckDataSourceIdle()
{
	if (!_is_in_source_empty)
		return false;
	else{
		if (_time_set_empty > GetTickCount()){
			InterlockedExchange(&_is_in_source_empty,0);//GetTickCount()49.7天重置一次。设为非空闲状态，向数据源服务器取订单以重新设置超时。
			return false;
		}

		if (_time_set_empty + _source_empty_timeout > GetTickCount()){
			return true;
		}else{
			InterlockedExchange(&_is_in_source_empty,0);
			return false;
		}
	}
}

void CTradeOrderControler::saveOrder( CTradeOrder* order,bool can_be_fetch )
{
	afkGetDbObject()->saveTradeOrder(order,can_be_fetch);
}

void CTradeOrderControler::exitRecyleOrder( CTradeOrder* order )
{
	afkGetDbObject()->exitRecyleTradeOrder(order);
}

void CTradeOrderControler::exit()
{
	if (_exit_flag)
	{
		return;
	}
	_exit_flag = true;

	map<string,CTradeOrder*> temp_to_del;
	for (list<CTradeOrder*>::const_iterator it=_trade_order_list.begin(); it!=_trade_order_list.end(); ++it){
		exitRecyleOrder(*it);
		temp_to_del[(*it)->_order_id] = (*it);
	}
	
	for (map<string,CTradeOrder*>::const_iterator it = temp_to_del.begin(); it!=temp_to_del.end();it++)
	{
		CTradeOrder* order_to_del = it->second;
		CTradeOrder* buddy_order_to_del = order_to_del->getBuddyOrder();
		delete order_to_del;
		delete buddy_order_to_del;
//		DebugMessageA("func %s delete order %s",__FUNCTION__,order_to_del->_order_id.c_str());
		i_order_del_counter += 2;
	}
	temp_to_del.clear();

	_trade_order_list.clear();
}

void CTradeOrderControler::ECbk_OrderStateChanged( CTradeOrder* order,EOrderState state )
{
	string strRoleType;
	if (order->_tradeRole == CLIENT_TYPE_BUYER){
		strRoleType = "buyer";
	}else{
		strRoleType = "seller";
	}

	switch(state)
	{
	case EOS_GETFROM_DB:
		{
			/*记录事件*/
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"订单:%s-%s 的当前状态被设置为初始状态.",order->_order_id.c_str(),strRoleType.c_str());
			order->log(logs);
			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->setDispatchedTime(DateTime(1,1,2000,0,0,0));
			saveOrder(order);
			break;
		}
	case EOS_SEND_TO_DISPATCHER:
		{
			/*记录事件*/
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"订单:%s-%s 被取出队列。",order->_order_id.c_str(),strRoleType.c_str());
			order->log(logs);

			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->setDispatchedTime(DateTime(1,1,2000,0,0,0));
			saveOrder(order);
			break;
		}
	case EOS_DISPATCHED:
		{
			/*记录事件*/
			if (!order->getClient())
			{
				LOG_ERROR("Order:%s处理EOS_DISPATCHED消息时order.getClient返回NULL",order->_order_id.c_str());
				break;
			}
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"订单:%s-%s 已经派发给客户机%d.",order->_order_id.c_str(),strRoleType.c_str(),order->getClient()->getMachineId());
			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->log(logs);
			saveOrder(order);
			break;
		}
	case EOS_READY_TO_TRADE:
		{
			/*记录事件*/
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"订单:%s-%s 的客户机 %d 已经准备好交易。",order->_order_id.c_str(),strRoleType.c_str(),order->getClient()->getMachineId());
			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->log(logs);
			saveOrder(order);
			break;
		}
	case EOS_TRADE_COMPLETE:
		{
			/*记录事件*/
			ASSERT(order->getTradeResult() != 0);

			int result = order->getTradeResult();
			if (result == MAC_PROCESSING_SUCCESS)//交易成功
			{
				char logs[1024] = {0};
				sprintf_s(logs,sizeof(logs),"订单:%s-%s处理完毕，处理结果为%d。",order->_order_id.c_str(),strRoleType.c_str(),order->getTradeResult());
				order->log(logs);
				saveOrder(order,true);

				//从客户端正在处理订单中删除自己
				CTradeClient* client = order->getClient();
				if (client){
					client->removeOrderFromProcessingList(order);
					//DebugMessageA("ECbk_OrderStateChanged，订单已经完成。mac_sell->removeOrderFromProcessingList");
				}

				delOrder(order);//从订单列表中删除自己

			}else{
				//交易失败了
				if (!orderCanRetry(order)){	//失败，而且不可重试
					char logs[1024] = {0};
					sprintf_s(logs,sizeof(logs),"订单:%s-%s处理完毕，处理结果为%d。",order->_order_id.c_str(),strRoleType.c_str(),order->getTradeResult());
					order->log(logs);

					saveOrder(order,true);

					CTradeOrder* buddy = order->getBuddyOrder();
					if (buddy == NULL)
					{
						LOG_ERROR("严重错误！order的buddy order为空");
					}else{
						if (buddy->getClient() == NULL){
							LOG_ERROR("order的buddy order的client为空,无法发送交易对象失败消息。");
						}else{
							buddy->getClient()->PM_MyBuddyFailed(buddy->getDispatchId(),result);
						}
					}

					//从客户端正在处理订单中删除自己
					CTradeClient* client = order->getClient();
					if (client){
						client->removeOrderFromProcessingList(order);
						//DebugMessageA("ECbk_OrderStateChanged，订单已经完成。mac_sell->removeOrderFromProcessingList");
					}
					//从订单列表中删除自己
					delOrder(order);

				}else{
					//从客户端正在处理订单中删除自己
					CTradeClient* client = order->getClient();
					if (client){//该客户端可能已经断线
						client->removeOrderFromProcessingList(order);
						//DebugMessageA("ECbk_OrderStateChanged，订单已经完成。mac_sell->removeOrderFromProcessingList");
					}

					//失败，但是可以重试
					if (0==order->getRetryTimes())//超过最大重试次数，删除订单。
					{
						order->setTradeResult(RETRY_TIME_END);
						char logs[1024] = {0};
						sprintf_s(logs,sizeof(logs),"订单:%s-%s失败超过最大次数，该订单被视为处理失败错误号为%d。",order->_order_id.c_str(),strRoleType.c_str(),RETRY_TIME_END);
						order->log(logs);
						CTradeOrder* buddy = order->getBuddyOrder();
						if (buddy == NULL)
						{
							LOG_ERROR("严重错误！order的buddy order为空");
						}else{
							if (buddy->getClient() == NULL){
								LOG_ERROR("buddyorder的client为空,无法发送交易对象失败消息。");
							}else{
								buddy->getClient()->PM_MyBuddyFailed(buddy->getDispatchId(),result);
							}
						}
						//删除自己
						saveOrder(order,true);
						delOrder(order);
					}else{	//可以重试，那么将它回收
						order->setRetryTimes(order->getRetryTimes()-1);
						order->resetOrderState();
						char logs[1024] = {0};
						sprintf_s(logs,sizeof(logs),"订单:%s-%s发生非致命错误，订单重试次数减一",order->_order_id.c_str(),strRoleType.c_str());
						order->log(logs);
						saveOrder(order);
					}
					break;
				}
			}
			break;
		}
	default:
		break;
	}
	//DebugMessageA("CTradeOrderControler::ECbk_OrderStateChanged!");
}

bool CTradeOrderControler::orderCanRetry( CTradeOrder* order )
{
	/*
	ECC_GAME_COLLAPSE = 0x100,			//客户端崩溃，具体哪种原因不晓得。注意交易过程不能崩溃，要是崩溃了就麻烦了。
	ECC_LOGIN_SERVER_ERROR = 0x101,		//服务器维护、繁忙、etc.
	TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL = 0x108,//服务器要求立即取消订单
	TRADE_FAILED_REASON_SERVER_COMMAND_DENY = 0x109,//服务器拒绝了交易请求
	TRADE_FAILED_REASON_BUDDY_DISCONNECTED = 0x10B,//服务器命令取消交易，因为交易对象断线
	TRADE_FAILED_REASON_I_AM_DISCONNECTED = 0x10C,//交易客户端意外断线。
	*/
	//这些错误原因都是可以重试的，但是像金钱不够，账号密码不对等错误肯定不用再重试。
	switch(order->getTradeResult()){
		case ECC_GAME_COLLAPSE:	
		case ECC_LOGIN_SERVER_ERROR:
		case TRADE_FAILED_REASON_BUDDY_DISCONNECTED:
		case TRADE_FAILED_REASON_I_AM_DISCONNECTED_NOT_TRADE:
//		case TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW:
		case TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL:
		case TRADE_FAILED_REASON_SERVER_COMMAND_DENY:
			return true;
			break;
		default:
			return false;
	}
	return false;
}

void CTradeOrderControler::saveOrderLog( CTradeOrder* seller,CTradeOrder* buyer )
{
	if (seller && buyer)
	{
		afkGetDbObject()->saveTradeOrderLog(seller,buyer);
	}
}

void CTradeOrderControler::tradeOrderLoaded( list<CTradeOrder*>& oli )
{
	if (oli.empty())
	{
		setDataSourceEmpty(CSettings::getInstance().getDataSouceQueryRetryTime(getGameTypeId()));//60秒内不再重试装载订单
	}
	addOrder(oli);
}
