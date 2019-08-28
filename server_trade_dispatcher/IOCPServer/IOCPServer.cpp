#include "stdafx.h"
#include "iocpserver.h"
#include "session.h"
#include "../common/DebugMessage.h"
#include "IoHander.h"
#include "../common/NetBase.h"

IocpServer::IocpServer()
:m_ioHander(NULL)
,m_ioParameter(NULL)
{
}

IocpServer::~IocpServer(){
	SAFE_DELETE(m_ioParameter);
	SAFE_DELETE(m_ioHander);
	WSACleanup();
}

BOOL IocpServer::Init(LPIOHANDLER_DESC pDesc)
{
	//select windows socket dll version 2.2.
	WSADATA wsadata;
	DWORD dwStartupRet;
	if (dwStartupRet = WSAStartup(0x2020,&wsadata) != 0)
	{
		DebugMessage(L"WSAStartup failed");
		return FALSE;
	}
	m_ioParameter = new IOHANDLER_DESC;
	if (!m_ioParameter)
		return FALSE;
	memset(m_ioParameter,0,sizeof(IOHANDLER_DESC));
	memcpy(m_ioParameter,pDesc,sizeof(IOHANDLER_DESC));
	//create iohander
	m_ioHander = new IoHander();
	return m_ioHander->Init(this,m_ioParameter);
}

BOOL IocpServer::Run()
{
	return TRUE;
}

void IocpServer::ShutDown()
{
	m_ioHander->Clear();
	return;
}