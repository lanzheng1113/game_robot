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
		str = "�����ݿ���ȡ������ʼ״̬";
		break;
	case EOS_SEND_TO_DISPATCHER:
		str = "��ȡ�����ɷ���";
		break;
	case EOS_DISPATCHED:
		str = "�Ѿ����ɷ������׿ͻ���";
		break;
	case EOS_READY_TO_TRADE:
		str = "���ߵĿͻ��˶��Ѿ�������׼���ý���";
		break;
	case EOS_TRADE_COMPLETE:
		str = "�����Ѿ����";
		break;
	default:
		str = "δ����Ľ���״̬��";
		break;
	}
	return str;
}

string CTradeOrder::EEmgcLevelToString( EEmgcLevel elv )
{
	string str;
	switch(elv){
		case EEL_NORMAL:
			str = "��ͨ";
			break;
		case EEL_FAST:
			str = "�Ӽ�";
			break;
		case EEL_FASTER:
			str = "ʮ���";
			break;
		case EEL_FASTEST:
			str = "����ĩ��";
			break;
		default:
			str = "����ʶ�Ľ�����";
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
		_recorder->Log(strToLog); //< ��¼�����������¼�
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
	//@FIXME:��������Ӽ���ظ�Ԫ�صĴ���
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

	//������˫��һ������ǳɹ�����һ�߲��ɹ�������������Ǳ��浽�����б��С�
	if (order_buddy && order_buddy->ShouldBeRemoved())
	{
		//LOG_INFO("���Լ�¼ɾ������%s����1one:%d,another:%d",order->_order_id.c_str(),order->getTradeResult(),order_buddy->getTradeResult());
		if (order->getTradeResult() == MAC_PROCESSING_SUCCESS && order_buddy->getTradeResult() != MAC_PROCESSING_SUCCESS){
			//LOG_INFO("���Լ�¼ɾ������%s����2",order->_order_id.c_str());
			if (order->_tradeRole == CLIENT_TYPE_SELLER)
				afkGetDbObject()->saveFatalFailedTradeRecord(order,order_buddy);
			else
				afkGetDbObject()->saveFatalFailedTradeRecord(order_buddy,order);
		}

		if (order_buddy->getTradeResult() == MAC_PROCESSING_SUCCESS && order->getTradeResult() != MAC_PROCESSING_SUCCESS){
			//LOG_INFO("���Լ�¼ɾ������%s����3",order->_order_id.c_str());
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
		DebugMessageA("Trade order controler �Ѿ��رգ��ܾ�Ҫ�󶩵�������");
		return NULL;
	}

	if (_exit_flag)
	{
		return NULL;
	}

	bool reloaded = false;

	if (_trade_order_list.empty()){//�������Ϊ��,���Ե���
		list<CTradeOrder*> oli1 = loadOrders(CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(_game_type_id));
//		DebugMessageA("end if loadOrders");
		if (oli1.empty()){
			return NULL;
		}
		reloaded = true;
	}
	
	//���ڶ����б�϶���Ϊ���ˣ�����δ����Ķ�������״̬ΪEOS_GETFROM_DB�Ķ���
	{
		CAutoLock locker(*_list_process_locker);
		for (list<CTradeOrder*>::iterator it = _trade_order_list.begin(); it!=_trade_order_list.end();++it){
			if ((*it)->getCurState() == EOS_GETFROM_DB){
				(*it)->setCurState(EOS_SEND_TO_DISPATCHER);//����orderΪ��ȡ��״̬
				return *it;
			}else{
				if ((*it)->getBuddyOrder()->getCurState() == EOS_GETFROM_DB)//���ǳ�ʼ״̬Ҳû��ϵ���鿴���Ļ�鶩���ǲ��Ǵ��ڳ�ʼ״̬��
				{
					(*it)->getBuddyOrder()->setCurState(EOS_SEND_TO_DISPATCHER);
					return (*it)->getBuddyOrder();
				}
			}
		}
	}

	//û���ҵ�δ����Ķ���������Ѿ����Թ����µ��붩�������տ�ʼ�б�Ϊ�յ������
	//�Ѿ��������˺ţ�ȴ�Ҳ������õ��˺ţ����һ�´��󷵻�false;
	if (reloaded){
		LOG_ERROR("�ӷ������������˺ţ�ȴ�Ҳ������õ��˺�1!");
		return false;
	}else{
		//û���������붩��[˵�����еĶ������Ѿ����ڷǳ�ʼ״̬]���Ǿͳ��Դ����ݿ��Զ�̽ӿ�ȡһ�ζ�����
		list<CTradeOrder*> oli2 = loadOrders(CSettings::getInstance().getDefGetOrderCountFromDbPerBatch(_game_type_id));
		if (oli2.empty())
		{
			return false;
		}else{
			//���loadOrders�����ؿ��б����Ѿ����¶�ȡ�Ķ���д�뵽�б�����,����һ��.
			CAutoLock locker2(*_list_process_locker);
			for (list<CTradeOrder*>::iterator it = _trade_order_list.begin(); it != _trade_order_list.end(); ++it){
				if ((*it)->getCurState() == EOS_GETFROM_DB){
					(*it)->setCurState(EOS_SEND_TO_DISPATCHER);//����orderΪ��ȡ��״̬
					return *it;
				}else{
					if ((*it)->getBuddyOrder()->getCurState() == EOS_GETFROM_DB)//���ǳ�ʼ״̬Ҳû��ϵ���鿴���Ļ�鶩���ǲ��Ǵ��ڳ�ʼ״̬��
					{
						(*it)->getBuddyOrder()->setCurState(EOS_SEND_TO_DISPATCHER);
						return (*it)->getBuddyOrder();
					}
				}
			}

			LOG_ERROR("�ӷ������������˺ţ�ȴ�Ҳ������õ��˺�1!");
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
			InterlockedExchange(&_is_in_source_empty,0);//GetTickCount()49.7������һ�Ρ���Ϊ�ǿ���״̬��������Դ������ȡ�������������ó�ʱ��
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
			/*��¼�¼�*/
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"����:%s-%s �ĵ�ǰ״̬������Ϊ��ʼ״̬.",order->_order_id.c_str(),strRoleType.c_str());
			order->log(logs);
			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->setDispatchedTime(DateTime(1,1,2000,0,0,0));
			saveOrder(order);
			break;
		}
	case EOS_SEND_TO_DISPATCHER:
		{
			/*��¼�¼�*/
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"����:%s-%s ��ȡ�����С�",order->_order_id.c_str(),strRoleType.c_str());
			order->log(logs);

			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->setDispatchedTime(DateTime(1,1,2000,0,0,0));
			saveOrder(order);
			break;
		}
	case EOS_DISPATCHED:
		{
			/*��¼�¼�*/
			if (!order->getClient())
			{
				LOG_ERROR("Order:%s����EOS_DISPATCHED��Ϣʱorder.getClient����NULL",order->_order_id.c_str());
				break;
			}
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"����:%s-%s �Ѿ��ɷ����ͻ���%d.",order->_order_id.c_str(),strRoleType.c_str(),order->getClient()->getMachineId());
			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->log(logs);
			saveOrder(order);
			break;
		}
	case EOS_READY_TO_TRADE:
		{
			/*��¼�¼�*/
			char logs[1024] = {0};
			sprintf_s(logs,sizeof(logs),"����:%s-%s �Ŀͻ��� %d �Ѿ�׼���ý��ס�",order->_order_id.c_str(),strRoleType.c_str(),order->getClient()->getMachineId());
			order->setTradeTime(DateTime(1,1,2000,0,0,0));
			order->log(logs);
			saveOrder(order);
			break;
		}
	case EOS_TRADE_COMPLETE:
		{
			/*��¼�¼�*/
			ASSERT(order->getTradeResult() != 0);

			int result = order->getTradeResult();
			if (result == MAC_PROCESSING_SUCCESS)//���׳ɹ�
			{
				char logs[1024] = {0};
				sprintf_s(logs,sizeof(logs),"����:%s-%s������ϣ�������Ϊ%d��",order->_order_id.c_str(),strRoleType.c_str(),order->getTradeResult());
				order->log(logs);
				saveOrder(order,true);

				//�ӿͻ������ڴ�������ɾ���Լ�
				CTradeClient* client = order->getClient();
				if (client){
					client->removeOrderFromProcessingList(order);
					//DebugMessageA("ECbk_OrderStateChanged�������Ѿ���ɡ�mac_sell->removeOrderFromProcessingList");
				}

				delOrder(order);//�Ӷ����б���ɾ���Լ�

			}else{
				//����ʧ����
				if (!orderCanRetry(order)){	//ʧ�ܣ����Ҳ�������
					char logs[1024] = {0};
					sprintf_s(logs,sizeof(logs),"����:%s-%s������ϣ�������Ϊ%d��",order->_order_id.c_str(),strRoleType.c_str(),order->getTradeResult());
					order->log(logs);

					saveOrder(order,true);

					CTradeOrder* buddy = order->getBuddyOrder();
					if (buddy == NULL)
					{
						LOG_ERROR("���ش���order��buddy orderΪ��");
					}else{
						if (buddy->getClient() == NULL){
							LOG_ERROR("order��buddy order��clientΪ��,�޷����ͽ��׶���ʧ����Ϣ��");
						}else{
							buddy->getClient()->PM_MyBuddyFailed(buddy->getDispatchId(),result);
						}
					}

					//�ӿͻ������ڴ�������ɾ���Լ�
					CTradeClient* client = order->getClient();
					if (client){
						client->removeOrderFromProcessingList(order);
						//DebugMessageA("ECbk_OrderStateChanged�������Ѿ���ɡ�mac_sell->removeOrderFromProcessingList");
					}
					//�Ӷ����б���ɾ���Լ�
					delOrder(order);

				}else{
					//�ӿͻ������ڴ�������ɾ���Լ�
					CTradeClient* client = order->getClient();
					if (client){//�ÿͻ��˿����Ѿ�����
						client->removeOrderFromProcessingList(order);
						//DebugMessageA("ECbk_OrderStateChanged�������Ѿ���ɡ�mac_sell->removeOrderFromProcessingList");
					}

					//ʧ�ܣ����ǿ�������
					if (0==order->getRetryTimes())//����������Դ�����ɾ��������
					{
						order->setTradeResult(RETRY_TIME_END);
						char logs[1024] = {0};
						sprintf_s(logs,sizeof(logs),"����:%s-%sʧ�ܳ������������ö�������Ϊ����ʧ�ܴ����Ϊ%d��",order->_order_id.c_str(),strRoleType.c_str(),RETRY_TIME_END);
						order->log(logs);
						CTradeOrder* buddy = order->getBuddyOrder();
						if (buddy == NULL)
						{
							LOG_ERROR("���ش���order��buddy orderΪ��");
						}else{
							if (buddy->getClient() == NULL){
								LOG_ERROR("buddyorder��clientΪ��,�޷����ͽ��׶���ʧ����Ϣ��");
							}else{
								buddy->getClient()->PM_MyBuddyFailed(buddy->getDispatchId(),result);
							}
						}
						//ɾ���Լ�
						saveOrder(order,true);
						delOrder(order);
					}else{	//�������ԣ���ô��������
						order->setRetryTimes(order->getRetryTimes()-1);
						order->resetOrderState();
						char logs[1024] = {0};
						sprintf_s(logs,sizeof(logs),"����:%s-%s�������������󣬶������Դ�����һ",order->_order_id.c_str(),strRoleType.c_str());
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
	ECC_GAME_COLLAPSE = 0x100,			//�ͻ��˱�������������ԭ�����á�ע�⽻�׹��̲��ܱ�����Ҫ�Ǳ����˾��鷳�ˡ�
	ECC_LOGIN_SERVER_ERROR = 0x101,		//������ά������æ��etc.
	TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL = 0x108,//������Ҫ������ȡ������
	TRADE_FAILED_REASON_SERVER_COMMAND_DENY = 0x109,//�������ܾ��˽�������
	TRADE_FAILED_REASON_BUDDY_DISCONNECTED = 0x10B,//����������ȡ�����ף���Ϊ���׶������
	TRADE_FAILED_REASON_I_AM_DISCONNECTED = 0x10C,//���׿ͻ���������ߡ�
	*/
	//��Щ����ԭ���ǿ������Եģ��������Ǯ�������˺����벻�Եȴ���϶����������ԡ�
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
		setDataSourceEmpty(CSettings::getInstance().getDataSouceQueryRetryTime(getGameTypeId()));//60���ڲ�������װ�ض���
	}
	addOrder(oli);
}
