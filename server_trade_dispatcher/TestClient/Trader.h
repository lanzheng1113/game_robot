/**
 * @file Trader.h
 * @author PV
 * @date 2013-1-1 00:00:00 
 * ��ν�Ľ���ִ����
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
 * Execϵ�к�����ִ��һϵ����Ҳ�����Ŷ���ס�
 * ��������������ưɡ�
 * ����ThdFun_Test��������һ���������߳�
 * trige_test_fun����ģ��ĳһ�������ɣ��ò�����ɺ����OnXXX������
 * OnXXX�����������һ������Ⱥ��������
 * OnXXXϵ�к��������Ǵ��������ִ�ɡ������ֻ��Ϊ�˲��Է��㣬������ˬ��XX������д�ɡ�
 */
class CTrader : public boost::signals::trackable
{
public:
	~CTrader();
public:
	int ExecLaunchGame(CTradeSession* session);			//< ִ��CreateProcess�ȣ�Ȼ�󷵻ء�
	int ExecEnterRole(CTradeSession* session);			//< ִ�е�¼�����������ɫ
	int ExecEnterTradeStage(CTradeSession* session);	//< ִ�н��뽻�׳�������	
	int ExecReadyToTrade(CTradeSession* session);		//< ִ��׼�����ײ���
	int ExecTrading(CTradeSession* session);			//< ִ�н��ײ���(��Ҫ��������׼)
	int ExecTradeEnd(CTradeSession* session);			//< ִ�н�����ɺ�Ĳ����������¼��־���˳���Ϸ�Լ���ң��ȵȡ�
	int ExecCancelTrade(CTradeSession* session,int resason);		//< ִ�������˳����׵�����

	int OnGameLaunched(int order_dispatch_id,int error_code);		//< ֪ͨ����session����Ϸ�����Ľ����
	int OnEnterRole(int order_dispatch_id,int error_code);		//< ֪ͨ����session��¼������Ϸ��ɫ�Ľ��
	int OnEnterTradeStage(int order_dispatch_id,int error_code);	//< ֪ͨ����session���뽻�׳����Ľ��
	int OnReadyToTrade(int order_dispatch_id,int error_code);		//< ֪ͨ����session���뽻��׼����״̬
	int OnTrading(int order_dispatch_id,int error_code);										//< ֪ͨ��session���뽻��״̬
	int OnTradeFaile(int order_dispatch_id,int error_code);		//< ֪ͨ����ʧ�ܡ�
	int OnTradeSuccess(int order_dispatch_id);					//< ���׳ɹ���ɡ������������һ��ֿ���


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
	void OnSessionProgressChanged(CTradeSession* sender,EClientOrderProgress now,EClientOrderProgress prevous);//< �¼��ص�����
private:

	CTrader();
	static CTrader * _instance;

	int ClearSession(CTradeSession* session);
	list<CTradeSession*> _sessions;
	CLock_CS* cs;			//< @remark: o(�s���t)o,In fact it does not work.Cause I found all the funcs that may change the "_sessions" list were called in only a thread at last!
	unsigned long m_hThdSocketRecv;
};