#pragma once
#include "util/Singleton.h"
#include "util/Trackable.h"
#include "util/NonCopyable.h"
#include "ConfirmOrder.h"
#include <map>
#include <list>
#include <vector>
using std::vector;
using std::map;
using std::list;

class CLock_CS;
class CConfirmOrder;
class IDatabaseObject;

class CConfirmDispatcher : public NonCopyable,public Trackable
{
public:
	CConfirmDispatcher(void);
	~CConfirmDispatcher(void);
public:
	void clear();
	CConfirmOrder* getNextConfirmOrder(int game_type);
	void initOrderList(const vector<int>& game_type_support);
	void ECbkConfirmStateChanged(CConfirmOrder* sender,EConfirmState state);
	void delConfirm(CConfirmOrder* confirm);
	void SaveConfirm(CConfirmOrder* confirm,bool getstate = false);
	void SaveConfirmLog(CConfirmOrder* confirm);
	void ExitRecyleConfirm(CConfirmOrder* confirm);
	void exit();
	void disable();
	int getCurConfirmCount();
	void preLoadConfirm();
	void ECBK_ConfirmsLoaded(IDatabaseObject* sender,int game_type,list<CConfirmOrder*> confirmList);	
	void setLogListenerCallback(boost::function<void (const string&,const string&,short,short)> val) { _fun_new_log = val; }
private:
	void LoadConfirm(int game_type);
	bool ConfirmCanRetry(CConfirmOrder* confirm);
	boost::function<void (const string&,const string&,short,short)> _fun_new_log;
private:
	map<int,list<CConfirmOrder*>*> _confirm_orders;
	CLock_CS* _cs_list_locker;
	bool _enable_flag;
	bool _exit_flag;
};
