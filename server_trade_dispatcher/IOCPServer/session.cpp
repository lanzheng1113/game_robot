#include "stdafx.h"
#include "session.h"
#include "sendbuffer.h"
#include "recvbuffer.h"
#include "NetworkCallBack.h"
#include "../common/netbase.h"
#include "../common/DebugMessage.h"

session::session( DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOut )
{
	m_pSendBuffer = new SendBuffer;
	m_pSendBuffer->Create( dwSendBufferSize, dwMaxPacketSize );

	m_pRecvBuffer = new RecvBuffer;
	m_pRecvBuffer->Create( dwRecvBufferSize, dwMaxPacketSize );

	m_dwTimeOut			= dwTimeOut;
	m_socket			= INVALID_SOCKET;
}


session::~session()
{
	//DebugMessageA("[session][destroy] session destroy.\n");
	CloseSocket();
	SAFE_DELETE(m_pSendBuffer);
	SAFE_DELETE(m_pRecvBuffer);
}

VOID session::Init()
{
	m_pSendBuffer->Clear();
	m_pRecvBuffer->Clear();

	ResetKillFlag();
}

BOOL session::PreRecv()
{
	WSABUF wsabuf;

	m_pRecvBuffer->GetRecvParam( (BYTE**)&wsabuf.buf, (int&)wsabuf.len );

	ZeroMemory( &m_recvIoData, sizeof(OVERLAPPEDEX) );

	m_recvIoData.dwOperationType = RECV_POSTED;

	int ret = WSARecv( m_socket, &wsabuf, 1, &m_recvIoData.dwIoSize, &m_recvIoData.dwFlags, &m_recvIoData, NULL ); 

	if( ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL session::PreSend()
{
	WSABUF wsabuf;
	
	if( m_pSendBuffer->GetSendParam( (BYTE**)&wsabuf.buf, (int&)wsabuf.len ) == FALSE )
	{
		//DebugMessageA("[session][PreSend]Error while get send parameter!\n");
		return TRUE;
	}

	ZeroMemory( &m_sendIoData, sizeof(OVERLAPPEDEX) );

	m_sendIoData.dwOperationType = SEND_POSTED;
	
	int ret = WSASend( m_socket, &wsabuf, 1, &m_sendIoData.dwIoSize, m_sendIoData.dwFlags, &m_sendIoData, NULL );

	if( ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING )
	{
		//DebugMessageA("[session][PreSend]Error while send!\n");
		return FALSE;
	}

	return TRUE;
}

BOOL session::Send( BYTE *pMsg, WORD wSize )
{
	PACKET_HEADER header;

	// set head
	header.size = wSize;

	// set send buffer
	if( m_pSendBuffer->Write( &header, pMsg ) == FALSE )
	{
		//OnLogString( "send buffer overflow." );
		Remove();
		return FALSE;
	}
	return TRUE;
}

BOOL session::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	assert( !IsBadReadPtr( ppMsg, sizeof(ppMsg) * dwNumberOfMessages ) );
	assert( !IsBadReadPtr( pwSize, sizeof(pwSize) * dwNumberOfMessages ) );

	PACKET_HEADER header;

	// set packet head
	header.size = 0;
	DWORD i;
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
		header.size += pwSize[i];
	}

	// write to sendbuffer
	if( !m_pSendBuffer->Write( (BYTE*)&header, sizeof(PACKET_HEADER) ) )
	{
		DebugMessageA( "[session][SendEx][Write head]error:write buffer overflow." );
		Remove();
		return FALSE;
	}

	// write by bat.
	for( i = 0; i < dwNumberOfMessages; ++i )
	{
		if( !m_pSendBuffer->Write( ppMsg[i], pwSize[i] ) )
		{
			DebugMessageA( "[session][SendEx][Write Msg]error:write buffer overflow." );
			Remove();
			return FALSE;
		}
	}
	return TRUE;
}

VOID session::BindNetworkCallBack( NetworkCallBack* cbk )
{
	m_NetworkCallBack = cbk;
	m_NetworkCallBack->SetSession( this );
}

VOID session::UnbindNetworkCallBack()
{
	if( m_NetworkCallBack == NULL )
	{
		return;
	}
	m_NetworkCallBack->SetSession( NULL );

	m_NetworkCallBack = NULL;
}

BOOL	_cbk_fun session::OnRecv(DWORD dwMaxPacketSize)
{
	BYTE			*pPacket;
	PACKET_HEADER	*pHeader;

	if(!m_NetworkCallBack){
		DebugMessageA("[session][OnRecv]error:CallbackObject is null");
		return FALSE;
	}
	//process data received
	while( pPacket = GetRecvBuffer()->GetFirstPacketPtr() )
	{
		pHeader = (PACKET_HEADER*)pPacket;

		if( pHeader->size <= 0 )
		{
			DebugMessageA("[session][OnRecv]error:Check Head->size <0");
			return FALSE;
		}

		if( pHeader->size + sizeof(PACKET_HEADER) > dwMaxPacketSize )
		{
			DebugMessageA("[session][OnRecv]error:packet size > dwMaxPacketSize");
			return FALSE;
		}

		m_NetworkCallBack->OnRecv( pPacket + sizeof(PACKET_HEADER), pHeader->size );
		m_pRecvBuffer->RemoveFirstPacket( sizeof(PACKET_HEADER) + pHeader->size );

		ResetTimeOut();
//		DebugMessageA("reset time out!!!!");
	}

	return TRUE;
}

VOID session::CloseSocket()
{
	if (INVALID_SOCKET != m_socket)
	{
		DebugMessageA("[session]close socket %d\n",m_socket);
		closesocket( m_socket ); 
		m_socket = INVALID_SOCKET;
	}
}

VOID session::Disconnect()
{
	//Disconnect
	ZeroMemory( &m_disconnectIoData, sizeof(OVERLAPPEDEX) );
	m_disconnectIoData.dwOperationType	= DISCONNECT_POSTED;

	//
	//CloseSocket();		
	PostQueuedCompletionStatus( m_hIocp, 0, (ULONG_PTR)this, &m_disconnectIoData );
}