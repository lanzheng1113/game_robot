#include "stdafx.h"
#include "TradeAffair.h"
#include "DBM.h"
#include "../common/crypt.h"
#include "../common/DebugMessage.h"
#include "../common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include "TradeAffairMgr.h"
#include <Libutil/util/Logger.h>
#include "TradeMacDispatcher.h"
#include "tradeClientMachine.h"
#include "../IOCPServer/NetworkCallBack.h"
#include "ObjectFactory.h"
#include "Settings.h"
#include "TradeOrder.h"
#include "ConfirmOrder.h"
#include "util/AutoBuffer.h"

#define EXTRA_BUFFER_DEFAULT_SIZE (1024)

CTradeAffair::CTradeAffair(CTradeAffairMgr* afrMgr,CTradeMacDispatcher* macDisp,COrderDispatcher* orderDisp)
:affair()
,m_pExtraRecvBuffer(NULL)
,m_pExtraSendBuffer(NULL)
,m_ClientMachine(NULL)
{
	m_pMgr = afrMgr;
	m_macDispather = macDisp;
	m_orderDispatcher = orderDisp;

	m_bShouldRemoved = false;
	m_pExtraRecvBuffer = new BYTE[EXTRA_BUFFER_DEFAULT_SIZE];
	m_pExtraSendBuffer = new BYTE[EXTRA_BUFFER_DEFAULT_SIZE];
	m_ExtraBufferMaxSize = EXTRA_BUFFER_DEFAULT_SIZE;
	m_MaxLenPerSend = 16000;
}

CTradeAffair::~CTradeAffair()
{
	DebugMessageA("%s",__FUNCTION__);
	m_pMgr = NULL;
	//safe delete.
	if (m_pExtraRecvBuffer)
	{
		delete [] m_pExtraRecvBuffer;
		m_pExtraRecvBuffer = NULL;
	}
	
	if (m_pExtraSendBuffer)
	{
		delete [] m_pExtraSendBuffer;
		m_pExtraSendBuffer = NULL;
	}

	if (m_ClientMachine)
	{
		m_ClientMachine->setMacState(CTradeClient::EClientState_disconnected);
		m_ClientMachine->setNetwork(NULL);
		//delete m_ClientMachine;
		m_ClientMachine = NULL;
	}
}

VOID CTradeAffair::OnSessionClosed()
{
	if (m_ClientMachine){
		m_ClientMachine->setMacState(CTradeClient::EClientState_disconnected);
		m_ClientMachine->setNetwork(NULL);
		m_ClientMachine->OnDisconnect();
		DebugMessageA("client %d has disconnected",m_ClientMachine->getMachineId());
	}
	remove();		//set remove 标记，当affair manager读取标志true时将删除它。
}

VOID CTradeAffair::OnDataRecv( BYTE *pMsg, WORD wSize )
{
	NET_HEAD* head = (NET_HEAD*)pMsg;
	CString strTableName;
	BYTE bufferToSend[1024] = {0};
	memset(bufferToSend,0,sizeof(bufferToSend));

	switch (head->MesgFullType)
	{
	case MSG_TYPE_LOGIN:
		{
			//DebugMessageA("MSG_TYPE_LOGIN! rcv=%d,need=%d",wSize,sizeof(PACKETGENERALHEAD)+sizeof(LOGIN_MESSAGE));
			if (wSize != sizeof(NET_HEAD)+sizeof(LOGIN_MESSAGE)){
				CloseSession();
				return;
			}

			LOGIN_MESSAGE msg;
			memcpy(&msg,pMsg+sizeof(NET_HEAD),sizeof(LOGIN_MESSAGE));

			OnMacLogin(head->GameType,msg.client_mac_id,msg.szName);
			break;
		}
	case MSG_TYPE_LOGOUT:
		{
			ACK_NET_HEAD ack;
			ack.AckMesgFullType = 2;
			Send((BYTE*)&ack,sizeof(ack));
			DebugMessageA("MSG_TYPE_LOGOUT!");
			break;
		}
	case MSG_TYPE_ACQUARE_ORDER:
		{
			if (wSize != sizeof(NET_HEAD) + sizeof(ACQUIRE_ORDER_MSG))
			{
				LOG_ERROR("错误！MSG_TYPE_ACQUARE_ORDER消息错误的大小.");
				CloseSession();
				return;
			}
			ACQUIRE_ORDER_MSG* pm = (ACQUIRE_ORDER_MSG*)(pMsg + sizeof(NET_HEAD));
			if (!m_ClientMachine)
			{
				CloseSession();
				LOG_ERROR("MSG_TYPE_ACQUARE_ORDER错误！CTradeAffair的m_ClientMachine为空！");
			}

			if(m_ClientMachine->getGameTypeId() != head->GameType){
				CloseSession();
				LOG_ERROR("错误！MSG_TYPE_ACQUARE_ORDER消息的GameType与当前CTradeAffair对象的m_ClientMachine.GameTypeId不同");
			}
			OnAcqureOrder(pm->burden);
			break;
		}
	case EMSG_CLIENT_CANCEL_ALL_OK:
		{
			//@FIXME:在这里添加处理客户端已经取消所有订单的消息的函数
			//@remark 这个消息附带在最后时刻完成了交易的订单
			//处理完这个消息后关闭连接。
			CloseSession();
			break;
		}
	case EMSG_CONFIRM_STATE_CHANGED:
		{
			if (wSize != sizeof(NET_HEAD) + sizeof(CONFIRM_STATE_CHANGED_MSG))
			{
				LOG_ERROR("EMSG_CONFIRM_STATE_CHANGED消息错误的大小");
				CloseSession();
				break;
			}
			if (!m_ClientMachine)
			{
				CloseSession();
				LOG_ERROR("EMSG_CONFIRM_STATE_CHANGED错误，CTradeAffair->client = NULL.");
				break;
			}
			if (m_ClientMachine->getGameTypeId() != head->GameType)
			{
				CloseSession();
				LOG_ERROR("EMSG_CONFIRM_STATE_CHANGED错误！消息的gameType与当前affir的client.gameTypeId不同！");
				break;
			}

			CONFIRM_STATE_CHANGED_MSG* msg = (CONFIRM_STATE_CHANGED_MSG*)(pMsg+sizeof(NET_HEAD));
			m_ClientMachine->ProgressConfirmStateChanged(msg->confirm_dispatch_id,(EClientConfirmProgress)msg->state_code,msg->result_id);
			break;
		}
	case EMSG_MAC_STATE_CHANGE:
		{
			if (wSize != sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG)){
				LOG_ERROR("EMSG_MAC_STATE_CHANGE消息错误的大小.");
				CloseSession();
				break;
			}

			if (!m_ClientMachine)
			{
				CloseSession();
				LOG_ERROR("EMSG_MAC_STATE_CHANGE错误！CTradeAffair的m_ClientMachine为空！");
				break;
			}

			if(m_ClientMachine->getGameTypeId() != head->GameType){
				CloseSession();
				LOG_ERROR("EMSG_MAC_STATE_CHANGE错误！消息的GameType与当前CTradeAffair对象的m_ClientMachine.GameTypeId不同");
				break;
			}

			TRADE_STATE_CHANGED_MSG* trade_state_chage = (TRADE_STATE_CHANGED_MSG*)(pMsg + sizeof(NET_HEAD));

			m_ClientMachine->ProgressTradeStateChanged(trade_state_chage->order_dispatch_id,
													(EClientOrderProgress)(trade_state_chage->state_code),
													trade_state_chage->result_id);

			break;
		}
	case EMSG_HEART:
		{
			if (wSize != sizeof(NET_HEAD)+sizeof(HEART_MSG)){
				LOG_ERROR("bad message len for emsg_heart.");
				CloseSession();
			}
			else{
				HEART_MSG* msg = (HEART_MSG *)(pMsg + sizeof(NET_HEAD));
				m_ClientMachine->setMacBurden(msg->burden);
			}
			break;
		}
	default:
		LOG_ERROR("unknow packet!");
		*(DWORD*)bufferToSend = msg_error_unknow_command;
		Send(bufferToSend,4);
		break;
	}
}

void CTradeAffair::doHeart()
{

// 	ACKPACKETHEAD head;
// 	memset(&head,0,sizeof(head));
// 	head.AckMesgFullType = msg_Proxy_heart;
// 	head.GameType = _GAME_TYPE_DNF;
// 	head.GameVersion = 0;
// 	head.LastPacket = 1;
// 	head.WgVersion = 0;
// 	head.DataSizeRemain = 0;
// 	Send((BYTE*)&head,sizeof(head));
// 	return;
}

void CTradeAffair::ResetExtraSendBuffer()
{
	m_CurrentExtraBufferLen = 0;
}

void CTradeAffair::doMoreData()
{
	ACK_NET_HEAD ackHead;
	BYTE* pCurSendBuffer = new BYTE [m_MaxLenPerSend];
	
	memcpy(&ackHead,m_pExtraSendBuffer,sizeof(ackHead));
	
	int datalen = 0;
	if (m_CurrentExtraBufferLen<m_MaxLenPerSend-sizeof(ackHead))
	{
		ackHead.LastPacket = 1;
		datalen = m_CurrentExtraBufferLen;
	}
	else{
		ackHead.LastPacket = 0;
		datalen = m_MaxLenPerSend-sizeof(ackHead);
	}

	ackHead.DataSizeRemain = m_CurrentExtraBufferLen - datalen;

	memcpy(pCurSendBuffer,&ackHead,sizeof(ackHead));
	memcpy(pCurSendBuffer+sizeof(ackHead),m_pExtraSendBufferCurrentPos,datalen);
	
	m_pExtraSendBufferCurrentPos += datalen;
	m_CurrentExtraBufferLen -= datalen;

	Send(pCurSendBuffer,datalen+sizeof(ackHead));
	DebugMessageA("Send doMoreData %d bytes",datalen+sizeof(ackHead));
	delete [] pCurSendBuffer;
}

void CTradeAffair::PreHugeDataSend(int dataSize )
{
	if (m_ExtraBufferMaxSize < dataSize)
	{	//内存太少，重新分配内存
		delete [] m_pExtraSendBuffer;
		m_ExtraBufferMaxSize = 0;

		m_pExtraSendBuffer = new BYTE [dataSize];
		m_ExtraBufferMaxSize = dataSize;
		m_CurrentExtraBufferLen = 0;
	}
}

void CTradeAffair::HugeDataSend(ACK_NET_HEAD& ackHead)
{
	//本次发送后剩余的数据量
	ackHead.DataSizeRemain = m_CurrentExtraBufferLen - m_MaxLenPerSend < 0 ? 0:(m_CurrentExtraBufferLen - m_MaxLenPerSend);

	if(m_MaxLenPerSend > m_CurrentExtraBufferLen){
		ackHead.LastPacket = 1;	//这是最后一个包。
		memcpy(m_pExtraSendBuffer,&ackHead,sizeof(ackHead));
		Send(m_pExtraSendBuffer,m_CurrentExtraBufferLen);
		DebugMessageA("SEND PACKET %d BYTES",m_CurrentExtraBufferLen);
		m_CurrentExtraBufferLen = 0;
		m_pExtraSendBufferCurrentPos = m_pExtraSendBuffer;
	}
	else{
		ackHead.LastPacket = 0; //这不是最后一个包。
		memcpy(m_pExtraSendBuffer,&ackHead,sizeof(ackHead));
		Send(m_pExtraSendBuffer,m_MaxLenPerSend);
		DebugMessageA("SEND PACKET %d BYTES,toal count %d BYTES",m_MaxLenPerSend,m_CurrentExtraBufferLen);
		m_CurrentExtraBufferLen = m_CurrentExtraBufferLen - m_MaxLenPerSend;
		m_pExtraSendBufferCurrentPos = m_pExtraSendBuffer + m_MaxLenPerSend;
	}
}

void CTradeAffair::OnMacLogin( int game_id, int c_id, const string& name )
{
	if (m_ClientMachine)
	{
		CloseSession();
		return;
	}

	/*@FIXME:在这里添加身份验证信息的代码*/
	if(!CheckMacAur(game_id,c_id,name)){
		CloseSession();
		return;
	}

	string ip = GetNetworkCallback()->GetIP();
	
	CTradeClient* has_mac = m_macDispather->getMac(game_id,c_id,name);
	if (has_mac != NULL)
	{
		if (has_mac->getMacState() == CTradeClient::EClientState_disconnected)
		{
			has_mac->setIp(ip);
			has_mac->setNetwork(this);
			has_mac->setMacState(CTradeClient::EClientState_connected_busy);
			m_ClientMachine = has_mac;
			DebugMessageA("机器id=%d的客户端已经重新连接！",c_id);
		}else{
			DebugMessageA("拒绝重复登录！");
			CloseSession();
			return;
		}
	}else{
		CTradeClient* pMac = new CTradeClient(game_id,name,c_id,ip,getDefaultGameOrderMax(game_id),this);
		if (!pMac){
			CloseSession();
			return;
		}

		m_ClientMachine = pMac;
		if(!m_macDispather->addMac(game_id,pMac)){
			CloseSession();
			return;
		}

	}

	//DebugMessageA("####dump machine %s",m_ClientMachine->dump().c_str());

	//DebugMessageA("####dump dispather:%s",m_macDispather->dump().c_str());

	ACK_NET_HEAD ack;
	ack.DataSizeRemain = 0;
	ack.GameType = game_id;
	ack.LastPacket = 1;
	ack.AckMesgFullType = MSG_TYPE_LOGIN;
	Send((BYTE*)&ack,sizeof(ack));
	return;
}


void CTradeAffair::OnAcqureOrder( int burden )
{
	int iret = m_macDispather->DealTradeRequest(m_ClientMachine,burden);

	if(	EDE_DISPATCH_SUCCESS != (EDispatchError)iret)
		PM_AssignOrderFailed((EDispatchError)iret);
}


int CTradeAffair::getDefaultGameOrderMax( int game_id )
{
	/*@FIXME:从配置中读取每个游戏允许的多开数量最大值*/
	return CSettings::getInstance().getSettingDefaultMaxOrderCount(game_id);
}

bool CTradeAffair::CheckMacAur( int game_id, int c_id, const string& name )
{
	/*@FIXME:添加对登录机器的身份验证*/
	return true;
}

BOOL CTradeAffair::PM_AssignConfirm( order_struct* order )
{
	ASSIGN_CONFIRM_MSG msg;
	msg.os = *order;

	ACK_NET_HEAD head;

	head.GameType = order->game_type;
	head.DataSizeRemain = 0;
	head.AckMesgFullType = EMSG_DISPATCH_CONFIRM_SUCCESS;
	head.LastPacket = 1;

	CAutoBuffer<BYTE> send_buffer(sizeof(head)+sizeof(msg));
	send_buffer.write((BYTE*)&head,sizeof(head));
	send_buffer.write((BYTE*)&msg,sizeof(msg),sizeof(head));
	return Send(send_buffer.getPtr(),sizeof(head)+sizeof(msg));
}


BOOL CTradeAffair::PM_AssignOrder( order_struct* order)
{
	ASSIGN_ORDER_MSG msg;
	msg.os = *order;

	ACK_NET_HEAD head;
	
	head.GameType = order->game_type;
	head.DataSizeRemain = 0;
	head.AckMesgFullType = EMSG_DISPATCH_SUCCESS;
	head.LastPacket = 1;

	char sendbuffer[2048] = {0};
	ASSERT(sizeof(head)+sizeof(msg) < sizeof(sendbuffer));
	
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	return Send((BYTE*)sendbuffer,sizeof(head)+sizeof(msg));
}

void CTradeAffair::PM_AssignOrderFailed( EDispatchError errCode )
{
	if (!m_ClientMachine)
	{
		return;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}

	ACK_NET_HEAD head;
	head.AckMesgFullType = EMSG_DISPATCH_FAILED;

	ASSIGN_ORDER_FAILED_MSG failed_msg;
	switch (errCode)
	{
	case EDE_DISPATCH_SUCCESS:
		ASSERT(0);
		return;
	case EDE_DISPATCH_ERROR_GAME_NOT_FOUND:
		failed_msg.error_code = EMSG_DISPATCH_ERROR_GAME_NOT_FOUND; 
		break;
	case EDE_DISPATCH_ERROR_NOT_FOUND_BUDDY:
		failed_msg.error_code = EMSG_DISPATCH_ERROR_NOT_FOUND_BUDDY;
		break;
	case EDE_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN:
		failed_msg.error_code = EMSG_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN;
		break;
	case EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR:
	case EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_BUYER_ERROR:
		return;
	default:
		return;
	}

	head.DataSizeRemain = 0;
	head.GameType = m_ClientMachine->getGameTypeId();
	head.LastPacket = 1;

	BYTE sendbuffer[1024] = {0};
	ASSERT(sizeof(head)+sizeof(failed_msg) < sizeof(sendbuffer));
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&failed_msg,sizeof(failed_msg));

	BOOL bsend = Send(sendbuffer,sizeof(head)+sizeof(failed_msg));
	if (!bsend)
	{
		//@FIXME:在这里添加发送配送订单错误的处理代码。
	}
}

void CTradeAffair::PM_ConfirmHasGetOrder_X( int order_dispatch_id,int error_code )
{
	if (!m_ClientMachine)
	{
		return;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}

	ACK_NET_HEAD ackHead;
	ackHead.AckMesgFullType = EMSG_ACK_CONFIRM_HAS_GET_ORDER_X;
	ackHead.DataSizeRemain = 0;
	ackHead.GameType = m_ClientMachine->getGameTypeId();
	ackHead.LastPacket = 1;

	ACK_COMFIRM_STATE_CHANGED msg;
	msg.order_dispatch_id = order_dispatch_id;
	msg.error_code = error_code;

	CAutoBuffer<BYTE> buffer(sizeof(ackHead)+sizeof(msg));
	buffer.write((BYTE*)&ackHead,sizeof(ackHead));
	buffer.write((BYTE*)&msg,sizeof(msg),sizeof(ackHead));
	Send(buffer.getPtr(),sizeof(msg)+sizeof(ackHead));
}


void CTradeAffair::PM_ConfirmStateChangedAckMsg( int order_dispatch_id,int error_code )
{
	if (!m_ClientMachine)
	{
		return;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}

	ACK_NET_HEAD ackHead;
	ackHead.AckMesgFullType = EMSG_ACK_CONFIRM_STATE_CHANGED;
	ackHead.DataSizeRemain = 0;
	ackHead.GameType = m_ClientMachine->getGameTypeId();
	ackHead.LastPacket = 1;

	ACK_COMFIRM_STATE_CHANGED msg;
	msg.order_dispatch_id = order_dispatch_id;
	msg.error_code = error_code;

	CAutoBuffer<BYTE> buffer(sizeof(ackHead)+sizeof(msg));
	buffer.write((BYTE*)&ackHead,sizeof(ackHead));
	buffer.write((BYTE*)&msg,sizeof(msg),sizeof(ackHead));
	Send(buffer.getPtr(),sizeof(msg)+sizeof(ackHead));
}

void CTradeAffair::PM_StateChangedAck_HasGetOrder_X( int order_dispatch_id,int error_code )
{
	if (!m_ClientMachine)
	{
		return;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}

	ACK_NET_HEAD ackHead;
	ackHead.AckMesgFullType = EMSG_ACK_TRADE_HAS_GET_ORDER_X;
	ackHead.DataSizeRemain = 0;
	ackHead.GameType = m_ClientMachine->getGameTypeId();
	ackHead.LastPacket = 1;

	ACK_TRADE_STATE_CHANGED msg;
	msg.order_dispatch_id = order_dispatch_id;
	msg.error_code = error_code;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&ackHead,sizeof(ackHead));
	memcpy(sendbuffer + sizeof(ackHead),&msg,sizeof(msg));

	Send((BYTE*)sendbuffer,sizeof(ACK_NET_HEAD)+sizeof(msg));
}

void CTradeAffair::PM_StateChangedAckMsg( int order_dispatch_id,int error_code )
{
	if (!m_ClientMachine)
	{
		return;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}

	ACK_NET_HEAD ackHead;
	ackHead.AckMesgFullType = EMSG_ORDER_STATE_CHANGED_ACK;
	ackHead.DataSizeRemain = 0;
	ackHead.GameType = m_ClientMachine->getGameTypeId();
	ackHead.LastPacket = 1;

	ACK_TRADE_STATE_CHANGED msg;
	msg.order_dispatch_id = order_dispatch_id;
	msg.error_code = error_code;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&ackHead,sizeof(ackHead));
	memcpy(sendbuffer + sizeof(ackHead),&msg,sizeof(msg));
	
	Send((BYTE*)sendbuffer,sizeof(ACK_NET_HEAD)+sizeof(msg));
}

void CTradeAffair::PM_CancelAllConfirms()
{
	if (!m_ClientMachine)
	{
		return;
	}
	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}
	ACK_NET_HEAD head;
	head.AckMesgFullType = EMSG_CANCAL_ALL_CONFIRM_ORDERS;
	head.DataSizeRemain = 0;
	head.LastPacket = 1;
	head.GameType = m_ClientMachine->getGameTypeId();

	CAutoBuffer<BYTE> ubf(sizeof(head));
	ubf.write((BYTE*)&head,sizeof(head));
	Send(ubf.getPtr(),sizeof(head));
}

BOOL CTradeAffair::PM_CancelAllOrders()
{
	if (!m_ClientMachine)
	{
		return FALSE;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return FALSE;
	}

	ACK_NET_HEAD head;
	head.AckMesgFullType = EMSG_CANCEL_ALL_ORDERS;
	head.DataSizeRemain = 0;
	head.LastPacket = 1;
	head.GameType = m_ClientMachine->getGameTypeId();

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	return Send((BYTE*)sendbuffer,sizeof(ACK_NET_HEAD));
}

void CTradeAffair::KillSession()
{
	CloseSession();
}

void CTradeAffair::PM_MyBuddyFailed( int order_dispatch_id,short buddy_error_code )
{
	if (!m_ClientMachine)
	{
		return;
	}

	if (m_ClientMachine->getMacState() == CTradeClient::EClientState_disconnected)
	{
		return;
	}

	ACK_NET_HEAD head;
	head.AckMesgFullType = EMSG_BUDDY_FAILED;
	head.DataSizeRemain = 0;
	head.LastPacket = 1;
	head.GameType = m_ClientMachine->getGameTypeId();

	ACK_BUDDY_FAILED packet;
	packet.buddy_failed_code = buddy_error_code;
	packet.order_dispatch_id = order_dispatch_id;
	packet.reserve = 0;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&packet,sizeof(packet));
	Send((BYTE*)sendbuffer,sizeof(head)+sizeof(packet));
}


