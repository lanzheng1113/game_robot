#pragma once
#include <string>
using std::string;
class DateTime;
#include "libutil/util/DateTime.h"
#include "libutil/util/NonCopyable.h"
#include "libutil/util/Event.h"
#include "../common/TradeMesgDef.h"

enum EConfirmState{
	ECS_GETFROM_DB,			//< 从数据库中取出，初始状态
	ECS_SEND_TO_DISPATCHER, //< 被取出给派发器
	ECS_DISPATCHED,			//< 已经被派发给客户端
	ECS_COMPLETE,			//< 已经完成确认
};

class COrderRecorder;
class CTradeClient;

class CConfirmOrder : public NonCopyable
{
public:
	CConfirmOrder();
	~CConfirmOrder();
	void					initlog();
	void					log(const string& val);
	VOID					Remove() { InterlockedExchange( (long*)&_isRemove, TRUE ); }
	VOID					ResetKillFlag() { InterlockedExchange( (long *)&_isRemove, FALSE ); }///< Reset remove flag.if m_bRemove is TRUE,it will be removed in active session list.
	BOOL					ShouldBeRemoved() { return _isRemove; }///<Access remove flag.
	static string			EOrderConfirmStateToString(EConfirmState state);
	Event<void (CConfirmOrder*,EConfirmState)> _event_confirm_order_state_changed; //< 事件：订单状态改变
public:
	int						getGameTypeId() const { return _game_type_id; }
	void					setGameTypeId(int val) { _game_type_id = val; }
	int						getDispatchId() const { return _dispatch_id; }
	void					setDispatchId(int val) { _dispatch_id = val; }
	string					getOrderId() const { return _order_id; }
	void					setOrderId(const string& val) { _order_id = val; }
	string					getAccount() const { return _account; }
	void					setAccount(const string& val) { _account = val; }
	string					getPassword() const { return _password; }
	void					setPassword(string val) { _password = val; }
	string					getRole() const { return _role; }
	void					setRole(const string& val) { _role = val; }
	string					getWorld() const { return _world; }
	void					setWorld(const string& val) { _world = val; }
	EConfirmState			getCurState() const { return _cur_state; }
	void					setCurState(EConfirmState val);
	int						getResultId() const { return _result_id; }
	void					setResultId(int val) { _result_id = val; }
	int						getMoneyConfirm() const { return _money_confirm; }
	void					setMoneyConfirm(int val) { _money_confirm = val; }
	DateTime				getGetTime() const { return _get_time; }
	void					setGetTime(const DateTime& val) { _get_time = val; }
	DateTime				getDispatchTime() const { return _dispatch_time; }
	void					setDispatchTime(const DateTime& val) { _dispatch_time = val; }
	DateTime				getCompleteTime() const { return _complete_time; }
	void					setCompleteTime(const DateTime& val) { _complete_time = val; }
	BOOL					getIsLogToText() const { return _is_log_to_text; }
	void					setIsLogToText(BOOL val) { _is_log_to_text = val; }
	CTradeClient*			getClient() const { return _client; }
	void					setClient(CTradeClient* val) { _client = val; }
	bool					getTypeTradeIsMoney() const { return _type_trade_is_money; }
	void					setTypeTradeIsMoney(bool val) { _type_trade_is_money = val; }
	string					getParams() const { return _params; }
	void					setParams(string val) { _params = val; }
	EClientConfirmProgress	getClientProgress() const { return _cur_client_progress; }
	void					setClientProgress(EClientConfirmProgress val) { InterlockedExchange((LONG*)&_cur_client_progress,val); }
	int						getRetryTimes(){ return _retry_times; }
	void					setRetryTimes( int val ){ _retry_times = val;}
	string					getLog() const { return _log; }
	void					setLog(const string& val) { _log = val; }
protected:

private:
	int _game_type_id;
	int _dispatch_id;
	int _result_id;
	int _money_confirm;
	int _retry_times;
	string _order_id;
	string _account;
	string _password;
	string _role;
	string _world;
	string _log;
	string _params;
	DateTime _get_time;
	DateTime _dispatch_time;
	DateTime _complete_time;
	BOOL _isRemove;
	BOOL _is_log_to_text;
	EConfirmState _cur_state;
	EClientConfirmProgress _cur_client_progress;
	COrderRecorder* _recorder;
	CTradeClient* _client;
	bool _type_trade_is_money;
};