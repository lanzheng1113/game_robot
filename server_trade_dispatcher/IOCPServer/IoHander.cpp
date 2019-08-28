#include "stdafx.h"
#include "IoHander.h"
#include "session.h"
#include "../common/DebugMessage.h"
#include "../common/netbase.h"
#include "SessionList.h"
#include "sendbuffer.h"
#include "recvbuffer.h"
#include "NetworkCallBack.h"
#include "SessionPool.h"

static LONG g_nTcpServerWorkerCnt = 0;

IoHander::IoHander()
{
	m_listenSocket = NULL;
	m_hIOCP = 0;
	m_bExit = FALSE;
	m_hThdAccept = 0;
	m_serverParameter = NULL;
	m_hThdSend = 0;
	m_dwMaxPacketSize = 0;
	m_pAcceptSessionPool = NULL;
}

IoHander::~IoHander()
{
	if (m_listenSocket != INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}
	if( m_pAcceptSessionPool )	
		delete m_pAcceptSessionPool;
}

BOOL IoHander::Init( IocpServer *pIOCPServer, LPIOHANDLER_DESC lpDesc )
{
	m_serverParameter = lpDesc;
	m_dwMaxPacketSize = lpDesc->dwMaxPacketSize;

	m_fnCreateAcceptedObject = m_serverParameter->fnCreateAcceptedObject;
	m_fnDestroyAcceptedObject = m_serverParameter->fnDestroyAcceptedObject;

	//create listen socket
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_listenSocket == INVALID_SOCKET)
	{
		DebugMessage(L"WSASocket() failed");
		return FALSE;
	}

	SOCKADDR_IN InternetAddr;
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(m_serverParameter->usListenPort);

	int iBindRet = bind(m_listenSocket,(LPSOCKADDR)&InternetAddr,sizeof(InternetAddr));
	if (iBindRet == SOCKET_ERROR)
	{
		DebugMessage(L"bind failed with error %d",WSAGetLastError());
		closesocket(m_listenSocket);
		return FALSE;
	}

	if (listen(m_listenSocket,5) == SOCKET_ERROR)
	{
		DebugMessageA("listen failed with error %d\n",WSAGetLastError());
		closesocket(m_listenSocket);
		return FALSE;
	}

	m_pAcceptSessionPool	= new SessionPool( lpDesc->dwMaxAcceptSession + EXTRA_ACCEPTEX_NUM,
		lpDesc->dwSendBufferSize,
		lpDesc->dwRecvBufferSize,
		lpDesc->dwMaxPacketSize,
		lpDesc->dwTimeOut,
		1);

	//create a iocp handle
	if ((m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0)) == NULL){
		DebugMessage(L"CreateIoCompletionPort failed");
		return FALSE;
	}
	
	//create io thread.
	for (int i = 0; i!=m_serverParameter->dwNumberOfIoThreads;i++)
	{
		CWinThread * pThd = AfxBeginThread(io_thread,this);
		if (!pThd)
		{
			continue;
		}
		InterlockedIncrement(&g_nTcpServerWorkerCnt);
	}

	//create accept thread.
	m_hThdAccept = AfxBeginThread(accept_thread,this)->m_hThread;
	m_hThdUpdate = AfxBeginThread(UpdateThread,this)->m_hThread;
	m_hThdSend = AfxBeginThread(send_thread,this)->m_hThread;

	return TRUE;
}


UINT IoHander::send_thread( LPVOID thdParam )
{
	IoHander *pIoHandler = (IoHander*)thdParam;
	while(!pIoHandler->m_bExit)
	{
		pIoHandler->m_AcceptedSessionList.Lock();
		session* pSession = NULL;
		for( SESSION_LIST_ITER it = pIoHandler->m_AcceptedSessionList.begin(); it != pIoHandler->m_AcceptedSessionList.end(); ++it )
		{
			pSession = *it;
			if( pSession->ShouldBeRemoved() ) 
				continue;

			if( pSession->PreSend() == FALSE ) 
			{
				DebugMessageA("[REMOVE][%d] pSession->PreSend() == FALSE\n", (int)pSession->GetSocket());
				pSession->Remove();
			}
		}
		pIoHandler->m_AcceptedSessionList.Unlock();
		Sleep(5);
	}
	pIoHandler->m_hThdSend = 0;
	return 0;
}

UINT IoHander::io_thread( LPVOID thdParam )
{
	IoHander *pIoHandler = (IoHander*)thdParam;

	BOOL			bSuccess = FALSE;
	DWORD			dwIoSize = 0;

	session			*pSession = NULL;
	OVERLAPPEDEX	*pOverlappedEx = NULL;

	while( 1 )
	{
		bSuccess = GetQueuedCompletionStatus( pIoHandler->m_hIOCP, &dwIoSize, (LPDWORD)&pSession,(LPOVERLAPPED*)&pOverlappedEx, INFINITE );

		//thread quit.
		if( pSession == NULL && dwIoSize == 0 && pOverlappedEx == NULL)
			break;

		//net work error.
		if( !bSuccess )
		{
			DWORD dwLastErr = GetLastError();
			if( dwLastErr == ERROR_NETNAME_DELETED )
			{
				DebugMessageA("[Io thread][REMOVE][%d] GetLastError() == ERROR_NETNAME_DELETED\n", (int)pSession->GetSocket());
				pSession->Remove();
			}
			else if(dwLastErr == 1236){//1236:由本地系统终止网络连接。 yy
				//DebugMessageA("[iohander][io_thread]err:由本地系统终止网络连接\n");
				;
			}
			else{
				DebugMessageA("[Io thread][REMOVE] Failed LastError = %d",dwLastErr);
			}
			continue;
		}

		// DISCONNECT_POSTED not post but iosize == 0;
		if( pOverlappedEx->dwOperationType != DISCONNECT_POSTED && dwIoSize == 0 )
		{
			DebugMessageA("[Io thread][REMOVE]IO Count == 0 remove the session (%d)\n",(int)pSession->GetSocket());
			pSession->Remove();
			continue;
		}

		// Completion 贸府
		switch( pOverlappedEx->dwOperationType )
		{
		case SEND_POSTED:
			pSession->GetSendBuffer()->Completion( dwIoSize );
			//DebugMessageA("[Io thread][SEND_POSTED]Send packet %d bytes",dwIoSize);
			break;

		case RECV_POSTED:
			if (pSession->ShouldBeRemoved()){
				DebugMessageA("[Io thread][REMOVE] session should be removed!\n");
				break;
			}
			//DebugMessageA("[io_thread][receive]recv=%d\n",dwIoSize);
			pSession->GetRecvBuffer()->Completion( dwIoSize );//push stack
			//pSession->OnRecv(pIoHandler->m_serverParameter->dwMaxPacketSize);//process the data.
			// post another Recv request.
			if( !pSession->PreRecv() )	
			{
				DebugMessageA("[REMOVE][%d] errr in pSession->PreRecv()\n", (int)pSession->GetSocket());
				pSession->Remove();
			}
			break;

		case DISCONNECT_POSTED://just remove it.
			pSession->Remove();
			break;
		}	
	}
	InterlockedDecrement(&g_nTcpServerWorkerCnt);
	return 0;
}

UINT IoHander::accept_thread( LPVOID thdParam )
{
	IoHander* pIoHander = (IoHander*)thdParam;
	SOCKET sockAccepted = SOCKET_ERROR;
	while(!pIoHander->m_bExit){
 		if ((sockAccepted = WSAAccept(pIoHander->m_listenSocket,NULL,NULL,NULL,0)) == SOCKET_ERROR)
 		{
 			DebugMessageA("[IoHander][accept_thread]CTCPServer: WSAAccept failed with error %d\n",WSAGetLastError());
			if (pIoHander->m_bExit){
				break;
			}
 			continue;
 		}
 		//DebugMessageA("Socket number %d connected\n",sockAccepted);
		//
 		//create a session accord to this connected socket.
 		session* Sess = pIoHander->AllocAcceptSession();
 		if (Sess == NULL)
 		{
 			DebugMessageA("[IoHander][accept_thread]new failed with error %d\n",GetLastError());
 			closesocket(sockAccepted);
 			continue;
 		}
 
		int addrlen = sizeof(SOCKADDR_IN);
		SOCKADDR_IN addr;
		memset(&addr,0,sizeof(SOCKADDR_IN));
		getpeername(sockAccepted,(sockaddr*)&addr,&addrlen);
		
		Sess->Init();
		Sess->SetConnecttime(GetTickCount());
		Sess->ResetKillFlag();
		Sess->SetIndex(0);
		Sess->SetPort(addr.sin_port);
		Sess->SetSockAddr(addr);
		Sess->SetSocket(sockAccepted);
		//Sess->SetTimeOut(pIoHander->m_serverParameter->dwTimeOut);
		Sess->ResetTimeOut();
		Sess->SetIocpHandle(pIoHander->m_hIOCP);

		NetworkCallBack* pNetworkCallback = pIoHander->m_fnCreateAcceptedObject();
		if (!pNetworkCallback)
		{
			DebugMessageA("[IoHander][accept_thread]m_fnCreateAcceptedObject failed\n");
			pIoHander->FreeSession(Sess);
			continue;
		}
		Sess->BindNetworkCallBack(pNetworkCallback);

		//add to session list;
		pIoHander->m_AcceptedSessionList.Lock();
		pIoHander->m_AcceptedSessionList.push_back(Sess);
		pIoHander->m_AcceptedSessionList.Unlock();

		//bind iocp handle.
 		HANDLE hPort = CreateIoCompletionPort((HANDLE)sockAccepted,pIoHander->m_hIOCP,(DWORD)Sess,0);
 		if (hPort == NULL)
 		{
 			DebugMessageA("[IoHander][accept_thread]CreateIoCompletionPort failed with error%d\n",GetLastError());
			pIoHander->FreeSession(Sess);
 			continue;
		}

		//post a RECV request to socket.
		Sess->PreRecv();
		//carry on until exit flag is set.
	}
	pIoHander->m_hThdAccept = 0;
	return 0;
}

void IoHander::kickdDeadSessions()
{
	SessionList TempList;

	m_AcceptedSessionList.Lock();
	for(SESSION_LIST_ITER iter = m_AcceptedSessionList.begin(); iter != m_AcceptedSessionList.end();)
	{
		if ((*iter)->ShouldBeRemoved()){
			TempList.push_back(*iter);
			iter = m_AcceptedSessionList.erase(iter);
		}
		else
			++ iter;
	}
	m_AcceptedSessionList.Unlock();

	for (SESSION_LIST_ITER iter = TempList.begin(); iter != TempList.end(); ++iter)
	{
		session* sess = *iter;

		NetworkCallBack* NetworkCbk = sess->GetNetworkCallBack();//unbind it;
		sess->UnbindNetworkCallBack();

		NetworkCbk->OnDisconnect();//Notify disconnect;
		
		m_fnDestroyAcceptedObject( NetworkCbk );//then "destroy" network callback object.it depend how m_fnDestroyAcceptedObject implete.

		FreeSession(sess);//at last,destroy the session.
	}
	TempList.clear();
}

session* IoHander::AllocAcceptSession()
{
	// DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut
	return m_pAcceptSessionPool->Alloc();
}

VOID IoHander::FreeSession( session *pSession )
{
	//DebugMessageA("[IoHander][FreeSession]:free the session");
	pSession->CloseSocket();
	pSession->Init();
	m_pAcceptSessionPool->Free( pSession );
}

void IoHander::ProcessActiveSessions()
{
	//pSession->OnRecv(pIoHandler->m_serverParameter->dwMaxPacketSize);//process the data.
	SESSION_LIST_ITER	it;
	session				*pSession;
	m_AcceptedSessionList.Lock();
	for( it = m_AcceptedSessionList.begin(); it != m_AcceptedSessionList.end(); ++it )
	{
		pSession = *it;

		if( pSession->ShouldBeRemoved() ) 
			continue;
		else
		{
			if (pSession->IsOnIdle())
			{
				DebugMessageA("[IoHander][ProcessActiveSessions]session %d on idle!kick it.",(int)pSession->GetSocket());
				pSession->Remove();
				continue;
			}

			// 罐扁 滚欺 贸府
			if( !pSession->OnRecv( m_dwMaxPacketSize ) )
			{
				pSession->Remove();
			}
		}
	}
	m_AcceptedSessionList.Unlock();
}

UINT IoHander::UpdateThread( LPVOID thdParam )
{
	IoHander* pIoHander = (IoHander*)thdParam;
	while (!pIoHander->m_bExit)
	{
		pIoHander->ProcessActiveSessions();
		pIoHander->kickdDeadSessions();
		Sleep(5);
	}
	pIoHander->m_hThdUpdate = 0;
	return 0;
}

VOID IoHander::Clear()
{
	m_bExit = TRUE;
	while (g_nTcpServerWorkerCnt)
	{
		PostQueuedCompletionStatus( m_hIOCP,0,0,0 );
		DebugMessageA("Waiting for worker thread exit");
		Sleep(100);
		DebugMessageA("Current workThread %d\n",g_nTcpServerWorkerCnt);
	}

	closesocket(m_listenSocket);
	m_listenSocket = INVALID_SOCKET;
	Sleep(300);

	if(m_hThdAccept){
		TerminateThread(m_hThdAccept,1);
		m_hThdAccept = 0;
	}

	if (m_hThdSend)
	{
		TerminateThread(m_hThdSend,1);
		m_hThdSend = 0;
	}
	
	if (m_hThdUpdate)
	{
		TerminateThread(m_hThdUpdate,1);
		m_hThdUpdate = 0;
	}
	
}