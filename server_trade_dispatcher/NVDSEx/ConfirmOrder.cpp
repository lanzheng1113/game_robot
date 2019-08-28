#include "stdafx.h"
#include "ConfirmOrder.h"
#include "../common/NetBase.h"
#include "OrderRecorder.h"
#include "Settings.h"

extern int getNextGlobalOrderId();

CConfirmOrder::CConfirmOrder()
:_account(""),
_complete_time(1,1,2000,0,0,0),
_cur_client_progress(ECCP_standby),
_cur_state(ECS_GETFROM_DB),
_dispatch_time(1,1,2000,0,0,0),
_game_type_id(-1),
_get_time(1,1,2000,0,0,0),
_is_log_to_text(FALSE),
_log(""),
_money_confirm(-1),
_order_id("bad id!"),
_password(""),
_result_id(-1),
_recorder(NULL),
_role(""),
_world(""),
_client(NULL),
_retry_times(3)
{
	_dispatch_id = getNextGlobalOrderId();
}


CConfirmOrder::~CConfirmOrder()
{
	SAFE_DELETE(_recorder);
}


void CConfirmOrder::initlog()
{
	ASSERT(_game_type_id != -1);
	if (!_recorder){
		string logFileName = CSettings::getInstance().getDefOrderLogPath(_game_type_id);
		logFileName += "\\";
		logFileName += _order_id;
		_recorder = new COrderRecorder(logFileName);
	}
}

void CConfirmOrder::log( const string& val )
{
	DateTime dtNow;
	string strToLog = dtNow.toString() + " " + val + "\r\n";;

	_log += strToLog;
	if (_is_log_to_text){
		_recorder->Log(strToLog); //< ��¼�����������¼�
	}
}

void CConfirmOrder::setCurState( EConfirmState val )
{
	if (_cur_state != val)
	{
		_cur_state = val;
		_event_confirm_order_state_changed(this,val);
	}
}

string CConfirmOrder::EOrderConfirmStateToString( EConfirmState state )
{
	string str;
	switch (state)
	{
	case ECS_GETFROM_DB:
		str = "�����ݿ���ȡ������ʼ״̬";
		break;
	case ECS_SEND_TO_DISPATCHER:
		str = "��ȡ�����ɷ���";
		break;
	case ECS_DISPATCHED:
		str = "�Ѿ����ɷ���ȷ�Ͽͻ���";
		break;
	case ECS_COMPLETE:
		str = "�����ȷ��";
		break;
	default:
		str = "δ����Ľ���״̬��";
		break;
	}
	return str;
}
