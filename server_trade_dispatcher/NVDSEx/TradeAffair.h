#ifndef _ASSIST_AFFAIR_H
#define _ASSIST_AFFAIR_H
#include <string>
using std::string;
#include "affair.h"
#include "../Common/shareStruct.h"
#include "TradeNetworkInterface.h"

class CTDatabaseObject;
class CTradeAffairMgr;
class CTradeMacDispatcher;
class COrderDispatcher;
class CTradeClient;

/**
 * @class CTradeAffair 
 * ������TCP�ػ��ϵ�һ������ͨ��һ��TCP���ӽ���һ��TCP�Ự����һ��TCP�Ự�Ͽ���ӵ�ж������
 */
#define ECallBack
class CTradeAffair : public affair , public CTradeNetworkInterface
{
public:
	CTradeAffair(CTradeAffairMgr* afrMgr,CTradeMacDispatcher* macDisp,COrderDispatcher* orderDisp);
	~CTradeAffair();

public:
	VOID	OnSessionClosed();
	VOID	OnDataRecv( BYTE *pMsg, WORD wSize );

	void remove(){m_bShouldRemoved = true;}
	bool GetShouldRemove(){return m_bShouldRemoved;}
public:
	/*######����������ʹ�õ�CTradeNetworkInterface�ӿ�######*/
	BOOL PM_AssignOrder(order_struct* order); //< @see CTradeNetworkInterface
	void PM_AssignOrderFailed(EDispatchError errCode);	//< @see CTradeNetworkInterface
	BOOL PM_CancelAllOrders();			//< @see CTradeNetworkInterface
	void PM_StateChangedAckMsg(int order_dispatch_id,int error_code);//< @see CTradeNetworkInterface
	void PM_StateChangedAck_HasGetOrder_X(int order_dispatch_id,int error_code);
	void KillSession();					//< @see CTradeNetworkInterface
	void PM_MyBuddyFailed(int order_dispatch_id,short buddy_error_code);//< @see CTradeNetworkInterface
	
	/**/
	BOOL PM_AssignConfirm(order_struct* order);
	void PM_ConfirmStateChangedAckMsg(int order_dispatch_id,int error_code);
	void PM_ConfirmHasGetOrder_X(int order_dispatch_id,int error_code);
	void PM_CancelAllConfirms();
private:
	/*���ദ��Recv��Ϣ�ĺ���*/
	void OnAcqureOrder(int burden);
	void OnMacLogin(int game_id, int c_id, const string& name);
private:
	void doHeart();
	void doMoreData();
	void ResetExtraSendBuffer();

	void PreHugeDataSend(int dataSize);
	void HugeDataSend(ACK_NET_HEAD& ackHead);
	int getDefaultGameOrderMax(int game_id);

	bool CheckMacAur( int game_id, int c_id, const string& name );
private:
	CTradeClient* m_ClientMachine;
	CTradeAffairMgr* m_pMgr;
	bool m_bShouldRemoved;

	BYTE* m_pExtraRecvBuffer;
	BYTE* m_pExtraSendBuffer;
	BYTE* m_pExtraSendBufferCurrentPos;
	
	int m_ExtraBufferMaxSize;
	int m_CurrentExtraBufferLen;

	int m_MaxLenPerSend;

	CTradeMacDispatcher* m_macDispather;
	COrderDispatcher* m_orderDispatcher;
};
#endif