#pragma once

#include <list>
using std::list;

class DoValidateAffair;
class CLock_CS;
class DBM;

struct ActiveUserInfo;


class DoValidateAffairMgr
{
public:
	DoValidateAffairMgr(void);
	~DoValidateAffairMgr(void);
public:
	typedef list<DoValidateAffair*>::iterator Affair_Iter;
public:
	BOOL RegisterAffair(DoValidateAffair* paff);
	BOOL UnRegisterAffair(DoValidateAffair* paff);
	
	VOID Run();
	VOID Exit();

	VOID DoValidateOK(DoValidateAffair* paff);
	BOOL IsUserOnline(DWORD dwUserID);
	
	int GetLoginedUserCount();

protected:
	static UINT process_thread(LPVOID lpThis);
private:
	list<DoValidateAffair*> m_affairListLogining;
	list<DoValidateAffair*> m_affairListLogined;
	CLock_CS* m_pListCriticalSection;
	BOOL m_bExit;
	HANDLE m_hThdProcessList;
	DBM* m_pDbm;
};

DoValidateAffairMgr* getDvdtAffairMgrPtr();