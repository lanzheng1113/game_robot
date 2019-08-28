/**
 * @file Trader.h
 * @author PV
 * @date 2013-1-1 00:00:00 
 * 所谓的交易执行体
 */
#pragma once
#include "noncopyable.h"
#include "../Common/shareStruct.h"
#include "../common/TradeMesgDef.h"

#include <boost/signal.hpp>
#include <boost/signals/trackable.hpp>

#include <string>
using std::string;
#include <list>
using std::list;

class CTradeSession;
class CLock_CS;

/**
 * @class CTrader
 * Exec系列函数是执行一系列外挂操作的哦，亲。
 * 所以请你把它完善吧。
 * 至于ThdFun_Test函数它是一个测试用线程
 * trige_test_fun用于模拟某一项操作完成，该操作完成后调用OnXXX函数。
 * OnXXX函数在你完成一项工作进度后调用它。
 * OnXXX系列函数纯粹是穿着蓑衣又打伞。这里只是为了测试方便，看它不爽就XX了它重写吧。
 */
class CTrader : public boost::signals::trackable
{
public:
	~CTrader();
public:
	int ExecLaunchGame(CTradeSession* session);			//< 执行CreateProcess等，然后返回。
	int ExecEnterRole(CTradeSession* session);			//< 执行登录操作，进入角色
	int ExecEnterTradeStage(CTradeSession* session);	//< 执行进入交易场景操作	
	int ExecReadyToTrade(CTradeSession* session);		//< 执行准备交易操作
	int ExecTrading(CTradeSession* session);			//< 执行交易操作(需要服务器核准)
	int ExecTradeEnd(CTradeSession* session);			//< 执行交易完成后的操作。比如记录日志，退出游戏以及外挂，等等。
	int ExecCancelTrade(CTradeSession* session,int resason);		//< 执行立即退出交易的命令

	int OnGameLaunched(int order_dispatch_id,int error_code);		//< 通知：该session的游戏开启的结果；
	int OnEnterRole(int order_dispatch_id,int error_code);		//< 通知，该session登录进入游戏角色的结果
	int OnEnterTradeStage(int order_dispatch_id,int error_code);	//< 通知：该session进入交易场景的结果
	int OnReadyToTrade(int order_dispatch_id,int error_code);		//< 通知：该session进入交易准备好状态
	int OnTrading(int order_dispatch_id,int error_code);										//< 通知：session进入交易状态
	int OnTradeFaile(int order_dispatch_id,int error_code);		//< 通知交易失败。
	int OnTradeSuccess(int order_dispatch_id);					//< 交易成功完成。此项故意与上一项分开。


	CTradeSession* getSession(int order_dispatch_id);//< get session object

	static CTrader & getInstance() {
		if (!_instance) {
			_instance = new CTrader;
		}
		return *_instance;
	}
protected:
	static unsigned int __stdcall ThdFun_Test(void* lpThis);
	void trige_test_fun();
public: //EVENT call back
	void OnSessionProgressChanged(CTradeSession* sender,EClientOrderProgress now,EClientOrderProgress prevous);//< 事件回调函数
private:

	CTrader();
	static CTrader * _instance;

	int ClearSession(CTradeSession* session);
	list<CTradeSession*> _sessions;
	CLock_CS* cs;			//< @remark: o(╯□╰)o,In fact it does not work.Cause I found all the funcs that may change the "_sessions" list were called in only a thread at last!
	unsigned long m_hThdSocketRecv;
};