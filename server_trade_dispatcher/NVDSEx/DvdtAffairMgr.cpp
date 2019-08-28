#include "StdAfx.h"
#include "DvdtAffairMgr.h"
#include "DoValidateAffair.h"
#include "../common/AutoLock.h"
#include "../common/NetBase.h"
#include "../common/DebugMessage.h"
#include "../common/DBdef.h"
#include "DBM.h"
#include "../common/EventIdDef.h"
#include "ActiveConnectionInfo.h"
#include "strsafe.h"

DoValidateAffairMgr g_DvlidateAffairMgr;

DoValidateAffairMgr::DoValidateAffairMgr(void)
{
	m_pListCriticalSection = new CLock_CS();
	m_bExit = FALSE;
	m_hThdProcessList = 0;
}

DoValidateAffairMgr::~DoValidateAffairMgr(void)
{
	SAFE_DELETE(m_pListCriticalSection);
}

BOOL DoValidateAffairMgr::RegisterAffair( DoValidateAffair* paff )
{
	{
		CAutoLock lock(*m_pListCriticalSection);
		m_affairListLogining.push_back(paff);
	}
	return TRUE;
}

BOOL DoValidateAffairMgr::UnRegisterAffair( DoValidateAffair* paff )
{
	//Do nothing,just return TRUE;
	return TRUE;
}

VOID DoValidateAffairMgr::DoValidateOK( DoValidateAffair* paff )
{
	{
		CAutoLock lock(*m_pListCriticalSection);
		m_affairListLogined.push_back(paff);
		m_affairListLogining.remove(paff);
	}
	DebugMessageA("[DoValidateAffairMgr][DoValidateOK]logining list = %d,logined list = %d",m_affairListLogining.size(),m_affairListLogined.size());
}

VOID DoValidateAffairMgr::Run()
{
	m_hThdProcessList = AfxBeginThread(process_thread,this)->m_hThread;
}

UINT DoValidateAffairMgr::process_thread( LPVOID lpThis )
{
	DoValidateAffairMgr* pMgr = (DoValidateAffairMgr*)lpThis;
	while (!pMgr->m_bExit)
	{
		Sleep(50);
		//--process the logining list;--
		list<DoValidateAffair*> TempList_logining;
		{//-lock area begin-
			CAutoLock lock(*(pMgr->m_pListCriticalSection));
			DWORD curtime = GetTickCount();
			
			for(list<DoValidateAffair*>::iterator it = pMgr->m_affairListLogining.begin();it != pMgr->m_affairListLogining.end();)
			{
				if ((*it)->ShouldRemoved() ){//delete should remove.
					TempList_logining.push_back(*it);
					it = pMgr->m_affairListLogining.erase(it);
				}
				else{
					if (curtime - (*it)->GetLastCommTime() > 3000)//3 seconds time out when do validate.
					{
						//DebugMessageA("[DoValidateAffairMgr][process_thread]:3 seconds time out");
						BOOL bCloseSession = (*it)->CloseSession();//it will set should remove flag later ,when affair receive a session-closed notify.
						if (!bCloseSession)
						{
							DebugMessageA("[DoValidateAffairMgr][process_thread]error:CloseSession return FALSE");//the network-object has been destroyed.
							(*it)->Remove();
						}
					}

					++it;
				}
			}
		}//-lock area end-

		for (list<DoValidateAffair*>::iterator it = TempList_logining.begin();it!=TempList_logining.end();++it)
		{
			if ((*it)->GetValidateStep() != DoValidateAffair::DoValidateStep_NotBegin)
			{
				pMgr->m_pDbm->WriteEvent((*it)->GetUserinfo()->dwUser_ID,Event_caledge_DoValidate_fail,Eid_3Second_Login_TimeOut,L"µÇÂ¼3ÃëÎ¥Àý",(*it)->GetClientIp());
			}
			delete (*it);//delete the affair should remove.
		}

		TempList_logining.clear();//clear it.do clear to prevent templist's element auto-delete .
		
		//--process the logined list--
		list<DoValidateAffair*> TempList_logined;
		{//-lock area begin-
			CAutoLock lock(*(pMgr->m_pListCriticalSection));
			DWORD curtime = GetTickCount();
			for (list<DoValidateAffair*>::iterator it = pMgr->m_affairListLogined.begin();it!=pMgr->m_affairListLogined.end();)
			{
				if ((*it)->ShouldRemoved())
				{
					TempList_logined.push_back(*it);
					it = pMgr->m_affairListLogined.erase(it);
//					DebugMessageA("[process_thread][remove logined]m_affairListLogined size = %d",pMgr->m_affairListLogined.size());
				}
				else{
					++it;
				}
			}
		}//-lock area end-
		
		for (list<DoValidateAffair*>::iterator it = TempList_logined.begin();it!=TempList_logined.end();++it)
		{
			delete (*it);//delete the affair should remove
		}
		TempList_logined.clear();
	}
	return 0;
}

VOID DoValidateAffairMgr::Exit()
{
	m_bExit = TRUE;
	DWORD dwWait = WaitForSingleObject(m_hThdProcessList,1000);
	if (dwWait == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThdProcessList,1);
		m_hThdProcessList = (HANDLE)0;
	}
}

BOOL DoValidateAffairMgr::IsUserOnline( DWORD dwUserID )
{
	BOOL bFind = FALSE;
	{
		CAutoLock lock(*m_pListCriticalSection);
		for (list<DoValidateAffair*>::iterator it = m_affairListLogining.begin();it != m_affairListLogining.end(); ++it)
		{
			if ((*it)->GetUserinfo()->dwUser_ID == dwUserID){
				bFind = TRUE;
				break;
			}
		}

		if (!bFind){
			for (list <DoValidateAffair*>::iterator it = m_affairListLogined.begin();it != m_affairListLogined.end();++it)
			{
				if((*it)->GetUserinfo()->dwUser_ID == dwUserID){
					bFind = TRUE;
					break;
				}
			}
		}
	}
	return bFind;
}

int DoValidateAffairMgr::GetLoginedUserCount()
{
	int iCount = 0;
	{
		CAutoLock lock(*m_pListCriticalSection);
		iCount = m_affairListLogined.size();
	}

	return iCount;
}

int DoValidateAffairMgr::GetLoginedUserInfo( ActiveUserInfo* pUserInfoList,int maxCount )
{
	int iCount = 0;
	ActiveUserInfo* uiOri = pUserInfoList;
	{
		CAutoLock lock(*m_pListCriticalSection);
		for(Affair_Iter it = m_affairListLogined.begin(); (it != m_affairListLogined.end())&&(iCount < maxCount);++it ){
			uiOri->dwUserID = (*it)->GetUserinfo()->dwUser_ID;
			StringCbCopy(uiOri->ProxyName,sizeof(uiOri->ProxyName),(*it)->GetUserinfo()->szProxyName);
			StringCbCopy(uiOri->szIp,sizeof(uiOri->szIp),(*it)->GetClientIp());
			StringCbCopy(uiOri->ConnectTime,sizeof(uiOri->ConnectTime),(LPCTSTR)((*it)->GetTimeDoValidateOk()->Format(L"%Y-%m-%d %H:%M:%S")));
			iCount ++;
			uiOri ++;
		}
	}
	return iCount;
}
DoValidateAffairMgr* getDvdtAffairMgrPtr()
{
	return &g_DvlidateAffairMgr;
}