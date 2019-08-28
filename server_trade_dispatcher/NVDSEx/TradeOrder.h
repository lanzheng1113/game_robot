#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <list>
using std::list;
#include "util/NonCopyable.h"
#include "../common/wares.h"
#include "util/DateTime.h"
#include "util/Event.h"
#include "util/Trackable.h"
#include "../common/TradeMesgDef.h"
#include "../common/shareStruct.h"
#include <boost/function.hpp>
enum EEmgcLevel{
	//定单的紧急程度
	EEL_NORMAL = 0,		//< 正常
	EEL_FAST,			//< 加急
	EEL_FASTER,			//< 十万火急
	EEL_FASTEST			//< 世界末日
};

enum EOrderState{
	EOS_GETFROM_DB,			//< 从数据库中取出，初始状态
	EOS_SEND_TO_DISPATCHER, //< 被取出给派发器
	EOS_DISPATCHED,			//< 已经被派发给交易客户端
	EOS_READY_TO_TRADE,		//< 两边的客户端都已经开启已准备好交易
	EOS_TRADE_COMPLETE,		//< 交易已经完成
};

class COrderRecorder;
class CTradeClient;

class CTradeOrder : public NonCopyable
{
public:
	CTradeOrder();
	~CTradeOrder();
public:
	/*以下这些在订单的分配过程中不会发生改变，因而直接作为public类型*/
	int _game_type;
	string _order_id;				//< 订单ID，必需并且全部游戏的任意一订单有其唯一值。绝不允许重复
	bool isValid(){ return _order_id != "bad_id"; }
	EClientRoleType _tradeRole;
	string _acc;
	string _pswd;
	string _role_name;
	string _buddy_role_name;			//< 交易对象的角色名
	string _world;						//< 区域信息
	string _trade_place;				//< 接头地点
	Wares _trade_good_detail;			//< 交易物品以及价格的具体信息，必需
	EEmgcLevel _emergency_level;		//< 优先级别，数字越大优先级越高。高优先级的定单将被分配更好的资源用于处理	
	bool _type_trade_is_money;			//< 交易类型是否为金钱交易，默认是。
 	Event<void (CTradeOrder*,EOrderState)> _event_order_state_changed;
	string _log;
public:
	EOrderState getCurState() const { return _cur_state; }
	void setCurState(EOrderState val);
	CTradeClient* getClient() const { return _client; }
	void setClient(CTradeClient* val) { _client = val; }
	short getTradeResult() const { return _trade_result; }
	void setTradeResult(short val) { _trade_result = val; }
	int getRetryTimes() const { return _retry_times; }
	void setRetryTimes(int val) { _retry_times = val; }
	DateTime getGetTime() const { return *_get_time; }
	void setGetTime(const DateTime& val) { *_get_time = val; }
	DateTime getDispatchedTime() const { return *_dispatched_time; }
	void setDispatchedTime(const DateTime& val) { *_dispatched_time = val; }
	DateTime getTradeTime() const { return *_trade_time; }
	void setTradeTime(const DateTime& val) { *_trade_time = val; }
	EClientOrderProgress getProgressClient() const { return _progress_client; }
	void setProgressClient(EClientOrderProgress val) { _progress_client = val; }
	CTradeOrder* getBuddyOrder() const { return _buddy_order; }
	void setBuddyOrder(CTradeOrder* val) { _buddy_order = val; }
	int getDispatchId() const { return _dispatch_id; }
	void setDispatchId(int val) { _dispatch_id = val; }
	int getTradeMoney() const { return _trade_money; }
	void setTradeMoney(int val) { _trade_money = val; }
	BOOL getIs_log_to_text() const { return _is_log_to_text; }
	void setIs_log_to_text(BOOL val) { _is_log_to_text = val; }
	void resetOrderState(){_cur_state = EOS_GETFROM_DB;}
	string getOrderParams() const { return _order_params; }
	void setOrderParams(string val) { _order_params = val; }
public:
	string dump() const;
 	static string EOrderStateToString(EOrderState stat);	
 	static string EEmgcLevelToString(EEmgcLevel elv);
 	void initlog();
 	void log(const string& val);

	VOID					Remove() { InterlockedExchange( (long*)&_isRemove, TRUE ); }
	VOID					ResetKillFlag() { InterlockedExchange( (long *)&_isRemove, FALSE ); }///< Reset remove flag.if m_bRemove is TRUE,it will be removed in active session list.
	BOOL					ShouldBeRemoved() { return _isRemove; }///<Access remove flag.
private:
	BOOL _is_log_to_text;
	BOOL _isRemove;
	int _trade_money;
	int _dispatch_id;
	/*以下内容在订单派发过程中会发生改变，因而需要保证线程安全*/
	EOrderState _cur_state;				//< 当前订单所属的处理过程。
	CTradeClient* _client;				//< 处理该订单的客户端
	short _trade_result;				//< 交易机返回的处理结果，必须初始化为0[未处理]，该值的解释从配置中读取。
	int _retry_times;					//< 订单处理失败后的可重试次数
	/*以下两个是为了保护指针对象，防止被误用，例如发生_dispatchd_time = &xxx_time的情况，正确使用是*_dispached_time = xxx_time*/
	DateTime* _get_time;				//< 从订单服务器获得的时间，为对象创建时的时间
	DateTime* _dispatched_time;			//< 将订单交给客户机的时间。未被分派时其值为2000-1-1 00:00:00
	DateTime* _trade_time;				//< 执行交易的时间，初始值为2000-1-1 00:00:00
	COrderRecorder* _recorder;
	EClientOrderProgress _progress_client;	//< 进度信息
	CTradeOrder* _buddy_order;
	string _order_params;
};

class CTradeOrders
{
public:
	CTradeOrder* orderSeller;
	CTradeOrder* orderBuyer;
protected:
private:
};

//行为上类似与结构体。
// class CTradeOrder : public NonCopyable
// {
// public:
// 	CTradeOrder();
// 	~CTradeOrder();
// public:
// 	/*以下这些在订单的分配过程中不会发生改变，因而直接作为public类型*/
// 	int _game_type;
// 	string _order_id;				//< 订单ID，必需并且全部游戏的任意一订单有其唯一值。绝不允许重复
// 	bool isValid(){ return _order_id != "bad_id"; }
// 	
// 	string _acc_seller;				//< 出售方帐号，必需
// 	string _pswd_seller;			//< 出售方密码，必需
// 	string _acc_buyer;				//< 购买方帐号，必需
// 	string _pswd_buyer;				//< 购买方密码，必需
// 	string _role_name_seller;		//< 出售方角色名
// 	string _role_name_buyller;		//< 购买方
// 	string _world;					//< 区域信息
// 	string _trade_place;			//< 接头地点
// 	DateTime* _get_time;			//< 从订单服务器获得的时间，为对象创建时的时间
// 	Wares _trade_good_detail;		//< 交易物品以及价格的具体信息，必需
// 	EEmgcLevel _emergency_level;	//< 优先级别，数字越大优先级越高。高优先级的定单将被分配更好的资源用于处理	
// 	bool _type_trade_is_money;		//< 交易类型是否为金钱交易，默认是。
// 	string dump() const;
// 
// 	static string EOrderStateToString(EOrderState stat);	
// 	static string EEmgcLevelToString(EEmgcLevel elv);
// 	
// 	Event<void (CTradeOrder*,EOrderState)> _event_order_state_changed;
// 
// public:
// 
// 	void initlog();
// 
// 	void log(const string& val);
// 	/*线程安全函数，因为处理订单至少牵涉到买卖双方，如果双方在不同的线程中同时发送消息时可能导致竞争。*/
// 	EOrderState getCurState() const { return _cur_state; }
// 	void setCurState(EOrderState val);
// 
// 	int getTradeResult() const { return _trade_result; }
// 	void setTradeResult(int val) { _trade_result = val; }	//< @FIXME:可能需要修改这里使用InterlockedExchange
// 	
// 	CTradeClient* getBuyerMachine() const { return _buyer_machine; }
// 	void setBuyerMachine(CTradeClient* val) { _buyer_machine = val; }	//< @FIXME:可能需要修改这里使用InterlockedExchange
// 
// 	CTradeClient* getSellerMachine() const { return _seller_machine; }
// 	void setSellerMachine(CTradeClient* val) { _seller_machine = val; }	//< @FIXME:可能需要修改这里使用InterlockedExchange
// 
// 	EClientOrderProgress getSellerProgress() const { return _progress_seller; }
// 	void setProgressSeller(EClientOrderProgress val) { _progress_seller = val; }
// 
// 	EClientOrderProgress getBuyerProgress() const { return _progress_buyer; }
// 	void setProgressBuyer(EClientOrderProgress val) { _progress_buyer = val; }
// 
// 	short getSellerResultCode() const { return _seller_result_code; }
// 	void setSellerResultCode(short val) { _seller_result_code = val; }
// 	
// 	short getBuyerResultCode() const { return _buyer_result_code; }
// 	void setBuyerResultCode(short val) { _buyer_result_code = val; }
// 
// 	int getRetryTimes() const { return _retry_times; }
// 	void setRetryTimes(int val) { _retry_times = val; }	//< @FIXME:可能需要修改这里使用InterlockedExchange
// 
// 	/*保护指针对象不被破坏，如_dispatchd_time = &xxx_time的情况_dispatchd_time指针就被破坏了*/
// 	DateTime getDispatchedTime() const { return *_dispatched_time; }
// 	void setDispatchedTime(const DateTime& val) { *_dispatched_time = val; }
// 	DateTime getTradeTime() const { return *_trade_time; }
// 	void setTradeTime(const DateTime& val) { *_trade_time = val; }
// 
// private:
// 	/*以下内容在订单派发过程中会发生改变，因而需要保证线程安全*/
// 	EOrderState _cur_state;			//< 当前订单所属的处理过程。
// 	CTradeClient* _seller_machine;			//< 处理该订单的出售机。
// 	CTradeClient* _buyer_machine;			//< 购买机
// 	int _trade_result;				//< 交易机返回的处理结果，必须初始化为0[未处理]，该值的解释从配置中读取。
// 	int _retry_times;				//< 订单处理失败后的可重试次数
// 	/*以下两个是为了保护指针对象，防止被误用，例如发生_dispatchd_time = &xxx_time的情况，正确使用是*_dispached_time = xxx_time*/
// 	DateTime* _dispatched_time;		//< 将订单交给客户机的时间。未被分派时其值为2000-1-1 00:00:00
// 	DateTime* _trade_time;			//< 执行交易的时间，初始值为2000-1-1 00:00:00
// 	COrderRecorder* _recorder;
// 	EClientOrderProgress _progress_seller;
// 	EClientOrderProgress _progress_buyer;
// 	short _seller_result_code;		//卖方传来的最后结果代码
// 	short _buyer_result_code;		//买方传来的最后结果代码
// };

enum dispatch_stage{
	one_by_one_sort_in_time,
	by_emergency_level,
};

class CLock_CS;

/**
 * @class CTradeOrderControler
 * 为支持多种类型游戏，使用CTradeOrderControler订单管理单元管理订单。一种游戏对应一种订单管理单元。
 * 
 * 功能
 * 1、从远程接口获得对应游戏的订单信息，并储存到待分配列表中。
 *		交易订单被数据库中取出的时候将被分为买卖双方两个关联的订单进行单独的处理。如果从服务器取得了N个订单，那么实际派发给客户机的是2N个。
 * 2、当交易机请求订单时，将订单派发给交易机。
 *		按照取出的顺序排序，先进先出。
 * 3、交易完成（成功或者失败）后将结果写入到数据库。并返回给远程接口
 * 
 */
class CTradeOrderControler : public NonCopyable,public Trackable
{
public:
	CTradeOrderControler(int game_type_id,const string& game_type_describe,dispatch_stage dispatchstage);
	virtual ~CTradeOrderControler();
protected:
public:
	//属性访问接口
	int getGameTypeId() const { return _game_type_id; }					//< 获得游戏类型，只读，在对象创建时被初始化
	string getGameTypeDescribe() const { return _game_type_describe; }	//< 获得游戏类型描述字符串，只读，在对象创建时被初始化
	dispatch_stage getStage() const { return _stage; }					//< 获得分配策略，只读，在对象创建时被初始化
	int getTradeListCount() const{ return _trade_order_list.size();}	//< 获得当前列表个数
	void setDataSourceEmpty(long time_out){								//< 在未来time_out毫秒内本派发器不处理get Order请求
		InterlockedExchange(&_is_in_source_empty,1);
		_source_empty_timeout = time_out;
		_time_set_empty = GetTickCount();
	}

	BOOL getIsEnable() const { return _isEnable; }
	/**
	 * 关闭派发器 will call recycleOrder.
	 * @remark:程序退出时被调用①将未派发的账号写入到数据库②标记已派发但未收到交易成功消息的账号，并写入到数据库
	 */
	void exit();
	void Enable(BOOL enable){InterlockedExchange((long*)&_isEnable,enable);}

public: //EVENT callbacks.
	//监视CTradeOrder的状态改变，ECbk_OrderStateChanged被绑定到CTradeOrder状态改变事件_event_order_state_changed上，当CTradeOrder的状态改变时此回调被调用。
	void ECbk_OrderStateChanged(CTradeOrder* sender,EOrderState state);

	void setLogListenerCallback(boost::function<void (const string&,const string&,short,short)> val) { _fun_new_log = val; }
public:
	/*
	 * 导入账号到当前列表中
	 * @param maxCountPerBatch 允许一次导入最多多少个。
	 * @return 返回本次导入的待交易订单列表，注意不是合并后的内部列表。
	 */
	virtual list<CTradeOrder*> loadOrders(int maxCountPerBatch) = 0;
	
	/**
	 * 记录交易结果,统计交易结果等等。
	 * @param 
	 */
	virtual void WriteEvent(const CTradeOrder& order,__time64_t trade_time,int balances,int trade_result_code_id) = 0;
	/**
	 * 获得下一个即将派发的账号。
	 * @param acc_to_trade 返回的账号
	 * @return 成功返回true,失败返回false.
	 * @remark: 注意在账号被取出或者派发后，列表内该账号的状态都要改变，避免重复。
	 */
	virtual CTradeOrder* GetNextOrder();

	/**
	 * 保存order到数据库，根据can_be_fetch决定是否设置数据库的getState字段（该字段决定数据库内order是否可取出）。
	 * @param order:订单
	 * @param can_be_fetch:如为true则该order被完全回收到数据库中getState字段被重置为0，如false则仅更新状态
	 */
	virtual void saveOrder(CTradeOrder* order,bool can_be_fetch = false);
	void saveOrderLog(CTradeOrder* seller,CTradeOrder* buyer);
	bool isExit() const { return _exit_flag; }
	void tradeOrderLoaded(list<CTradeOrder*>& oli);
protected:
	/**
	 * 检查是否可从数据源获得订单。
	 */
	bool CheckDataSourceIdle();
	
	/**
	 * 添加到列表接口,如列表已有账号则直接返回。
	 * @param order: 订单
	 * @remark:添加订单到列表时，如果列表内已经有同一订单号的订单，则直接返回。
	 */
	virtual bool addOrder(CTradeOrder* order);
	
	/**
	 * 合并order_list到内部列表，添加后保证无重复的order_id
	 */
	virtual bool addOrder(list<CTradeOrder*> order_list);
	
	/**
	 * 从列表删除删除，没有该订单也不返回错误。
	 * @param order_id:订单id
	 * @remark  如订单存在删除后释放该Order的内存，此函数线程安全。
	 */
	virtual void delOrder(CTradeOrder* order);//不开放此接口

private:
	/**
	 * 回收订单到数据库，一般仅在程序将退出时被调用。
	 */
	void exitRecyleOrder(CTradeOrder* order);
	/**
	 * 限定了重试次数
	 */
	bool orderCanRetry(CTradeOrder* order);

private:
	BOOL _isEnable;							//< 使能开关，当为false时不允许从派发列表中取账号。
	bool _exit_flag;						//< 退出标识，如已退出则不能执行派发和导入账号操作。
	int _game_type_id;						//< 游戏类型
	string _game_type_describe; 			//< 游戏类型的文字描述
	list<CTradeOrder*> _trade_order_list;	//< 订单列表。绝不可出现重复值，否则程序将发生运行错误。
	dispatch_stage _stage;					//< 发号策略。
	CLock_CS* _list_process_locker;			//< 列表访问锁。
	long _is_in_source_empty;				//< 数据源空标志，若为数据源为空(即源服务器和临时服务器当前均无订单)，GetNextOrder在_time_set_empty+_source_empty_timeout时间到来之前返回false;
	long _source_empty_timeout;				//< 超时时间，单位为毫秒
	unsigned long _time_set_empty;			//< 设置_is_in_source_empty标志为1的时间=GetTickCount();

	boost::function<void (const string&,const string&,short,short)> _fun_new_log;
};

