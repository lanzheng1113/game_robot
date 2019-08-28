#include "TradeOrder.h"

class COrderControlerCommon : public CTradeOrderControler
{
public:
	COrderControlerCommon(int game_type_id,const string& game_type_describe,dispatch_stage dispatchstage);
	virtual ~COrderControlerCommon(){;}
public:
	list<CTradeOrder*> loadOrders(int maxCountPerBatch);
	void WriteEvent(const CTradeOrder& order,__time64_t trade_time,int balances,int trade_result_code_id);
protected:
private:
};