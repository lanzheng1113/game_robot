#include "stdafx.h"
#include "ObjectFactory.h"
#include "TradeMacDispatcher.h"
#include "OrderDispatcher.h"
#include "OrderControlerCommon.h"
#include "MacsControlerCommon.h"
#include "settings.h"
#include "confirmdispatcher.h"
#include <exception>
using std::runtime_error;
#include "DBM.h"
#include "IDataBaseObject.h"
#include "MySQLCppConDB.h"
#include "NVDSExDlg.h"

CObjectFactory::CObjectFactory()
{
	_main_dlg_ptr = NULL;
	_object_initial_complete = false;
	_database_interface = NULL;
}

CObjectFactory::~CObjectFactory()
{

}

void CObjectFactory::clear()
{
	if (_order_dispatcher)
	{
		delete _order_dispatcher;
		_order_dispatcher = NULL;
	}

	if (_Macs_dispatcher)
	{
		delete _Macs_dispatcher;
		_Macs_dispatcher = NULL;
	}

	if (_confirm_dispatcher)
	{
		delete _confirm_dispatcher;
		_confirm_dispatcher = NULL;
	}

	if (_database_interface)
	{
		delete _database_interface;
		_database_interface = NULL;
	}
}


void CObjectFactory::CreateDispatcher()
{
	ASSERT(_main_dlg_ptr != NULL);

	vector<type_game_settings> _settings_game_support = CSettings::getInstance().getSettingsGameTypes();
	COrderDispatcher *order_dispacher = new COrderDispatcher();
	if (!order_dispacher){
		throw runtime_error("为派发器分配内存失败！");
	}
	/*@FIXME:在这里创建具体处理订单的controler*/
	for (int i=0; i!=_settings_game_support.size();++i)
	{
		if (_settings_game_support[i]._game_type_id != BAD_GAME_ID)
		{
			COrderControlerCommon* controler = new COrderControlerCommon(_settings_game_support[i]._game_type_id,
						_settings_game_support[i]._game_type_describe,
						(dispatch_stage)CSettings::getInstance().orderDispatchStageFromString(_settings_game_support[i]._acc_get_stage));
			if (!controler)
			{
				throw runtime_error("为订单分派控制器单元分配内存失败！");
			}
			controler->setLogListenerCallback(boost::bind(&CNVDSExDlg::add_item,(CNVDSExDlg*)_main_dlg_ptr,_1,_2,_3,_4));
			order_dispacher->AddControler(controler);
		}
	}
	_order_dispatcher = order_dispacher;

	CTradeMacDispatcher* macs_dispather = new CTradeMacDispatcher();
	if (!macs_dispather){
		throw runtime_error("为派发器分配内存失败！");
	}
	macs_dispather->SetOrderDispatcher(_order_dispatcher);
	
	/*@FIXME:在这里创建具体负责分派机器的controler*/
	for (int i=0; i!=_settings_game_support.size();i++){
		if (_settings_game_support[i]._game_type_id != BAD_GAME_ID){
			CMacsControlerCommon* controler = new CMacsControlerCommon(_settings_game_support[i]._game_type_id,
				_settings_game_support[i]._game_type_describe,
				_settings_game_support[i]._is_can_muti_client);
			if (!controler)
			{
				throw runtime_error("为机器派发器单元分配内存失败！");
			}
			macs_dispather->AddClientMacControler(controler);
		}
	}
	_Macs_dispatcher = macs_dispather;

	vector<int> game_support_id_list;
	for (int i=0; i!=_settings_game_support.size(); i++)
	{
		if (_settings_game_support[i]._game_type_id != BAD_GAME_ID)
		{
			game_support_id_list.push_back(_settings_game_support[i]._game_type_id);
		}
	}
	_confirm_dispatcher = new CConfirmDispatcher();
	_confirm_dispatcher->initOrderList(game_support_id_list);
	_confirm_dispatcher->setLogListenerCallback(boost::bind(&CNVDSExDlg::add_item,(CNVDSExDlg*)_main_dlg_ptr,_1,_2,_3,_4));
}

BOOL CObjectFactory::CreateDataBaseObj(const CString& strDbName,const CString& strUserName,
									   const CString& strPassword,ADODB_DataBaseType dbType)
{
	if (_database_interface)
	{
		return FALSE;
	}

	if (dbType == E_DB_TYPE_ACCESS)
	{
		_database_interface = new CTDatabaseObject();
	}else{
		//_database_interface = new CTDatabaseObject();
		_database_interface = new CMySQLCppConDB();
	}
	//_database_interface = new CTDatabaseObject();

	ASSERT(_confirm_dispatcher != NULL);
	_database_interface->_event_confirm_order_loaded += boost::bind(&CConfirmDispatcher::ECBK_ConfirmsLoaded,_confirm_dispatcher,_1,_2,_3);
	_database_interface->_event_trade_order_loaded += boost::bind(&COrderDispatcher::ECBK_TradeOrderLoaded,_order_dispatcher,_1,_2,_3);
	return _database_interface->Init(strDbName,strUserName,strPassword,dbType);
}
