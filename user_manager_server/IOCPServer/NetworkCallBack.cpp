#include "StdAfx.h"
#include "NetworkCallBack.h"
#include "session.h"
#include "../common/DebugMessage.h"
NetworkCallBack::NetworkCallBack(void)
{
	m_pSession = NULL;
}

NetworkCallBack::~NetworkCallBack(void)
{
	//DebugMessageA("[NetworkCallBack] ~NetworkCallBack");
}


VOID NetworkCallBack::Disconnect(  )
{
	if( m_pSession )
	{
		m_pSession->Disconnect();
	}
}

BOOL NetworkCallBack::Send( BYTE *pMsg, WORD wSize )
{
	if( !m_pSession || m_pSession->ShouldBeRemoved() ) return FALSE;
	return m_pSession->Send( pMsg, wSize );
}

BOOL NetworkCallBack::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	if( !m_pSession || m_pSession->ShouldBeRemoved() ) return FALSE;

	return m_pSession->SendEx( dwNumberOfMessages, ppMsg, pwSize );
}

char* NetworkCallBack::GetIP()
{
	if( m_pSession )
	{
		return m_pSession->GetIP();
	}
	else
	{
		return NULL;
	}
}
