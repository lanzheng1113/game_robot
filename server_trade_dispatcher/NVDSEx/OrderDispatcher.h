#pragma once
#include <vector>
using std::vector;
#include "type_game_setting.h"
#include "tradeorder.h"
class CTradeOrderControler;

/**
* @class accDispatcher
* accDispatcher ��Ϊ��Ϸ�˺��ɷ�������
*/
class COrderDispatcher : public NonCopyable
{
public:
	COrderDispatcher(){}
	~COrderDispatcher(){;}
protected:
public:
	void PreLoadOrders();
	//virtual void Reset(const vector<type_game_settings>& game_setting);//�޸����õ���ֱ�����µ����б�����
	void exit();
	/**
	 * ȡ����Ϸ����IDΪgameType����һ������
	 * @param gameType:��Ϸ����
	 * @return �ɹ����ض�������ָ��,ʧ�ܷ���NULL
	 */
	CTradeOrder* GetNextOrder(int gameType);

	void AddControler(CTradeOrderControler* collection);
	void EnableDispatcher(BOOL enable);
	int getNowTradeListCount();
	void ECBK_TradeOrderLoaded(IDatabaseObject* sender,int game_type,list<CTradeOrder*> orderList);
private:
	vector<CTradeOrderControler*> _controlers;
};
