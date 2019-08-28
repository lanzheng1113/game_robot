#include "StdAfx.h"
#include "TradeAffairMgr.h"
#include "../common/AutoLock.h"


CTradeAffairMgr g_TradeAffarMgr;

CTradeAffairMgr::CTradeAffairMgr(void)
{
	m_bClosed = false;
	m_hThread = (HANDLE)0;
	m_listLocker = new CLock_CS();
}

CTradeAffairMgr::~CTradeAffairMgr(void)
{
	if (m_listLocker)
	{
		delete m_listLocker;
		m_listLocker = NULL;
	}
}

UINT CTradeAffairMgr::Thdfun_UserListDeamon( LPVOID lpThis )
{
	CTradeAffairMgr* pMgr = (CTradeAffairMgr*)lpThis;
	while(!pMgr->m_bClosed){
		pMgr->processAffairList();
		Sleep(200);
	}
	return 0;
}

void CTradeAffairMgr::processAffairList()
{
	AffairList temp_list;
	{
		CAutoLock locker(*m_listLocker);
		for (AffairList_Iter it = m_AssistAffiarlist.begin(); it!=m_AssistAffiarlist.end();)
		{
			if ((*it)->GetShouldRemove())
			{
				temp_list.push_back(*it);
				it = m_AssistAffiarlist.erase(it);
			}else{
				 ++it;
			}
		}
	}


	for (AffairList_Iter it = temp_list.begin(); it != temp_list.end(); ++it)
	{
		delete (*it);
	}

	temp_list.clear();
}

void CTradeAffairMgr::Run()
{
	if (!m_hThread)
	{
		m_hThread = AfxBeginThread(Thdfun_UserListDeamon,this)->m_hThread;
	}
}

void CTradeAffairMgr::AddAffair( CTradeAffair* afr )
{
	{
		CAutoLock locker(*m_listLocker);
		m_AssistAffiarlist.push_back(afr);
	}
}

CTradeAffairMgr* afkGetTradeAffairManager()
{
	return &g_TradeAffarMgr;
}
