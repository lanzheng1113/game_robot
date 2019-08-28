#pragma once
#include "util/Singleton.h"

class CRemoteDataSource :
	public Singleton<CRemoteDataSource>
{
public:
	int getOrder(int game_type_id);
	int getConfirm(int game_type_id);
private:
	void store_to_local_db(void* pdata,int size){;}
};