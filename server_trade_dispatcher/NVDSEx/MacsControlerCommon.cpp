#include "stdafx.h"
#include "MacsControlerCommon.h"
#include "DebugMessage.h"
#include "util/StringEx.h"
#include "../common/CommonOrderCancelReason.h"
#include "TradeOrder.h"
#include "ObjectFactory.h"

CMacsControlerCommon::CMacsControlerCommon(int game_type_id,const string& game_type_describe,bool is_muti_client_game)
:CClientMacControler(game_type_id,game_type_describe,is_muti_client_game)
{

}

CMacsControlerCommon::~CMacsControlerCommon()
{

}

string CMacsControlerCommon::dump() const
{
	return CClientMacControler::dump();
}

// void CMacsControlerCommon::DoOrderCanceled( CTradeOrder* order,int reason )
// {
// 	switch (reason)
// 	{
// 	case COMMON_C_REASON_SELL_MAC_DISCONNECTD:
// 		{
// 			CTradeClientMachine* mac = order->getBuyerMachine();
// 			EClientRoleType roleType = CLIENT_TYPE_BUYER;
// 			break;
// 		}
// 	case COMMON_C_REASON_BUY_MAC_DISCONNECTD:
// 		{
// 			CTradeClientMachine* mac = order->getSellerMachine();
// 			EClientRoleType roleType = CLIENT_TYPE_SELLER;
// 			break;
// 		}
// 	default:
// 		break;
// 	}
// }
