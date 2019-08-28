#pragma once

#include "NonCopyable.h"
#include "recvbuffer.h"
#include "../Common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include "NetworkMsgPoster.h"
#include <string>
using std::string;
#include <list>
using std::list;
class CTradeSession;
class CLock_CS;

class CTradeClientNetWork :
	public INetworkMsgPoster
{
public:
	CTradeClientNetWork(void);
	~CTradeClientNetWork(void);

	BOOL Go();

	void ShutDown();
	void setSocket(SOCKET val){m_socket = val;}
	BOOL isTimeOut();
	void ResetTimeOut();
	int	Send(IN const char FAR * buf, IN int len);

public:
	//imp INetworkMsgPoster;
	bool PM_HasGetOrder(int order_dispatch_id);					//< �����������ȷ����Ϣ�����Ѿ����յ��˶���������Ϊorder_id����Ϊ�Ľ�ɫ��is_as_seller?���۷�:����
	bool PM_GameLunched(int order_dispatch_id);					//< �ѳɹ�������Ϸ
	bool PM_EnterRole(int order_dispatch_id);					//< ���ѽ�����Ϸ��ɫ��
	bool PM_EnterTradeStage(int order_dispatch_id);				//< ���뽻�׵ص�
	bool PM_ReadyToTrade(int order_dispatch_id);				//< ׼���ý���
	bool PM_Trading(int order_dispatch_id);						//< ���ڽ���
	bool PM_TradeSuccess(int order_dispatch_id);				//<���׳ɹ�
	bool PM_TradeFailed(int order_dispatch_id,int fail_reason); //< ����ʧ��
	bool PM_INeedAOrder(int burden);														//< �����ڿ��У�����Ҫһ�������������ҵĸ�����Ϣ��
public:
	//packet receive deal with.
	void GM_GetOrder( const ASSIGN_ORDER_MSG& orderdata);
	void GM_GetOrderFailed( int reason );
	void GM_CancelAllOrders();
	void GM_MyBuddyIsDead(int order_dispatch_id,int failed_reasion);
	void MP_SendHeart();
	void OnSessionProgressChanged(CTradeSession* sender,EClientOrderProgress now,EClientOrderProgress previous);
	int getGameTypeId() const { return m_game_type_id; }
	void setGameTypeId(int val) { m_game_type_id = val; }
	//test
	void showMe();
	int getSessionCount(){return _sessions.size();}
protected:
	static unsigned int __stdcall ThdFun_SockRecv(void* lpThis);	
	static unsigned int __stdcall ThdFun_SocketHeart(void* lpThis);
private:
	void OnRecv(PBYTE Packet,WORD PacketLen);
	BOOL DoLogin(SOCKET sock);
	int getMyMacId();
	string getMyMacName();
	void Run();
	CTradeSession* getSession( int order_dispatch_id );
private:
	CLock_CS *cs;
	list<CTradeSession*> _sessions;
	DWORD m_lastCommtime;
	HANDLE m_hThdSocketRecv;
	HANDLE m_hThdSocketHeart;
	RecvBuffer* m_RecvBuffer;
	SOCKET m_socket;
	bool m_isSocket_valide;
	int m_game_type_id;
	unsigned long _last_time_send_request;
	volatile long m_is_idle_sended;
};
