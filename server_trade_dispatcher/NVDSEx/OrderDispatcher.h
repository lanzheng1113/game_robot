#pragma once
#include <vector>
using std::vector;
#include "type_game_setting.h"
#include "tradeorder.h"
class CTradeOrderControler;

/**
* @class accDispatcher
* accDispatcher 作为游戏账号派发器基类
*/
class COrderDispatcher : public NonCopyable
{
public:
	COrderDispatcher(){}
	~COrderDispatcher(){;}
protected:
public:
	void PreLoadOrders();
	//virtual void Reset(const vector<type_game_settings>& game_setting);//修改配置档后直接重新导入列表，保留
	void exit();
	/**
	 * 取得游戏类型ID为gameType的下一个订单
	 * @param gameType:游戏类型
	 * @return 成功返回订单对象指针,失败返回NULL
	 */
	CTradeOrder* GetNextOrder(int gameType);

	void AddControler(CTradeOrderControler* collection);
	void EnableDispatcher(BOOL enable);
	int getNowTradeListCount();
	void ECBK_TradeOrderLoaded(IDatabaseObject* sender,int game_type,list<CTradeOrder*> orderList);
private:
	vector<CTradeOrderControler*> _controlers;
};
