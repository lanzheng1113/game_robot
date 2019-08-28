#include "stdafx.h"
#include <assert.h>
#include "SessionPool.h"
#include "Session.h"
#include "SessionList.h"

SessionPool::SessionPool( DWORD dwSize,
						  DWORD dwSendBufferSize,
						  DWORD dwRecvBufferSize, 
						  DWORD dwMaxPacketSize,
						  DWORD dwTimeOutTick,
						  DWORD dwIndexStart)
{
	m_pList = NULL;
	Create( dwSize, dwSendBufferSize, dwRecvBufferSize, dwMaxPacketSize, dwTimeOutTick, dwIndexStart );
}

SessionPool::~SessionPool()
{
	if( m_pList ) delete m_pList;
}

VOID SessionPool::Create( DWORD dwSize, DWORD dwSendBufferSize, DWORD dwRecvBufferSize, DWORD dwMaxPacketSize, DWORD dwTimeOutTick, DWORD dwIndexStart )
{
	m_dwMaxSize			= dwSize;

	// ���� ����Ʈ ����
	m_pList = new SessionList;

	// ����Ʈ�� �ν��Ͻ����� ä���.
	session *pSession;

	for( DWORD i = 0; i < dwSize; ++i )
	{
		pSession = new session( dwSendBufferSize, dwRecvBufferSize, dwMaxPacketSize, dwTimeOutTick );

		pSession->SetIndex( dwIndexStart + i );

		m_pList->push_back( pSession );
	}
}

session* SessionPool::Alloc()
{
	m_pList->Lock();

	if( m_pList->empty() )
	{
		m_pList->Unlock();
		
		return NULL;
	}

	session *pSession = m_pList->front();

	m_pList->pop_front();

	//printf( "New ����Ǯ(%x) ���� ����(%d)\n", this, m_pList->size() ); 

	m_pList->Unlock();

	return pSession;
}

VOID SessionPool::Free( session* pSession )
{
	m_pList->Lock();

	assert( m_pList->size() < m_dwMaxSize );

	m_pList->push_back( pSession );

	m_pList->Unlock();
}

int SessionPool::GetLength()
{
	m_pList->Lock();

	int size = (int)( m_pList->size() );

	m_pList->Unlock();

	return size;
}
