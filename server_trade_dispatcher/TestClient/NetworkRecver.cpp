#include "StdAfx.h"
#include <string>
using std::string;
#include "winsock2.h"
#include "NetworkRecver.h"
#include "../common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include "AutoLock.h"
#include "process.h"
#include "StringEx.h"
#include "CSend.h"
#include "TradeSession.h"
#include "Trader.h"
#include <cassert>
#include "CpuUsage.h"
#include "Logs.h"

CTradeClientNetWork::CTradeClientNetWork(void)
{
	m_isSocket_valide = false;
	m_lastCommtime = GetTickCount();
	m_RecvBuffer = new RecvBuffer();
	m_RecvBuffer->Create(40*1024,40*1024);
	m_hThdSocketRecv = NULL;
	m_hThdSocketHeart = NULL;
	m_game_type_id = 10001;
	m_is_idle_sended = 0;
	cs = new CLock_CS;
}

CTradeClientNetWork::~CTradeClientNetWork(void)
{
	SAFE_DELETE(m_RecvBuffer);
	SAFE_DELETE(cs);
}

BOOL CTradeClientNetWork::Go()
{
	int iResult;
	WSADATA wsaData;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return FALSE;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("socket() failed with error: %ld\n", WSAGetLastError());
		return FALSE;
	}
	
	setSocket(sock);
	
	if(!DoLogin(sock))
		return FALSE;

	Run();

	return TRUE;
}

void CTradeClientNetWork::ShutDown()
{
	TerminateThread(m_hThdSocketHeart,0);
	TerminateThread(m_hThdSocketRecv,0);
	CloseHandle(m_hThdSocketRecv);
	CloseHandle(m_hThdSocketHeart);
	closesocket(m_socket);
	WSACleanup();
}

void CTradeClientNetWork::ResetTimeOut()
{
	m_lastCommtime = GetTickCount();
}

BOOL CTradeClientNetWork::isTimeOut()
{
	if (m_lastCommtime - GetTickCount() > 20*1000)
	{
		return TRUE;
	}else{
		return FALSE;
	}
}

int CTradeClientNetWork::Send( IN const char FAR * buf, IN int len )
{
	return CSend(m_socket,buf,len);
}


unsigned int  CTradeClientNetWork::ThdFun_SocketHeart( void* lpThis )
{
	CTradeClientNetWork* pNetworker = (CTradeClientNetWork*)lpThis;
	SetTimer(NULL,111,15000,NULL);
	
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		switch(msg.message){
			case WM_TIMER:
				{
					if (pNetworker->isTimeOut()){
						pNetworker->MP_SendHeart();
						//DebugMessageA("发送心跳！");
					}
					break;
				}
			default:
				break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void CTradeClientNetWork::Run()
{
	m_isSocket_valide = true;
	if (!m_hThdSocketRecv)
	{
		m_hThdSocketRecv = (HANDLE)_beginthreadex(NULL,0,ThdFun_SockRecv,this,0,NULL);
	}

	if (!m_hThdSocketHeart)
	{
		m_hThdSocketHeart = (HANDLE)_beginthreadex(NULL,0,ThdFun_SocketHeart,this,0,NULL);
	}
}

unsigned int CTradeClientNetWork::ThdFun_SockRecv( void* lpThis )
{
	CTradeClientNetWork* pMgr = (CTradeClientNetWork*)lpThis;
	while(pMgr->m_isSocket_valide){
		BYTE* pBufferRecv = NULL;
		int nLength = 0;

		pMgr->m_RecvBuffer->GetRecvParam(&pBufferRecv,nLength);
		if (pBufferRecv == NULL || nLength == 0)
		{
			MessageBox(NULL,L"缓冲区不足！",L"ERROR",MB_OK|MB_ICONSTOP);
			pMgr->m_isSocket_valide = false;
			ExitProcess(0);
		}

		int iRecv = recv(pMgr->m_socket,(char*)pBufferRecv,nLength,0);
		switch(iRecv){
			case SOCKET_ERROR:
				{
					pMgr->m_isSocket_valide = false;
					MessageBox(NULL,L"网络错误！点击确定退出程序。",L"客户端网络错误！",MB_OK);
					ExitProcess(0);
					break;
				}
			case 0:
				{
					pMgr->m_isSocket_valide = false;
					MessageBox(NULL,L"连接被断开！点击确定退出程序。",L"客户端网络错误！",MB_OK);
					ExitProcess(0);
					break;
				}
			default:
				{
					pMgr->m_RecvBuffer->Completion(iRecv);
					const int dwMaxPacketSize = 40*1024;

					//process data received
					BYTE* pBuffer = NULL;
					while (pBuffer = pMgr->m_RecvBuffer->GetFirstPacketPtr())
					{
						PACKET_HEADER *pHeader = (PACKET_HEADER*)pBuffer;

						if( pHeader->size <= 0 )
						{
							printf("[session][OnRecv]error:Check Head->size <0");
							pMgr->m_isSocket_valide = false;
							MessageBox(NULL,L"收到错误的数据包，该数据包的大小是无效的！",L"ERROR",MB_OK | MB_ICONSTOP);
							continue;
						}

						if( pHeader->size + sizeof(PACKET_HEADER) > dwMaxPacketSize )
						{
							printf("[session][OnRecv]error:packet size > dwMaxPacketSize");
							continue;
						}

						pMgr->OnRecv( pBuffer + sizeof(PACKET_HEADER), pHeader->size );
						pMgr->m_RecvBuffer->RemoveFirstPacket( sizeof(PACKET_HEADER) + pHeader->size );
					}
				}
		}
	}
	return 1;
}

void CTradeClientNetWork::OnRecv( PBYTE Packet,WORD PacketLen )
{
	if (PacketLen < sizeof(ACK_NET_HEAD))
	{
		if (PacketLen == 4)
		{
			//这是一个指示错误的数据包
			DWORD errorCode = *(DWORD*)Packet;
			switch(errorCode){
				case msg_error_unknow_command:
					MessageBox(NULL,L"服务器拒绝请求，无法识别的命令！",L"ERROR",MB_ICONSTOP);
					break;
				default:
					MessageBox(NULL,L"未知的错误！！请联系程序提供方！",L"ERROR",MB_ICONSTOP);
					break;
			}
		}
		return;
	}

	ResetTimeOut();

	ACK_NET_HEAD head;
	memcpy(&head,Packet,sizeof(head));
	if (head.AckMesgFullType == EMSG_HEART)
	{
		printf("收到心跳回应！");//do noting.这是个心跳包的回应。
	}
	else{
		//ProcessDataReceived(Packet,PacketLen);
		switch(head.AckMesgFullType){
			case EMSG_DISPATCH_SUCCESS:
				{
					if (sizeof(ACK_NET_HEAD) + sizeof(ASSIGN_ORDER_MSG) != PacketLen)
					{
						break;
					}

					ASSIGN_ORDER_MSG order_ass_msg;
					memcpy(&order_ass_msg,Packet+sizeof(ACK_NET_HEAD),sizeof(ASSIGN_ORDER_MSG));

					GM_GetOrder(order_ass_msg);
					break;
				}
			case EMSG_DISPATCH_FAILED:
				{
					if (PacketLen != sizeof(ACK_NET_HEAD)+sizeof(ASSIGN_ORDER_FAILED_MSG))
					{
						break;
					}

					ASSIGN_ORDER_FAILED_MSG* msg = (ASSIGN_ORDER_FAILED_MSG*)(Packet + sizeof(ACK_NET_HEAD));
					GM_GetOrderFailed(msg->error_code);
					break;
				}
			case EMSG_CANCEL_ALL_ORDERS:
				{
					//@FIXME:在这里添加处理取消所有订单的消息的函数体。
					if (PacketLen != sizeof(ACK_NET_HEAD))
					{
						break;
					}
					GM_CancelAllOrders();
					break;
				}
			case EMSG_BUDDY_FAILED:
				{
					if (PacketLen != sizeof(ACK_NET_HEAD)+sizeof(ACK_BUDDY_FAILED))
					{
						break;
					}
					ACK_BUDDY_FAILED* msg = (ACK_BUDDY_FAILED*)(Packet + sizeof(ACK_NET_HEAD));
					GM_MyBuddyIsDead(msg->order_dispatch_id,msg->buddy_failed_code);
					break;
				}
			case EMSG_BUDDY_DISCONNECTED:
				{
					if (PacketLen != sizeof(ACK_NET_HEAD)+sizeof(ACK_BUDDY_FAILED))
					{
						break;
					}
					ACK_BUDDY_FAILED* msg = (ACK_BUDDY_FAILED*)(Packet + sizeof(ACK_NET_HEAD));
					GM_MyBuddyIsDead(msg->order_dispatch_id,msg->buddy_failed_code);
					break;
				}
			default:
				break;
		}
	}
}

BOOL CTradeClientNetWork::DoLogin(SOCKET sock)
{
	struct sockaddr_in clientService; 
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( "192.168.1.224" );
	clientService.sin_port = htons( 4230 );

	//----------------------
	// Connect to server.
	int iResult = connect( sock, (SOCKADDR*) &clientService, sizeof(clientService) );
	if (iResult == SOCKET_ERROR) {
		printf( "connect failed with error: %d\n", WSAGetLastError() );
		goto failed;
	}

	char sendbufer[4096] = {0};

	LOGIN_MESSAGE li;

	char szName[32] = {0};
	DWORD dw=sizeof(szName);
	::GetUserNameA(szName,&dw);

	strcpy_s(li.szName,sizeof(li.szName),getMyMacName().c_str());

	li.client_mac_id = getMyMacId();

	NET_HEAD head;
	head.GameType = getGameTypeId();
	head.CheckSum = 0;
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = MSG_TYPE_LOGIN;
	head.PacketLen = sizeof(head)+sizeof(li);
	printf("msg size = %d\n",head.PacketLen);

	memcpy(sendbufer,&head,sizeof(head));
	memcpy(sendbufer+sizeof(head),&li,sizeof(li));

	iResult = Send(sendbufer,head.PacketLen);
	if (iResult == SOCKET_ERROR) {
		printf("send()0 failed with error: %d\n", WSAGetLastError());
		goto failed;
	}
	printf("Bytes Sent: %d\n", iResult);
	
	char recvbuf[4096] = "";
	int iRecvCount = recv(sock, recvbuf, sizeof(recvbuf), 0);
	if ( iRecvCount > 0 ){
		printf("Bytes received: %d\n", iRecvCount);
		//printf("0x%08x\n",*(DWORD*)(recvbuf+2));
		if (iRecvCount != 2+sizeof(ACK_NET_HEAD)){
			printf("bad length!%d\n",iRecvCount);
			goto failed;;
		}
	}
	else if ( iRecvCount == 0 ){
		printf("Connection closed\n");
		goto failed;
	}
	else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());
		goto failed;
	}

	return TRUE;
failed:
	closesocket(sock);
	return FALSE;
}

int CTradeClientNetWork::getMyMacId()
{
	static int i = LOWORD(GetTickCount());
	return i;
}

string CTradeClientNetWork::getMyMacName()
{
	string str = "test_client_machine-";
	str += String::fromLong(getMyMacId());
	return str;
}

void CTradeClientNetWork::GM_GetOrder( const ASSIGN_ORDER_MSG& orderdata)
{
	printf("*******i get a order*******\n");
	CTradeSession* session = new CTradeSession(orderdata.os/*,this*/);
	session->_event_progress_changed.connect(boost::bind(&CTradeClientNetWork::OnSessionProgressChanged,this,_1,_2,_3));//网络消息泵
	session->_event_progress_changed.connect(boost::bind(&CTrader::OnSessionProgressChanged,&CTrader::getInstance(),_1,_2,_3));//事务执行器
	{
		CAutoLock lock(*cs);
		_sessions.push_back(session);
	}
	session->setProgressInfors(EOP_order_received);
	InterlockedExchange(&m_is_idle_sended,0);
	return;
}

void CTradeClientNetWork::GM_GetOrderFailed( int reason )
{
	printf("******wait,error code = %d******\n",reason);
	return;
}

bool CTradeClientNetWork::PM_HasGetOrder( int order_dispatch_id )
{
	printf("CTradeClientNetWork::PM_HasGetOrder\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_order_received;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen\n");
		return false;
	}
	return true;
}

bool CTradeClientNetWork::PM_GameLunched( int order_dispatch_id )
{
	printf("CTradeClientNetWork::PM_HasGetOrder\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_game_lunched;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}

bool CTradeClientNetWork::PM_EnterRole( int order_dispatch_id )
{
	printf("CTradeClientNetWork::PM_EnterRole\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_in_game_role;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}


bool CTradeClientNetWork::PM_EnterTradeStage(  int order_dispatch_id  )
{
	printf("CTradeClientNetWork::PM_EnterTradeStage\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_at_trade_stage;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}


bool CTradeClientNetWork::PM_ReadyToTrade( int order_dispatch_id )
{
	printf("CTradeClientNetWork::PM_ReadyToTrade\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_ready_to_trade;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}


bool CTradeClientNetWork::PM_Trading(  int order_dispatch_id  )
{
	printf("CTradeClientNetWork::PM_Trading\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_trading;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}

bool CTradeClientNetWork::PM_TradeSuccess( int order_dispatch_id )
{
	printf("CTradeClientNetWork::PM_TradeSuccess\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = MAC_PROCESSING_SUCCESS;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_trade_end;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}

bool CTradeClientNetWork::PM_TradeFailed( int order_dispatch_id,int fail_reason )
{
	printf("CTradeClientNetWork::PM_TradeFailed\n");
	NET_HEAD head;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_MAC_STATE_CHANGE;
	head.PacketLen = sizeof(NET_HEAD) + sizeof(TRADE_STATE_CHANGED_MSG);


	TRADE_STATE_CHANGED_MSG msg;
	msg.result_id = fail_reason;
	msg.order_dispatch_id = order_dispatch_id;
	msg.state_code = EOP_trade_end;

	char sendbuffer[1024] = {0};
	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));

	if(Send(sendbuffer,head.PacketLen)!=head.PacketLen){
		C_log("Send failed because send count != head.packetlen");
		return false;
	}
	return true;
}

bool CTradeClientNetWork::PM_INeedAOrder( int burden )
{
// 	if (m_is_idle_sended)
// 	{
// 		return true;
// 	}

	printf("CTradeClientNetWork::PM_INeedAOrder\n");
	NET_HEAD head;
	head.GameType = getGameTypeId();
	head.CheckSum = 0;
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = MSG_TYPE_ACQUARE_ORDER;
	
	ACQUIRE_ORDER_MSG msg;
	msg.burden = burden;

	head.PacketLen = sizeof(head)+sizeof(msg);

	char sendbuffer[1024] = {0};

	memcpy(sendbuffer,&head,sizeof(head));
	memcpy(sendbuffer+sizeof(head),&msg,sizeof(msg));
	
	int iResult = Send(sendbuffer,head.PacketLen);

	if (iResult == SOCKET_ERROR) {
		printf("send()0 failed with error: %d\n", WSAGetLastError());
		return false;
	}
	InterlockedExchange(&m_is_idle_sended,1);
	return true;
}

void CTradeClientNetWork::OnSessionProgressChanged( CTradeSession* sender,EClientOrderProgress now,EClientOrderProgress previous )
{
	printf("CTradeClientNetWork::OnSessionProgressChanged\n");

	if (EOP_trade_end == now)
	{
		{
			CAutoLock lock(*cs);
			_sessions.remove(sender);
		}
		if (!sender->getLastOperationResult())
		{
			//wow,it's failure.
			order_struct order = sender->getOrderInfo();
			if(!PM_TradeFailed(order.order_dispatch_id,sender->getFailedReason())){
				//@FIXME:raise a exception?
				C_log("PM \"TradeFailed\" 发生错误！");
				return;
			}
		}else{
			//congratulation,y'a successfull.
			if (previous != EOP_trading)
			{
				//who skip the operation step?
				C_log("谁动了我的奶酪？");
			}
			order_struct order = sender->getOrderInfo();
			if (!PM_TradeSuccess(order.order_dispatch_id)){
				//@FIXME:raise a exception?
				C_log("PM \"TradeSuccess\" 发生错误！");
				return;
			}
		}
		return;
	}


	order_struct order = sender->getOrderInfo();
	switch(now){
		case EOP_standby:
			{
				assert(0);
				break;
			}
		case EOP_order_received:
			{
				PM_HasGetOrder(order.order_dispatch_id);
				break;
			}
		case EOP_game_lunched:
			{
				PM_GameLunched(order.order_dispatch_id);
				break;
			}
		case EOP_in_game_role:
			{
				PM_EnterRole(order.order_dispatch_id);
				break;
			}
		case EOP_at_trade_stage:
			{
				PM_EnterTradeStage(order.order_dispatch_id);
				break;
			}
		case EOP_ready_to_trade:
			{
				PM_ReadyToTrade(order.order_dispatch_id);
				break;
			}
		case EOP_trading:
			{
				PM_Trading(order.order_dispatch_id);
				break;
			}
		case EOP_trade_end:
			{
				assert(0);
				break;
			}
		default:
			assert(0);
			break;
	}
	return ;
}

void CTradeClientNetWork::showMe()
{
	printf("fun: CTradeClientNetWork(), address: 0x%08x\n",&CTradeClientNetWork::CTradeClientNetWork());
	printf("fun: GM_GetOrder, address: 0x%08x\n",&CTradeClientNetWork::GM_GetOrder);
	printf("fun: GM_GetOrderFailed, address: 0x%08x\n",&CTradeClientNetWork::GM_GetOrderFailed);
	printf("fun: GM_GetStopCmd, address: 0x%08x\n",&CTradeClientNetWork::GM_CancelAllOrders);
	printf("fun: getGameTypeId, address: 0x%08x\n",&CTradeClientNetWork::getGameTypeId);
	
	printf("data: this, address: 0x%08x\n",this);
	printf("data: session_list, address: 0x%08x\n",&_sessions);
	printf("data: m_lastCommtime, address: 0x%08x\n",&m_lastCommtime);
	printf("data: m_hThdSocketRecv, address: 0x%08x\n",&m_hThdSocketRecv);
	printf("TRADE_STATE_CHANGED_MSG + head = %d\n",sizeof(TRADE_STATE_CHANGED_MSG)+sizeof(NET_HEAD));
}

void CTradeClientNetWork::GM_CancelAllOrders()
{
	printf("收到退出所有订单的消息……");
	CAutoLock lock(*cs);
// 	for (list<CTradeSession*>::const_iterator it = _sessions.begin(); it!=_sessions.end(); ++it){
// 		(*it)->cancel_trade_imm();
// 	}//We could not use these statements cause function "cancel_trade_imm" may call "_sessions.remove(element)",that make iterator invalid.
	
	//Use statements below,it doesn't work caulse cancel_trade_imm may not call "_sessions.remove(element)",that make it in dead loop.
// 	while (_sessions.size())
// 	{
// 		_sessions.front()->cancel_trade_imm();
// 	}

	list<CTradeSession*> _sessions_dead = _sessions;
	for (list<CTradeSession*>::const_iterator it = _sessions_dead.begin(); it != _sessions_dead.end(); ++it)
	{
		(*it)->cancel_trade_imm(TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL);//Return false if the order is trading or has traded.But we don't care it,just notify it:you should exit imm.
	}
	_sessions_dead.clear();//notice ,call clear to avoid call ~CTradeSession() twice.
}

void CTradeClientNetWork::MP_SendHeart()
{
	NET_HEAD head;
	memset(&head,0,sizeof(head));
	head.HeadFlag = message_tag_trade;
	head.MesgFullType = EMSG_HEART;
	head.CheckSum = 0;
	head.GameType = getGameTypeId();
	head.PacketLen = sizeof(NET_HEAD)+sizeof(HEART_MSG);

	HEART_MSG msg;
	double burden = GetCpuUsage();
	msg.burden = burden<0?100:burden * 100;

	char bufferSend[140] = {0};
	memcpy(bufferSend,&head,sizeof(head));
	memcpy(bufferSend+sizeof(head),&msg,sizeof(msg));

	int isend = Send(bufferSend,head.PacketLen);
	if (isend != head.PacketLen){
		C_log("发送心跳包失败！");
	}

	return;
}

CTradeSession* CTradeClientNetWork::getSession( int order_dispatch_id )
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

void CTradeClientNetWork::GM_MyBuddyIsDead( int order_dispatch_id,int failed_reasion )
{
	CTradeSession* session = getSession(order_dispatch_id);
	if (session == NULL)
	{
		return;
	}
	C_log("GM_MyBuddyIsDead,order_id = %s,seller_or_buyer=%d,failed_reason=%d",session->getOrderInfo().order_id,session->getOrderInfo().trade_role,failed_reasion);
	session->cancel_trade_imm(TRADE_FAILED_REASON_BUDDY_IS_FAILED);
}
