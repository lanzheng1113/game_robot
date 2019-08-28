#include "StdAfx.h"
#include "affair.h"
#include "NetworkCallBack.h"
#include "../common/DebugMessage.h"
affair::affair(void)
{
	m_NetworkCallback = NULL;
	m_CloseRequestPosted = FALSE;
}

affair::~affair(void)
{
	//DebugMessageA("[affair][~affair]");
}

BOOL affair::Send( BYTE *pMsg, WORD wSize )
{
	if (!m_NetworkCallback)
	{
		DebugMessageA("[affair][Send] error: NetworkCallback is null");
		return FALSE;
	}
	return m_NetworkCallback->Send(pMsg,wSize);
}

BOOL affair::SendEx( DWORD dwNumberOfMessages, BYTE **ppMsg, WORD *pwSize )
{
	if (!m_NetworkCallback)
	{
		DebugMessageA("[affair][SendEx] error: NetworkCallback is null");
		return FALSE;
	}
	return m_NetworkCallback->SendEx(dwNumberOfMessages,ppMsg,pwSize);
}

BOOL affair::CloseSession()
{
	if(m_CloseRequestPosted){
		//DebugMessageA("[affair][closeSession] has already posted a close request.");
		return TRUE;
	}
	
	if (!m_NetworkCallback)
	{
		DebugMessageA("[affair][closeSession] error: NetworkCallback is null");
		return FALSE;
	}

	m_CloseRequestPosted = TRUE;
	m_NetworkCallback->Disconnect(); //net work callbacks will destroy after OnClosed();

	return TRUE;
}