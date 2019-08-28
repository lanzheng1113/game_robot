#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <list>
using std::list;
#include "util/NonCopyable.h"
#include "../common/wares.h"
#include "util/DateTime.h"
#include "util/Event.h"
#include "util/Trackable.h"
#include "../common/TradeMesgDef.h"
#include "../common/shareStruct.h"
#include <boost/function.hpp>
enum EEmgcLevel{
	//�����Ľ����̶�
	EEL_NORMAL = 0,		//< ����
	EEL_FAST,			//< �Ӽ�
	EEL_FASTER,			//< ʮ���
	EEL_FASTEST			//< ����ĩ��
};

enum EOrderState{
	EOS_GETFROM_DB,			//< �����ݿ���ȡ������ʼ״̬
	EOS_SEND_TO_DISPATCHER, //< ��ȡ�����ɷ���
	EOS_DISPATCHED,			//< �Ѿ����ɷ������׿ͻ���
	EOS_READY_TO_TRADE,		//< ���ߵĿͻ��˶��Ѿ�������׼���ý���
	EOS_TRADE_COMPLETE,		//< �����Ѿ����
};

class COrderRecorder;
class CTradeClient;

class CTradeOrder : public NonCopyable
{
public:
	CTradeOrder();
	~CTradeOrder();
public:
	/*������Щ�ڶ����ķ�������в��ᷢ���ı䣬���ֱ����Ϊpublic����*/
	int _game_type;
	string _order_id;				//< ����ID�����貢��ȫ����Ϸ������һ��������Ψһֵ�����������ظ�
	bool isValid(){ return _order_id != "bad_id"; }
	EClientRoleType _tradeRole;
	string _acc;
	string _pswd;
	string _role_name;
	string _buddy_role_name;			//< ���׶���Ľ�ɫ��
	string _world;						//< ������Ϣ
	string _trade_place;				//< ��ͷ�ص�
	Wares _trade_good_detail;			//< ������Ʒ�Լ��۸�ľ�����Ϣ������
	EEmgcLevel _emergency_level;		//< ���ȼ�������Խ�����ȼ�Խ�ߡ������ȼ��Ķ�������������õ���Դ���ڴ���	
	bool _type_trade_is_money;			//< ���������Ƿ�Ϊ��Ǯ���ף�Ĭ���ǡ�
 	Event<void (CTradeOrder*,EOrderState)> _event_order_state_changed;
	string _log;
public:
	EOrderState getCurState() const { return _cur_state; }
	void setCurState(EOrderState val);
	CTradeClient* getClient() const { return _client; }
	void setClient(CTradeClient* val) { _client = val; }
	short getTradeResult() const { return _trade_result; }
	void setTradeResult(short val) { _trade_result = val; }
	int getRetryTimes() const { return _retry_times; }
	void setRetryTimes(int val) { _retry_times = val; }
	DateTime getGetTime() const { return *_get_time; }
	void setGetTime(const DateTime& val) { *_get_time = val; }
	DateTime getDispatchedTime() const { return *_dispatched_time; }
	void setDispatchedTime(const DateTime& val) { *_dispatched_time = val; }
	DateTime getTradeTime() const { return *_trade_time; }
	void setTradeTime(const DateTime& val) { *_trade_time = val; }
	EClientOrderProgress getProgressClient() const { return _progress_client; }
	void setProgressClient(EClientOrderProgress val) { _progress_client = val; }
	CTradeOrder* getBuddyOrder() const { return _buddy_order; }
	void setBuddyOrder(CTradeOrder* val) { _buddy_order = val; }
	int getDispatchId() const { return _dispatch_id; }
	void setDispatchId(int val) { _dispatch_id = val; }
	int getTradeMoney() const { return _trade_money; }
	void setTradeMoney(int val) { _trade_money = val; }
	BOOL getIs_log_to_text() const { return _is_log_to_text; }
	void setIs_log_to_text(BOOL val) { _is_log_to_text = val; }
	void resetOrderState(){_cur_state = EOS_GETFROM_DB;}
	string getOrderParams() const { return _order_params; }
	void setOrderParams(string val) { _order_params = val; }
public:
	string dump() const;
 	static string EOrderStateToString(EOrderState stat);	
 	static string EEmgcLevelToString(EEmgcLevel elv);
 	void initlog();
 	void log(const string& val);

	VOID					Remove() { InterlockedExchange( (long*)&_isRemove, TRUE ); }
	VOID					ResetKillFlag() { InterlockedExchange( (long *)&_isRemove, FALSE ); }///< Reset remove flag.if m_bRemove is TRUE,it will be removed in active session list.
	BOOL					ShouldBeRemoved() { return _isRemove; }///<Access remove flag.
private:
	BOOL _is_log_to_text;
	BOOL _isRemove;
	int _trade_money;
	int _dispatch_id;
	/*���������ڶ����ɷ������лᷢ���ı䣬�����Ҫ��֤�̰߳�ȫ*/
	EOrderState _cur_state;				//< ��ǰ���������Ĵ�����̡�
	CTradeClient* _client;				//< ����ö����Ŀͻ���
	short _trade_result;				//< ���׻����صĴ������������ʼ��Ϊ0[δ����]����ֵ�Ľ��ʹ������ж�ȡ��
	int _retry_times;					//< ��������ʧ�ܺ�Ŀ����Դ���
	/*����������Ϊ�˱���ָ����󣬷�ֹ�����ã����緢��_dispatchd_time = &xxx_time���������ȷʹ����*_dispached_time = xxx_time*/
	DateTime* _get_time;				//< �Ӷ�����������õ�ʱ�䣬Ϊ���󴴽�ʱ��ʱ��
	DateTime* _dispatched_time;			//< �����������ͻ�����ʱ�䡣δ������ʱ��ֵΪ2000-1-1 00:00:00
	DateTime* _trade_time;				//< ִ�н��׵�ʱ�䣬��ʼֵΪ2000-1-1 00:00:00
	COrderRecorder* _recorder;
	EClientOrderProgress _progress_client;	//< ������Ϣ
	CTradeOrder* _buddy_order;
	string _order_params;
};

class CTradeOrders
{
public:
	CTradeOrder* orderSeller;
	CTradeOrder* orderBuyer;
protected:
private:
};

//��Ϊ��������ṹ�塣
// class CTradeOrder : public NonCopyable
// {
// public:
// 	CTradeOrder();
// 	~CTradeOrder();
// public:
// 	/*������Щ�ڶ����ķ�������в��ᷢ���ı䣬���ֱ����Ϊpublic����*/
// 	int _game_type;
// 	string _order_id;				//< ����ID�����貢��ȫ����Ϸ������һ��������Ψһֵ�����������ظ�
// 	bool isValid(){ return _order_id != "bad_id"; }
// 	
// 	string _acc_seller;				//< ���۷��ʺţ�����
// 	string _pswd_seller;			//< ���۷����룬����
// 	string _acc_buyer;				//< �����ʺţ�����
// 	string _pswd_buyer;				//< �������룬����
// 	string _role_name_seller;		//< ���۷���ɫ��
// 	string _role_name_buyller;		//< ����
// 	string _world;					//< ������Ϣ
// 	string _trade_place;			//< ��ͷ�ص�
// 	DateTime* _get_time;			//< �Ӷ�����������õ�ʱ�䣬Ϊ���󴴽�ʱ��ʱ��
// 	Wares _trade_good_detail;		//< ������Ʒ�Լ��۸�ľ�����Ϣ������
// 	EEmgcLevel _emergency_level;	//< ���ȼ�������Խ�����ȼ�Խ�ߡ������ȼ��Ķ�������������õ���Դ���ڴ���	
// 	bool _type_trade_is_money;		//< ���������Ƿ�Ϊ��Ǯ���ף�Ĭ���ǡ�
// 	string dump() const;
// 
// 	static string EOrderStateToString(EOrderState stat);	
// 	static string EEmgcLevelToString(EEmgcLevel elv);
// 	
// 	Event<void (CTradeOrder*,EOrderState)> _event_order_state_changed;
// 
// public:
// 
// 	void initlog();
// 
// 	void log(const string& val);
// 	/*�̰߳�ȫ��������Ϊ����������ǣ�浽����˫�������˫���ڲ�ͬ���߳���ͬʱ������Ϣʱ���ܵ��¾�����*/
// 	EOrderState getCurState() const { return _cur_state; }
// 	void setCurState(EOrderState val);
// 
// 	int getTradeResult() const { return _trade_result; }
// 	void setTradeResult(int val) { _trade_result = val; }	//< @FIXME:������Ҫ�޸�����ʹ��InterlockedExchange
// 	
// 	CTradeClient* getBuyerMachine() const { return _buyer_machine; }
// 	void setBuyerMachine(CTradeClient* val) { _buyer_machine = val; }	//< @FIXME:������Ҫ�޸�����ʹ��InterlockedExchange
// 
// 	CTradeClient* getSellerMachine() const { return _seller_machine; }
// 	void setSellerMachine(CTradeClient* val) { _seller_machine = val; }	//< @FIXME:������Ҫ�޸�����ʹ��InterlockedExchange
// 
// 	EClientOrderProgress getSellerProgress() const { return _progress_seller; }
// 	void setProgressSeller(EClientOrderProgress val) { _progress_seller = val; }
// 
// 	EClientOrderProgress getBuyerProgress() const { return _progress_buyer; }
// 	void setProgressBuyer(EClientOrderProgress val) { _progress_buyer = val; }
// 
// 	short getSellerResultCode() const { return _seller_result_code; }
// 	void setSellerResultCode(short val) { _seller_result_code = val; }
// 	
// 	short getBuyerResultCode() const { return _buyer_result_code; }
// 	void setBuyerResultCode(short val) { _buyer_result_code = val; }
// 
// 	int getRetryTimes() const { return _retry_times; }
// 	void setRetryTimes(int val) { _retry_times = val; }	//< @FIXME:������Ҫ�޸�����ʹ��InterlockedExchange
// 
// 	/*����ָ����󲻱��ƻ�����_dispatchd_time = &xxx_time�����_dispatchd_timeָ��ͱ��ƻ���*/
// 	DateTime getDispatchedTime() const { return *_dispatched_time; }
// 	void setDispatchedTime(const DateTime& val) { *_dispatched_time = val; }
// 	DateTime getTradeTime() const { return *_trade_time; }
// 	void setTradeTime(const DateTime& val) { *_trade_time = val; }
// 
// private:
// 	/*���������ڶ����ɷ������лᷢ���ı䣬�����Ҫ��֤�̰߳�ȫ*/
// 	EOrderState _cur_state;			//< ��ǰ���������Ĵ�����̡�
// 	CTradeClient* _seller_machine;			//< ����ö����ĳ��ۻ���
// 	CTradeClient* _buyer_machine;			//< �����
// 	int _trade_result;				//< ���׻����صĴ������������ʼ��Ϊ0[δ����]����ֵ�Ľ��ʹ������ж�ȡ��
// 	int _retry_times;				//< ��������ʧ�ܺ�Ŀ����Դ���
// 	/*����������Ϊ�˱���ָ����󣬷�ֹ�����ã����緢��_dispatchd_time = &xxx_time���������ȷʹ����*_dispached_time = xxx_time*/
// 	DateTime* _dispatched_time;		//< �����������ͻ�����ʱ�䡣δ������ʱ��ֵΪ2000-1-1 00:00:00
// 	DateTime* _trade_time;			//< ִ�н��׵�ʱ�䣬��ʼֵΪ2000-1-1 00:00:00
// 	COrderRecorder* _recorder;
// 	EClientOrderProgress _progress_seller;
// 	EClientOrderProgress _progress_buyer;
// 	short _seller_result_code;		//�������������������
// 	short _buyer_result_code;		//�򷽴��������������
// };

enum dispatch_stage{
	one_by_one_sort_in_time,
	by_emergency_level,
};

class CLock_CS;

/**
 * @class CTradeOrderControler
 * Ϊ֧�ֶ���������Ϸ��ʹ��CTradeOrderControler��������Ԫ��������һ����Ϸ��Ӧһ�ֶ�������Ԫ��
 * 
 * ����
 * 1����Զ�̽ӿڻ�ö�Ӧ��Ϸ�Ķ�����Ϣ�������浽�������б��С�
 *		���׶��������ݿ���ȡ����ʱ�򽫱���Ϊ����˫�����������Ķ������е����Ĵ�������ӷ�����ȡ����N����������ôʵ���ɷ����ͻ�������2N����
 * 2�������׻����󶩵�ʱ���������ɷ������׻���
 *		����ȡ����˳�������Ƚ��ȳ���
 * 3��������ɣ��ɹ�����ʧ�ܣ��󽫽��д�뵽���ݿ⡣�����ظ�Զ�̽ӿ�
 * 
 */
class CTradeOrderControler : public NonCopyable,public Trackable
{
public:
	CTradeOrderControler(int game_type_id,const string& game_type_describe,dispatch_stage dispatchstage);
	virtual ~CTradeOrderControler();
protected:
public:
	//���Է��ʽӿ�
	int getGameTypeId() const { return _game_type_id; }					//< �����Ϸ���ͣ�ֻ�����ڶ��󴴽�ʱ����ʼ��
	string getGameTypeDescribe() const { return _game_type_describe; }	//< �����Ϸ���������ַ�����ֻ�����ڶ��󴴽�ʱ����ʼ��
	dispatch_stage getStage() const { return _stage; }					//< ��÷�����ԣ�ֻ�����ڶ��󴴽�ʱ����ʼ��
	int getTradeListCount() const{ return _trade_order_list.size();}	//< ��õ�ǰ�б����
	void setDataSourceEmpty(long time_out){								//< ��δ��time_out�����ڱ��ɷ���������get Order����
		InterlockedExchange(&_is_in_source_empty,1);
		_source_empty_timeout = time_out;
		_time_set_empty = GetTickCount();
	}

	BOOL getIsEnable() const { return _isEnable; }
	/**
	 * �ر��ɷ��� will call recycleOrder.
	 * @remark:�����˳�ʱ�����âٽ�δ�ɷ����˺�д�뵽���ݿ�ڱ�����ɷ���δ�յ����׳ɹ���Ϣ���˺ţ���д�뵽���ݿ�
	 */
	void exit();
	void Enable(BOOL enable){InterlockedExchange((long*)&_isEnable,enable);}

public: //EVENT callbacks.
	//����CTradeOrder��״̬�ı䣬ECbk_OrderStateChanged���󶨵�CTradeOrder״̬�ı��¼�_event_order_state_changed�ϣ���CTradeOrder��״̬�ı�ʱ�˻ص������á�
	void ECbk_OrderStateChanged(CTradeOrder* sender,EOrderState state);

	void setLogListenerCallback(boost::function<void (const string&,const string&,short,short)> val) { _fun_new_log = val; }
public:
	/*
	 * �����˺ŵ���ǰ�б���
	 * @param maxCountPerBatch ����һ�ε��������ٸ���
	 * @return ���ر��ε���Ĵ����׶����б�ע�ⲻ�Ǻϲ�����ڲ��б�
	 */
	virtual list<CTradeOrder*> loadOrders(int maxCountPerBatch) = 0;
	
	/**
	 * ��¼���׽��,ͳ�ƽ��׽���ȵȡ�
	 * @param 
	 */
	virtual void WriteEvent(const CTradeOrder& order,__time64_t trade_time,int balances,int trade_result_code_id) = 0;
	/**
	 * �����һ�������ɷ����˺š�
	 * @param acc_to_trade ���ص��˺�
	 * @return �ɹ�����true,ʧ�ܷ���false.
	 * @remark: ע�����˺ű�ȡ�������ɷ����б��ڸ��˺ŵ�״̬��Ҫ�ı䣬�����ظ���
	 */
	virtual CTradeOrder* GetNextOrder();

	/**
	 * ����order�����ݿ⣬����can_be_fetch�����Ƿ��������ݿ��getState�ֶΣ����ֶξ������ݿ���order�Ƿ��ȡ������
	 * @param order:����
	 * @param can_be_fetch:��Ϊtrue���order����ȫ���յ����ݿ���getState�ֶα�����Ϊ0����false�������״̬
	 */
	virtual void saveOrder(CTradeOrder* order,bool can_be_fetch = false);
	void saveOrderLog(CTradeOrder* seller,CTradeOrder* buyer);
	bool isExit() const { return _exit_flag; }
	void tradeOrderLoaded(list<CTradeOrder*>& oli);
protected:
	/**
	 * ����Ƿ�ɴ�����Դ��ö�����
	 */
	bool CheckDataSourceIdle();
	
	/**
	 * ��ӵ��б�ӿ�,���б������˺���ֱ�ӷ��ء�
	 * @param order: ����
	 * @remark:��Ӷ������б�ʱ������б����Ѿ���ͬһ�����ŵĶ�������ֱ�ӷ��ء�
	 */
	virtual bool addOrder(CTradeOrder* order);
	
	/**
	 * �ϲ�order_list���ڲ��б���Ӻ�֤���ظ���order_id
	 */
	virtual bool addOrder(list<CTradeOrder*> order_list);
	
	/**
	 * ���б�ɾ��ɾ����û�иö���Ҳ�����ش���
	 * @param order_id:����id
	 * @remark  �綩������ɾ�����ͷŸ�Order���ڴ棬�˺����̰߳�ȫ��
	 */
	virtual void delOrder(CTradeOrder* order);//�����Ŵ˽ӿ�

private:
	/**
	 * ���ն��������ݿ⣬һ����ڳ����˳�ʱ�����á�
	 */
	void exitRecyleOrder(CTradeOrder* order);
	/**
	 * �޶������Դ���
	 */
	bool orderCanRetry(CTradeOrder* order);

private:
	BOOL _isEnable;							//< ʹ�ܿ��أ���Ϊfalseʱ��������ɷ��б���ȡ�˺š�
	bool _exit_flag;						//< �˳���ʶ�������˳�����ִ���ɷ��͵����˺Ų�����
	int _game_type_id;						//< ��Ϸ����
	string _game_type_describe; 			//< ��Ϸ���͵���������
	list<CTradeOrder*> _trade_order_list;	//< �����б������ɳ����ظ�ֵ��������򽫷������д���
	dispatch_stage _stage;					//< ���Ų��ԡ�
	CLock_CS* _list_process_locker;			//< �б��������
	long _is_in_source_empty;				//< ����Դ�ձ�־����Ϊ����ԴΪ��(��Դ����������ʱ��������ǰ���޶���)��GetNextOrder��_time_set_empty+_source_empty_timeoutʱ�䵽��֮ǰ����false;
	long _source_empty_timeout;				//< ��ʱʱ�䣬��λΪ����
	unsigned long _time_set_empty;			//< ����_is_in_source_empty��־Ϊ1��ʱ��=GetTickCount();

	boost::function<void (const string&,const string&,short,short)> _fun_new_log;
};

