#include "StdAfx.h"
#include "Trader.h"
#include "TradeSession.h"
#include "assert.h"
#include "../common/AutoLock.h"
#include "../common/DebugMessage.h"
#include <process.h>
#include "Logs.h"

CTrader* CTrader::_instance = NULL;

CTrader::CTrader()
{
	cs = new CLock_CS;

	m_hThdSocketRecv = (unsigned long)_beginthreadex(NULL,0,ThdFun_Test,this,0,NULL);
}

CTrader::~CTrader()
{
	if(cs)
		delete cs;
	cs = NULL;
}

int CTrader::ExecLaunchGame( CTradeSession* session )
{
	printf("session %s-%d Launchgame.\n",session->getOrderInfo().order_id,session->getOrderInfo().trade_role);
	//HANDLE hProcess;
	//CreateProcess(...)
	//do sth to notify  game execute.
	//..........
	//if notify failed,return 0;
	//return 0;

	//otherwise, return 1.
	return 1;
}

int CTrader::ExecEnterRole( CTradeSession* session )
{
	//PostThreadMessage,PostMessage... etc.
	//如果程序设置为是开启游戏后自动进入角色，自动进入交易场景，那么这些对应的Exec操作直接返回1即可。
	printf("%s\n",__FUNCTION__);
	return 1;
}

int CTrader::ExecEnterTradeStage( CTradeSession* session )
{
	//PostThreadMessage,PostMessage... etc.
	//如果程序设置为是开启游戏后自动进入角色，自动进入交易场景，那么这些对应的Exec操作直接返回1即可。
	printf("%s\n",__FUNCTION__);
	return 1;
}

int CTrader::ExecReadyToTrade( CTradeSession* session )
{
	//PostThreadMessage,PostMessage... etc.
	//如果程序设置为是开启游戏后自动进入角色，自动进入交易场景，那么这些对应的Exec操作直接返回1即可。
	printf("%s\n",__FUNCTION__);
	return 1;
}

int CTrader::ExecTrading( CTradeSession* session )
{
	//PostThreadMessage,PostMessage... etc.
	//交易需要服务器的批准，所以这个必须主动通知外挂相关进程的。
	printf("%s\n",__FUNCTION__);
	return 1;
}

int CTrader::ExecTradeEnd( CTradeSession* session )
{
	//PostThreadMessage,PostMessage... etc.
	//如果执行完交易后外挂控制游戏自动关闭，那么这个可以直接返回1；
	printf("%s\n",__FUNCTION__);
	return 1;
}

int CTrader::ExecCancelTrade( CTradeSession* session ,int resason)
{
	//1ST,Such as Terminate process,terminate thread. etc.
	printf("%s取消订单交易\n",__FUNCTION__);
	//2ND,Log the message of exit trade.
	//C_log("session - xxx has ben canceled");
	//
	//
	//3RD,This message send to itself but not send by demon thread(or process);
	OnTradeFaile(session->getOrderInfo().order_dispatch_id,
				resason);
	return 1;
}


int CTrader::OnGameLaunched( int order_dispatch_id,int error_code )
{
	printf("%s-%d\n",__FUNCTION__,order_dispatch_id);
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session){
		printf("not found -- %s-%d",session->getOrderInfo().order_id,order_dispatch_id);
		return 0;
	}

	if (error_code == CLIENT_OP_ERROR_CODE_SUCCESS){
		session->setProgressInfors(EOP_game_lunched);
	}else{
		session->setProgressInfors(EOP_trade_end,false,error_code);
	}
	return 1;
}

int CTrader::OnEnterRole( int order_dispatch_id,int error_code )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session)
	{
		printf("not found -- %s-%d",session->getOrderInfo().order_id,order_dispatch_id);
		return 0;
	}
	printf("%s-%s-%d-%d\n",__FUNCTION__,session->getOrderInfo().order_id,order_dispatch_id,error_code);

	if (error_code == CLIENT_OP_ERROR_CODE_SUCCESS)
	{
		session->setProgressInfors(EOP_in_game_role);
	}else{
		session->setProgressInfors(EOP_trade_end,false,error_code);
	}
	return 1;
}

int CTrader::OnEnterTradeStage( int order_dispatch_id,int error_code )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session)
	{
		return 0;
	}
	printf("%s-%s-%d\n",__FUNCTION__,session->getOrderInfo().order_id,order_dispatch_id);

	if (error_code == CLIENT_OP_ERROR_CODE_SUCCESS)
	{
		session->setProgressInfors(EOP_at_trade_stage);
	}else{
		session->setProgressInfors(EOP_trade_end,false,error_code);
	}
	return 1;
}

int CTrader::OnReadyToTrade( int order_dispatch_id,int error_code )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session)
	{
		return 0;
	}
	printf("%s-%s-%d\n",__FUNCTION__,session->getOrderInfo().order_id,order_dispatch_id);

	if (error_code == CLIENT_OP_ERROR_CODE_SUCCESS)
	{
		session->setProgressInfors(EOP_ready_to_trade);
	}else{
		session->setProgressInfors(EOP_trade_end,false,error_code);
	}
	return 1;
}

int CTrader::OnTrading( int order_dispatch_id,int error_code )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session)
	{
		return 0;
	}
	printf("%s-%s-%d\n",__FUNCTION__,session->getOrderInfo().order_id,order_dispatch_id);
	if (error_code == CLIENT_OP_ERROR_CODE_SUCCESS)
	{
		session->setProgressInfors(EOP_trading);
	}else{
		session->setProgressInfors(EOP_trade_end,false,error_code);
	}
	return 1;
}

int CTrader::OnTradeFaile( int order_dispatch_id,int error_code )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session)
	{
		return 0;
	}

	printf("%s-%s-%d-%d\n",__FUNCTION__,session->getOrderInfo().order_id,order_dispatch_id,error_code);
	assert(error_code != CLIENT_OP_ERROR_CODE_SUCCESS);
	session->setProgressInfors(EOP_trade_end,false,error_code);
	return 1;
}

int CTrader::OnTradeSuccess( int order_dispatch_id )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (!session)
	{
		return 0;
	}

	printf("%s-%s-%d\n",__FUNCTION__,session->getOrderInfo().order_id,order_dispatch_id);

	session->setProgressInfors(EOP_trade_end);//trade success,yeah!
	return 1;
}

void CTrader::OnSessionProgressChanged( CTradeSession* sender,EClientOrderProgress now,EClientOrderProgress prevous )
{
	printf("%s-%s-%d\n",__FUNCTION__,sender->getOrderInfo().order_id,sender->getOrderInfo().trade_role);
	switch(now){
		case EOP_trade_end:
			{
				if (EOP_trade_end == now)
				{
					if (sender->getLastOperationResult())
					{
						//已经成功完成所有操作
						//@FIXME:在这里添加交易完成后对客户端的处理
						ExecTradeEnd(sender);
						ClearSession(sender);
						return;
					}else{
						//该订单发生了错误被取消了
						//@FIXME：在这里添加交易失败后对客户端的处理
						ExecTradeEnd(sender);
						ClearSession(sender);
						return;
					}
				}
				break;
			}
		case EOP_standby:
			{
				assert(0);//完全是莫名其妙的状态
				break;
			}

		case EOP_order_received:
			{
				{
					CAutoLock lock(*cs);
					if (find(_sessions.begin(),_sessions.end(),sender) == _sessions.end())
					{
						_sessions.push_back(sender);
						C_log("Insert order \'%s\' to session list!",sender->getOrderInfo().order_id);
					}
				}
				ExecLaunchGame(sender);
				break;
			}
		case EOP_game_lunched:
			{
				ExecEnterRole(sender);
				break;
			}
		case EOP_in_game_role:
			{
				ExecEnterTradeStage(sender);
				break;
			}
		case EOP_at_trade_stage:
			{
				ExecReadyToTrade(sender);
				break;
			}
		case EOP_ready_to_trade:
			{
				ExecTrading(sender);
				break;
			}
		case EOP_trading:
			{
				break;
			}
		default:
			break;
	}
}

CTradeSession* CTrader::getSession( int order_dispatch_id )
{
	{
		CAutoLock lock(*cs);
		for (list<CTradeSession*>::const_iterator it = _sessions.begin(); it != _sessions.end(); ++it){
			if ((*it)->getOrderInfo().order_dispatch_id == order_dispatch_id){
				return *it;
			}
		}
	}
	return NULL;
}

//< @remark:the session remove from the list(in CTrader::_sessions) here.
int CTrader::ClearSession( CTradeSession* session )
{
	printf("%s\n",__FUNCTION__);
	{
	CAutoLock lock(*cs);	
	_sessions.remove(session);
	}
	printf("ClearSession Ret!\n");
	return 1;
}

unsigned int __stdcall CTrader::ThdFun_Test( void* lpThis )
{
	SetTimer(NULL,112,1000,NULL);
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		if (msg.message == WM_TIMER)
		{
			CTrader::getInstance().trige_test_fun();
			continue;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 1;
}

//< @remark: this is a roll function object.
//< the _roll_base is rand value interval as 1~_roll_base
//< the param addbase is basevale,then the ret value is "addbase+[a value between 0~(_roll_base-1)]"
struct roll 
{
public:
	roll(unsigned long roll_base){srand(GetTickCount());_roll_base = roll_base;}
	unsigned long operator()(int addbase){
		return addbase+rand()%_roll_base;
	}
private:
	unsigned long _roll_base;
};

//< @ramark
//< @warning: this function is a test function,do not use this in yr program.
void CTrader::trige_test_fun()
{
	CAutoLock lock(*cs);
	static bool bini = false;
	if (!bini)
	{
		srand(GetTickCount());
	}

	int size = _sessions.size();
	if (!size)
	{
		printf("    No Sessions to test!!!");
		return ;
	}

	int index = rand()%size;
	int i = 0;
	printf("test fun test index=%d\n",index);
	for (list<CTradeSession*>::const_iterator it = _sessions.begin(); it != _sessions.end(); ++it)
	{
		if (i++ != index)
		{
			continue;
		}
		
		printf("test fun test order=%s-%d\n",(*it)->getOrderInfo().order_id,(*it)->getOrderInfo().trade_role);
		CTradeSession* session = *it;
		int order_dispatch_id = session->getOrderInfo().order_dispatch_id;
		switch(session->getProgress()){
			case EOP_standby:					//待命，准备接收账号密码
				printf("test fun test:do EOP_standby\n");
				break;
			case EOP_order_received:			//客户端收到了账号，密码等信息
				printf("test fun test:do EOP_order_received\n");
				OnGameLaunched(order_dispatch_id,CLIENT_OP_ERROR_CODE_SUCCESS);
// 				switch(roll(3)(0)){
// 					case 0:
// 						OnGameLaunched(orderId,sellOrBuyer,CLIENT_OP_ERROR_CODE_SUCCESS);
// 						break;
// 					case 1:
// 						OnGameLaunched(orderId,sellOrBuyer,ECC_LOGIN_SERVER_ERROR);
// 						break;
// 					case 2:
// 						OnGameLaunched(orderId,sellOrBuyer,ECC_GAME_COLLAPSE);
// 						break;
// 					default:
// 						OnGameLaunched(orderId,sellOrBuyer,roll(100)(0x800));	// OK,assume we has a accident,and the err code is 800~899
// 						break;
// 				}
				break;
			case EOP_game_lunched:
				{
					printf("test fun test:do EOP_game_lunched\n");
					OnEnterRole(order_dispatch_id,CLIENT_OP_ERROR_CODE_SUCCESS);
// 					switch(roll(4)(0)){
// 						case 0:
// 							OnEnterRole(orderId,sellOrBuyer,CLIENT_OP_ERROR_CODE_SUCCESS);
// 							break;
// 						case 1:
// 							OnEnterRole(orderId,sellOrBuyer,ECC_GAME_COLLAPSE);
// 							break;
// 						case 2:
// 							OnEnterRole(orderId,sellOrBuyer,ECC_ACC_OR_PSWD_INCORRECT);
// 							break;
// 						case 3:
// 							OnEnterRole(orderId,sellOrBuyer,ECC_ROLE_NOT_EXIST);
// 							break;
// 						default:
// 							OnEnterRole(orderId,sellOrBuyer,roll(100)(0x800));	// OK,assume we has a accident,and the err code is 800~899
// 							break;
// 					}
					break;
				}

			case EOP_in_game_role://客户端进入了游戏角色
				{
					printf("test fun test:do EOP_in_game_role\n");
					OnEnterTradeStage(order_dispatch_id,CLIENT_OP_ERROR_CODE_SUCCESS);
// 					switch(roll(4)(0)){
// 						case 0:
// 							OnEnterTradeStage(orderId,sellOrBuyer,CLIENT_OP_ERROR_CODE_SUCCESS);
// 							break;
// 						case 1:
// 							OnEnterTradeStage(orderId,sellOrBuyer,ECC_GAME_COLLAPSE);
// 							break;
// 						case 2:
// 							OnEnterTradeStage(orderId,sellOrBuyer,ECC_CANNOT_ENTER_TRADE_STAGE_DANGE);
// 							break;
// 						case 3:
// 							OnEnterTradeStage(orderId,sellOrBuyer,ECC_CANNOT_ENTER_TRADE_STAGE_OTHER);
// 							break;
// 						default:
// 							OnEnterTradeStage(orderId,sellOrBuyer,roll(100)(0x800));	// OK,assume we has a accident,and the err code is 800~899
// 							break;
// 					}
					//OnEnterTradeStage(orderId,sellOrBuyer,)
					break;
				}
			case EOP_at_trade_stage:			//角色进入了指定场景
				printf("test fun test:do EOP_at_trade_stage\n");
				OnReadyToTrade(order_dispatch_id,CLIENT_OP_ERROR_CODE_SUCCESS);
// 				switch(roll(4)(0)){
// 					case 0:
// 						OnReadyToTrade(orderId,sellOrBuyer,CLIENT_OP_ERROR_CODE_SUCCESS);
// 						break;
// 					case 1:
// 						OnReadyToTrade(orderId,sellOrBuyer,ECC_GAME_COLLAPSE);
// 						break;
// 					case 2:
// 						OnReadyToTrade(orderId,sellOrBuyer,ECC_BUYER_MONEY_NOT_ENOUGH);
// 						break;
// 					case 3:
// 						OnReadyToTrade(orderId,sellOrBuyer,ECC_SELLER_HAS_NO_WAIR);
// 						break;
// 					default:
// 						OnReadyToTrade(orderId,sellOrBuyer,roll(100)(0x800));
// 						break;
// 				}
				break;
			case EOP_ready_to_trade:			//客户端已经准备好交易
				OnTrading(order_dispatch_id,CLIENT_OP_ERROR_CODE_SUCCESS);
				break;
			case EOP_trading:					//正在交易
				printf("test fun test:do EOP_trading\n");
				switch(roll(2)(0)){
					case 0:
						OnTradeSuccess(order_dispatch_id);
						break;
					case 1:
						OnTradeFaile(order_dispatch_id,roll(100)(0x800));//< @remark: In fact I wonder has there any accidents to cause a failue when both of them had pushed "trade" button in games.
						break;
					default:
						break;
				}
				break;
			case EOP_trade_end:					//交易完成
				printf("test fun test:do EOP_trade_end\n");
				break;
			default:
				break;
		}
		break;
	}
}





