#ifndef SHARE_STRUCT_H
#define SHARE_STRUCT_H
#include <string>
using std::string;

#define max_client_user_name_len 16
#define max_acc_len		32

class AccToProcessAndId
{
public:
	string _acc;		//该账号
	WORD _process_id;	//该账号对应的处理ID，服务器发还处理结果时用这个代表账号。
protected:
private:
};

struct AccInfoForProxy
{
	int id;
	char acc[max_acc_len];
	char szClientUserName[max_client_user_name_len];
	SYSTEMTIME DeadLine;
	SYSTEMTIME createTime;
	BYTE isValide;
};

struct WaiGuaPrice 
{
	long TypeId;
	double YueKaPrice;
	double ZhouKaPrice;
	char KardName[32];
};

struct CreateAccMsg 
{
	DWORD WgVersionId;
	DWORD accTypeId;
	DWORD createCount;
	char szUserClientName[max_client_user_name_len];
};

struct ChargeAccMsg{
	DWORD WgVersionId;
	DWORD accTypeId;
	DWORD chargeCount;
};

struct ChargeAccErrorDescribe{
	WORD process_id;
	int error_code;
};

struct switchAccErrorDescribe{
	char szAcc[max_acc_len];
	int error_code;
};

struct CreateAccErrorDescribe{
	WORD process_id;
	int error_code;
};

struct ProxyHistoryActive{
	int id;
	int type_id;
	SYSTEMTIME time;
	char event_describe[200];
	char detail_file_name[100];
};

enum{
	//返回-1 账号已经存在，返回-2账户余额不足，返回-3账号越界，返回-4测试卡数量不足。
	create_acc_error_unknown = -6,
	create_acc_error_count_0 = -5,
	create_acc_error_testkard_count_limit = -4,
	create_acc_error_balance_not_enough = -3,
	create_acc_error_already_exist = -2,
	create_acc_error_server_error = -1,
	create_acc_success,
};

enum{
	charge_acc_error_unknown = -100,
	charge_acc_error_count_0,
	charge_acc_error_cannot_charge_testkard,
	charge_acc_error_balance_not_enough,
	charge_acc_error_not_exist,
	charge_acc_error_server_error,
	charge_acc_success = 0,
};

enum{
	disable_acc_error_unknown = -100,
	disable_acc_success = 0,
};

enum{
	error_proxy_switch_acc_bad_len = -1000,
	error_proxy_switch_acc_beyond_max,
	error_proxy_switch_acc_count_0,
	error_proxy_switch_acc_count_not_equal,
	error_proxy_switch_acc_buf_size_error,
	error_proxy_switch_acc_not_yours,
	error_proxy_switch_new_acc_already_exist,

	switch_acc_error_unknow = -100,
	switch_acc_success = 0,
};

enum{
	proxy_event_id_create_acc = 100,
	proxy_event_id_charge_acc,
	proxy_event_id_swith_acc,
	proxy_event_id_del_acc,
};
#endif

