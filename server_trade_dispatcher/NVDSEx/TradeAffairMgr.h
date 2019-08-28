#pragma once
#include "TradeAffair.h"

#include <list>
using std::list;

class CLock_CS;

class CTradeAffairMgr
{
public:
	CTradeAffairMgr(void);
	~CTradeAffairMgr(void);
public:
	typedef list<CTradeAffair*> AffairList;
	typedef list<CTradeAffair*>::const_iterator const_AffairList_Iter;
	typedef list<CTradeAffair*>::iterator AffairList_Iter;

	void AddAffair(CTradeAffair* afr);
	//void RemoveAffair(AssistAffair* afr);
	void Run();
	void Close(){m_bClosed = true;}
protected:
	static UINT Thdfun_UserListDeamon(LPVOID lpThis);
private:
	void processAffairList();
private:
	AffairList m_AssistAffiarlist;
	bool m_bClosed;
	HANDLE m_hThread;
	CLock_CS* m_listLocker;
};


CTradeAffairMgr* afkGetTradeAffairManager();