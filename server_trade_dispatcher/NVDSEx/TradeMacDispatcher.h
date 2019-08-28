#pragma once
#include "util/NonCopyable.h"
#include "tradeClientMachine.h"
#include "type_game_setting.h"
#include "../common/shareStruct.h"
class DateTime;
class COrderDispatcher;
struct order_struct;

#define __cbk

class CTradeMacDispatcher : public NonCopyable
{
public:
	CTradeMacDispatcher();
	~CTradeMacDispatcher();
public:
	void AddClientMacControler(CClientMacControler* controler);

	COrderDispatcher* GetOrderDispatcher() const { return order_dispatcher; }
	void SetOrderDispatcher(COrderDispatcher* val) { order_dispatcher = val; }

	bool addMac(int game_type,CTradeClient* mac);

	/**
	 * �յ�һ���ͻ���ϣ����ö�������Ϣ
	 */
	int DealTradeRequest(CTradeClient* mac,int mac_burden);

	/**
	 * ��õ�ǰ���ڴ���Ķ�������
	 */
	int getCurrentProcessOrderCount() const;

	int GetCurrentProcessConfirmCount() const;
	/**
	 *
	 */
	CTradeClient* getMac(int game_id, int c_id, const string& name);

	string dump()const;

	void exit();			//�����˳�
	void force_exit();		//�����˳�
private:
	/**
	 * �ɷ�һ�������������ɷ�����̨������
	 * @remark:������࿪ʱ���������������ͬһ̨������
	 */
	int dispatchOrder(CTradeOrder* order,CTradeClient* client);
	void corder_to_order_struct(IN CTradeOrder* order,OUT order_struct* o_struct);
	/**/
	int dispatchConfirm(CConfirmOrder* confirm,CTradeClient* client);
	void confirm_to_order_struct(CConfirmOrder* confirm,order_struct* o_struct);
protected:
private:
	COrderDispatcher* order_dispatcher;						//< �˺��ɷ��������п��еĿͻ��˴���ʱ����������ȡ�˺š�
	vector<CClientMacControler*> _client_mac_controlers;	//< ��Ϸ�����б�
};

