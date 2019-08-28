#pragma once

#include <vector>
using std::vector;
#include "util/Singleton.h"
#include "type_game_setting.h"

class COrderDispatcher;
class CTradeMacDispatcher;
class CConfirmDispatcher;
class IDatabaseObject;

class CObjectFactory : public Singleton<CObjectFactory>
{
public:
	CObjectFactory();
	~CObjectFactory();
public:
	void setMainWndPtr(void* pMainWnd){_main_dlg_ptr = pMainWnd;}
	void CreateDispatcher();		//�������ô����������ɿ�����,�������ô����������ɿ�����
	BOOL CreateDataBaseObj(const CString& strDbName,const CString& strUserName,const CString& strPassword,ADODB_DataBaseType dbType);

	COrderDispatcher* GetOrderDispatcher()const{ return _order_dispatcher; }
	CTradeMacDispatcher* GetMacDispatcher()const{ return _Macs_dispatcher; }
	CConfirmDispatcher* GetConfirmDispatcher() const{ return _confirm_dispatcher; }
	IDatabaseObject* getDatabaseInterface() const { return _database_interface; }

	bool isObjectInit(){return _object_initial_complete;}
	void setObjectInit(bool inited){_object_initial_complete = inited;}

	void clear();
private:
	COrderDispatcher* _order_dispatcher;					//����������
	CTradeMacDispatcher* _Macs_dispatcher;					//����������
	CConfirmDispatcher* _confirm_dispatcher;				//����ɷ���
	IDatabaseObject* _database_interface;
	void* _main_dlg_ptr;
	bool _object_initial_complete;
public:
private:
	
};