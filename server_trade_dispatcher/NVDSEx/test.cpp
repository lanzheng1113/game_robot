#include "stdafx.h"
#include "test.h"
#include "DebugMessage.h"
#include "ActiveConnectionInfo.h"
#include "../common/shareStruct.h"
#include "tradeClientMachine.h"
#include "NetworkCallBack.h"
#include "ActiveConnectionInfo.h"
#include "../common/ADODB.H"
#include "DBM.h"
#include "MacsControlerCommon.h"
#include "../common/NetBase.h"
#include "ObjectFactory.h"
#include "OrderControlerCommon.h"
#include "RemoteDataSource.h"
#include "Settings.h"
#include "../Common/StringTool.h"
#include "TradeAffair.h"
#include "TradeAffairMgr.h"
#include "TradeCodeDef.h"
#include "TradeMacDispatcher.h"
#include "../common/TradeMesgDef.h"
#include "TradeNetworkInterface.h"
#include "TradeOrder.h"
#include "type_game_setting.h"
#include "../common/wares.h"
#include "../common/AutoLock.h"
#include "OrderDispatcher.h"
#include "util/datetime.h"

void test_all_struct_size()
{
//	DebugMessageA("name = %s,size = %d","ActiveUserInfo",sizeof(ActiveUserInfo));
//	DebugMessageA("name = %s,size = %d","tagPACKET_HEADER",sizeof(tagPACKET_HEADER));
//	DebugMessageA("name = %s,size = %d","tagIOHANDLER_DESC",sizeof(tagIOHANDLER_DESC));
//	DebugMessageA("name = %s,size = %d","order_struct",sizeof(order_struct));
//	DebugMessageA("name = %s,size = %d","LOGIN_MESSAGE",sizeof(LOGIN_MESSAGE));
//	DebugMessageA("name = %s,size = %d","PACKETGENERALHEAD",sizeof(NET_HEAD));
//	DebugMessageA("name = %s,size = %d","ACKPACKETHEAD",sizeof(ACK_NET_HEAD));
//	DebugMessageA("name = %s,size = %d","ACQUARE_ORDER_FAILED_ACK_MSG",sizeof(ACQUARE_ORDER_FAILED_ACK_MSG));
//	DebugMessageA("name = %s,size = %d","ACQUARE_ORDER_MSG",sizeof(ACQUIRE_ORDER_MSG));
//	DebugMessageA("name = %s,size = %d","ASSIGN_ORDER_MSG",sizeof(ASSIGN_ORDER_MSG));
//	DebugMessageA("name = %s,size = %d","TRADE_STATE_CHANGED",sizeof(TRADE_STATE_CHANGED_MSG));
//	DebugMessageA("name = %s,size = %d","MacBurdenAppend",sizeof(MacBurdenAppend));
//	DebugMessageA("name = %s,size = %d","CTradeClientMachine::history_record",sizeof(CTradeClient::history_record));
//	DebugMessageA("name = %s,size = %d","goods_character_table",sizeof(goods_character_table));
//	DebugMessageA("name = %s,size = %d","goods_character",sizeof(goods_character));
//	DebugMessageA("name = %s,size = %d","Wares",sizeof(Wares));
//	DebugMessageA("name = %s,size = %d","NetworkCallBack",sizeof(NetworkCallBack));
//	DebugMessageA("name = %s,size = %d","affair",sizeof(affair));
//	DebugMessageA("name = %s,size = %d","CTDatabaseObject::tbset",sizeof(CTDatabaseObject::tbset));
//	DebugMessageA("name = %s,size = %d","CMacsControlerCommon",sizeof(CMacsControlerCommon));
//	DebugMessageA("name = %s,size = %d","COrderDispatcher",sizeof(COrderDispatcher));
//	DebugMessageA("name = %s,size = %d","CTradeMacDispatcher",sizeof(CTradeMacDispatcher));
//	DebugMessageA("name = %s,size = %d","CObjectFactory",sizeof(CObjectFactory));
//	DebugMessageA("name = %s,size = %d","CTradeOrderControler",sizeof(CTradeOrderControler));
//	DebugMessageA("name = %s,size = %d","CRemoteDataSource",sizeof(CRemoteDataSource));
//	DebugMessageA("name = %s,size = %d","CSettings",sizeof(CSettings));
//	DebugMessageA("name = %s,size = %d","CTDatabaseObject",sizeof(CTDatabaseObject));
//	DebugMessageA("name = %s,size = %d","CTradeAffairMgr",sizeof(CTradeAffairMgr));
//	DebugMessageA("name = %s,size = %d","CTradeClientMachine",sizeof(CTradeClient));
//	DebugMessageA("name = %s,size = %d","CTradeAffair",sizeof(CTradeAffair));
//	DebugMessageA("name = %s,size = %d","CLock_CS",sizeof(CLock_CS));
//	DebugMessageA("name = %s,size = %d","DateTime",sizeof(DateTime));
//	DebugMessageA("name = %s,size = %d","CTradeOrder",sizeof(CTradeOrder));
//	DebugMessageA("name = %s,size = %d","CTradeNetworkInterface",sizeof(CTradeNetworkInterface));
//	DebugMessageA("name = %s,size = %d","CompareBurden",sizeof(CompareBurden));
////	DebugMessageA("name = %s,size = %d","CTradeOrderProgress",sizeof(CTradeOrderProgress));
//	DebugMessageA("name = %s,size = %d","CClientMacControler",sizeof(CClientMacControler));
//	DebugMessageA("name = %s,size = %d","type_game_settings",sizeof(type_game_settings));
}
