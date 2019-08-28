#pragma once

#include "util/NonCopyable.h"
#include "util/Event.h"
#include <vector>
#include <list>
#include <string>
using std::vector;
using std::list;
using std::string;
#include "../common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include "../common/AutoLock.h"
#include "TradeCodeDef.h"
class DateTime;
class CTradeOrder;
class CLock_CS;
class CConfirmOrder;

struct MacBurdenAppend{
	int _seconds_per_success_order;						//< ƽ���ɹ�����һ�������ʱ��
	int _last_hour_processed_success_order;				//< ��һ��Сʱ����ɹ����˺Ÿ���
	int _processed_count_total;							//< �������ϴ����˶��ٸ��˺�
	int _processed_count_failed;						//< �������Ϻ���ʧ���˶��ٴΡ�
};

class CTradeNetworkInterface;
/**
 * @class TradeClientMachine
 * ���׿ͻ�����
 * ע��ͻ��˶��ߵ�ʱ��Ҫ�ͷŵ�CTradeClient�����䱣�����������ߵ�ʱ���ٻ���ʹ�á�
 * ��ΪPMϵ�к���Ҫ����̰߳�ȫ�������һ���̵߳���PM������ʱ����һ�����������ͷź����Ǿͱ����ˡ�
 */
class CTradeClient : NonCopyable
{
public:
	/**
	 * �ͻ����ĵ�ǰ״̬����������ʱ��Ҫ�Կͻ����ĵ�ǰ״̬�����ء�ƽ�����ؽ��м����Է�����ʵĻ��������ס�
	 * ��ʾ״̬������Խ�󱻷���Ļ����ԽС���ڶԿͻ��������ʱ�����õ���
	 */
	enum EMachine_state{
		EClientState_connected_idle = 1,	//< �û�����δ�ﵽ����أ����ڿ���״̬
		EClientState_connected_busy,		//< �û����Ѿ��ﵽ����أ����ڷ�æ״̬
		EClientState_coma,					//< ����״̬������δ�ϣ�������������Ӧ������һ��ʱ��󽫱��ߵ���
		EClientState_disconnected,			//< �ÿͻ�����δ���ӡ�
	};

	struct history_record 
	{
		string _order_id;
		int _result_id;
		time_t _time;
	};

public:

	BOOL PM_AssignOrder(order_struct* order);	
	void PM_AssignOrderFailed(EDispatchError errCode);
	void PM_StateChangedAckMsg(int order_dispatch_id,int error_code);
	void PM_StateChangedAck_HasGetOrder_X(int order_dispatch_id,int error_code);
	void KillSession();
	void PM_MyBuddyFailed(int order_dispatch_id,int error_code);

	BOOL PM_AssignConfirm(order_struct* confirm_order);
	void PM_ConfirmStateChangedAckMsg(int order_dispatch_id,int error_code);
	void PM_ConfirmHasGetOrder_X(int order_dispatch_id,int error_code);

	CTradeClient(int game_type_id,
						const string& machine_name,
						int machine_id,
						const string& ip,
						int max_order_count,
						CTradeNetworkInterface* network);
	~CTradeClient();

	int getMacBurden() const{ return _mac_burden;}
	void setMacBurden(int burden){ InterlockedExchange((long*)(&_mac_burden),burden);}	//use InterLockExchange?
	
	int getMacBurdenEx() const{return _processed_count;}	//< ����ĸ�����Ϣ����Ϊ�Ը��صĲ��䡣ע��ƽ����ͻ����ĸ��ء�

	void setMacState(EMachine_state state){ InterlockedExchange((long*)(&_state),state); }
	EMachine_state getMacState()const { return _state; }

	int getMachineId()const {return _machine_id;}
	
	string getMacName()const {return _machine_name;}

	int getGameTypeId()const {return _game_type_id;}
	int getMaxOrderCount() const {return _max_order_count;}
	int getCurOrderCount() const {return _processing_orders.size();}
	int getCurConfirmCount() const {return _processing_confirms.size();}

	string getIp() const { return _ip; }
	void setIp(string val) { _ip = val; }

	void addOrderToProcessingList(CTradeOrder* order);
	void addConfirm(CConfirmOrder* confirm_order);
	void OnDisconnect();

	void removeOrderFromProcessingList(CTradeOrder* order);//< ע������б�����2����ͬ��order,��ôֻɾ������һ����
	void removeConfirm(CConfirmOrder* confirm);
//	CTradeNetworkInterface* getNetwork() const { return _network; }
	void setNetwork(CTradeNetworkInterface* val) { CAutoLock lock(*_network_protect_lock);_network = val; }

	string dump() const;

	void clearOrders();	//< �����š��˳���ͨ����ͻ��˷���ȡ�����н��׵���Ϣ��
	void clearConfirmOrder();

	void force_clear_orders_and_confirms();	//< ��ǿ�С��˳���ֱ�ӹر�����

	void ProgressTradeStateChanged(int dispatche_id,EClientOrderProgress progress,short client_result_code);

	void ProgressConfirmStateChanged(int dispatche_id,EClientConfirmProgress progress,short client_result_code);

	static string EOrderProgressClientToString(EClientOrderProgress eopc);
	static string EConfrimProgressClientToString(EClientConfirmProgress progress);

	//�ͻ��˽��ȣ�˫����׼���ý���
	Event<void (CTradeClient* /*sender*/,CTradeOrder* /*order*/)> _event_both_ready_to_trade;
protected:
private:
	void unexpected_order_state_on_disconnect(CTradeOrder* order);
	void recyle_order_on_disconnect(CTradeOrder* order);
	CTradeOrder* getProcessingOrderById(int dispatchId);
	CConfirmOrder* getConfirmById(int dispatchId);

	int _game_type_id;						//< �ÿͻ����ܴ������Ϸ����
	EMachine_state _state;					//< ��ǰ�ͻ�����״̬��Ĭ��Ϊbusy,���ͻ��˷���һ��trade requestʱ����Ϊidle.
	list<CTradeOrder*> _processing_orders;	//< �û������ڴ���Ľ��׶���������Ϸ����࿪�����Ҹû���ͬʱ���ɷ�Ϊ����˫���Ŀͻ���ʱ��_processing_order����2����ͬ��order
	list<CConfirmOrder*> _processing_confirms;

	string _ip;								//< ��ǰip
	string _machine_name;					//< ����������ʶ
	int _machine_id;						//< �������

	int _processed_success_count;			//< �����ӵ���������ɹ������˶��������ס�
	int _processed_count;					//< �����ӵ������������˶����˺�,��ȥ����ľ���ʧ�ܴ�����ʧ���ʿ�����Ϊ�ɷ��Ĳο���
	DateTime* _start_time;					//< �û������ӵ�����������ʼʱ�䡣
	int _seconds_per_success_order;			//< �ɹ�����һ�ε�ƽ��ʱ�䣬������������������Ϊ���Ը��صĲο���ͬΪidle�����ѡ������С�Ĵ����ס�

	int _max_order_count;					//< ͬʱ�ɴ�������������ޣ����ڵ������͵���Ϸʼ��Ϊһ��
											//< ע�⣺�����Ϊ���ز����������
	list<history_record> _histories;		//< ��ʷ��¼������ͳ��ĳһ��ʱ���ڵĸ���ֵ��
	
	bool _request_acc;						//< �ͻ����Ƿ��Ϳ��Խ��׵���Ϣ
	int _mac_burden;						//< �ͻ��˵ĵ�ǰ����

	CTradeNetworkInterface* _network;		//< �ÿͻ����ڷ��������ڷ�����Ϣ���������

	CLock_CS* _confirm_list_lock;
	CLock_CS* _order_list_lock;
	CLock_CS* _network_protect_lock;		//< �����ڶ��̻߳�������һ���߳���_network������ΪNULL���Ự�Ͽ���������һ���߳���_network��ʹ�á�
};


class CompareBurden{
public:
	CompareBurden(){;}
	bool operator()(const CTradeClient* mac1, const CTradeClient* mac2){
		if (mac1->getMacState() != mac2->getMacState())
			return mac1->getMacState() < mac2->getMacState();
		else
			if (mac1->getMacBurden() == mac2->getMacBurden())
				return mac1->getMacBurdenEx() < mac2->getMacBurdenEx();//������һ��ʱ���Ƚϸ��ظ�����Ϣ����ʷ��¼�ȡ�
			else
				return mac1->getMacBurden() < mac2->getMacBurden();
	}
};


/**
 * @class CClientMacCollection
 * �ͻ�����Ⱥ��
 */

class CClientMacControler : public NonCopyable
{
public:
	CClientMacControler(int game_type_id,const string& game_type_describe,bool is_muti_client_game);
	virtual ~CClientMacControler();
public:
	bool isExit()const {return _exit_flag;}

	bool addMac(CTradeClient* mac);

	void delMac(int client_mac_id);
	
	CTradeClient* getMac(int c_id, const string& name );

	int getGameTypeId() const { return _game_type_id; }

	int getCurrentProcessOrderCount() const;

	int getCurrentProcessConfirmCount() const;

	virtual string dump() const;

	void exit();
	void force_exit();
private:
	bool _exit_flag;										//< �Ƿ�������״̬
	bool _is_muti_client_game;								//< �Ƿ�֧�ֶ࿪
	int _game_type_id;										//< ��Ϸ����
	string _game_type_describe; 							//< ��Ϸ���͵���������
	list<CTradeClient*> _client_machine_list;		//< �ͻ����б�ͬһ����Ϸ����������ظ��Ļ�����
	CLock_CS* _mac_list_locker;
};