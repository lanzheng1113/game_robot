#pragma once
#include "tradeClientMachine.h"

class CMacsControlerCommon : public CClientMacControler
{
public:
	CMacsControlerCommon(int game_type_id,const string& game_type_describe,bool is_muti_client_game);
	~CMacsControlerCommon();
public:
//	void DoOrderCanceled(CTradeOrder* order,int reason);
	string dump() const;
protected:
private:
};

