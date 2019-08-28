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
	int _seconds_per_success_order;						//< 平均成功处理一个请求的时间
	int _last_hour_processed_success_order;				//< 上一个小时处理成功的账号个数
	int _processed_count_total;							//< 自连接上处理了多少个账号
	int _processed_count_failed;						//< 自连接上后处理失败了多少次。
};

class CTradeNetworkInterface;
/**
 * @class TradeClientMachine
 * 交易客户机类
 * 注意客户端断线的时候不要释放掉CTradeClient而让其保留等重新连线的时候再回收使用。
 * 因为PM系列函数要求多线程安全，如果在一个线程调用PM函数的时候另一个函数调用释放函数那就悲剧了。
 */
class CTradeClient : NonCopyable
{
public:
	/**
	 * 客户机的当前状态，分派任务时需要对客户机的当前状态、负载、平均负载进行技术以分配合适的机器处理交易。
	 * 表示状态的数字越大被分配的机会就越小。在对客户机排序的时候有用到。
	 */
	enum EMachine_state{
		EClientState_connected_idle = 1,	//< 该机器尚未达到最大负载，处于空闲状态
		EClientState_connected_busy,		//< 该机器已经达到最大负载，处于繁忙状态
		EClientState_coma,					//< 昏迷状态，连接未断，但是无心跳相应。持续一段时间后将被踢掉。
		EClientState_disconnected,			//< 该客户端尚未连接。
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
	
	int getMacBurdenEx() const{return _processed_count;}	//< 额外的负载信息，作为对负载的补充。注意平衡各客户机的负载。

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

	void removeOrderFromProcessingList(CTradeOrder* order);//< 注意如果列表内有2个相同的order,那么只删除其中一个。
	void removeConfirm(CConfirmOrder* confirm);
//	CTradeNetworkInterface* getNetwork() const { return _network; }
	void setNetwork(CTradeNetworkInterface* val) { CAutoLock lock(*_network_protect_lock);_network = val; }

	string dump() const;

	void clearOrders();	//< “优雅”退出，通过向客户端发送取消所有交易的信息。
	void clearConfirmOrder();

	void force_clear_orders_and_confirms();	//< “强行”退出，直接关闭连接

	void ProgressTradeStateChanged(int dispatche_id,EClientOrderProgress progress,short client_result_code);

	void ProgressConfirmStateChanged(int dispatche_id,EClientConfirmProgress progress,short client_result_code);

	static string EOrderProgressClientToString(EClientOrderProgress eopc);
	static string EConfrimProgressClientToString(EClientConfirmProgress progress);

	//客户端进度，双方已准备好交易
	Event<void (CTradeClient* /*sender*/,CTradeOrder* /*order*/)> _event_both_ready_to_trade;
protected:
private:
	void unexpected_order_state_on_disconnect(CTradeOrder* order);
	void recyle_order_on_disconnect(CTradeOrder* order);
	CTradeOrder* getProcessingOrderById(int dispatchId);
	CConfirmOrder* getConfirmById(int dispatchId);

	int _game_type_id;						//< 该客户机能处理的游戏类型
	EMachine_state _state;					//< 当前客户机的状态，默认为busy,当客户端发送一个trade request时设置为idle.
	list<CTradeOrder*> _processing_orders;	//< 该机器正在处理的交易订单，当游戏允许多开，并且该机器同时被派发为买卖双方的客户机时，_processing_order将有2个相同的order
	list<CConfirmOrder*> _processing_confirms;

	string _ip;								//< 当前ip
	string _machine_name;					//< 机器描述标识
	int _machine_id;						//< 机器编号

	int _processed_success_count;			//< 自连接到服务器后成功处理了多少批交易。
	int _processed_count;					//< 自连接到服务器后处理了多少账号,减去上面的就是失败次数。失败率可以作为派发的参考。
	DateTime* _start_time;					//< 该机器连接到服务器的起始时间。
	int _seconds_per_success_order;			//< 成功处理一次的平均时间，此项与上三项用于作为测试负载的参考。同为idle情况下选负载最小的处理交易。

	int _max_order_count;					//< 同时可处理订单的最高上限，对于单开类型的游戏始终为一。
											//< 注意：该项不作为负载测算的条件。
	list<history_record> _histories;		//< 历史记录，用于统计某一段时间内的负载值。
	
	bool _request_acc;						//< 客户端是否发送可以交易的信息
	int _mac_burden;						//< 客户端的当前负载

	CTradeNetworkInterface* _network;		//< 该客户端在服务器用于发送消息的网络对象

	CLock_CS* _confirm_list_lock;
	CLock_CS* _order_list_lock;
	CLock_CS* _network_protect_lock;		//< 避免在多线程环境下在一个线程中_network被设置为NULL（会话断开）而在另一个线程中_network被使用。
};


class CompareBurden{
public:
	CompareBurden(){;}
	bool operator()(const CTradeClient* mac1, const CTradeClient* mac2){
		if (mac1->getMacState() != mac2->getMacState())
			return mac1->getMacState() < mac2->getMacState();
		else
			if (mac1->getMacBurden() == mac2->getMacBurden())
				return mac1->getMacBurdenEx() < mac2->getMacBurdenEx();//当负载一样时，比较负载附加信息【历史记录等】
			else
				return mac1->getMacBurden() < mac2->getMacBurden();
	}
};


/**
 * @class CClientMacCollection
 * 客户机集群。
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
	bool _exit_flag;										//< 是否处于能用状态
	bool _is_muti_client_game;								//< 是否支持多开
	int _game_type_id;										//< 游戏类型
	string _game_type_describe; 							//< 游戏类型的文字描述
	list<CTradeClient*> _client_machine_list;		//< 客户机列表。同一款游戏不允许出现重复的机器。
	CLock_CS* _mac_list_locker;
};