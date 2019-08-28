#include "stdafx.h"
#include "OrderControlerCommon.h"
#include "dbm.h"
#include "Settings.h"
#include "util/Logger.h"
#include "../common/DebugMessage.h"
list<CTradeOrder*> COrderControlerCommon::loadOrders( int maxCountPerBatch )
{
	list<CTradeOrder*> ret;

	if (!getIsEnable())
	{
		return ret;
	}

	if (isExit())
	{
		return ret;
	}

	if(CheckDataSourceIdle()){
		return ret;
	}

	afkGetDbObject()->GetTradeNoneGetOrder(getGameTypeId(),maxCountPerBatch);

	return ret;
}

void COrderControlerCommon::WriteEvent( const CTradeOrder& order,__time64_t trade_time,int balances,int trade_result_code_id )
{

}

COrderControlerCommon::COrderControlerCommon( int game_type_id,const string& game_type_describe,dispatch_stage dispatchstage )
:CTradeOrderControler(game_type_id,game_type_describe,dispatchstage)
{

}
