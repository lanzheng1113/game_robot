#pragma once
 
#include "wares.h"

enum EClientRoleType{
	CLIENT_TYPE_SELLER,
	CLIENT_TYPE_BUYER,
};

#define		OrderTypeTrade			0
#define		OrderTypeConfirm		1

struct order_struct{
	int order_dispatch_id;
	int game_type;
	char order_id[32];
	char acc[32];
	char pswd[32];
	char role[32];
	char world[48];
	char trade_place[48];
	char buddy_role[32];
	unsigned long trade_money;
	int time_out_second;
	EClientRoleType trade_role;
	unsigned char is_trade_money;
	unsigned char retry_times_remain;
	char role_pswd[40];//角色密码
	char ware_pswd[40];//仓库密码
	int login_model;//登录模式 值为1表示QQ登录，值为2表示网易登录
	unsigned short order_type;
	Wares trade_ware_detail;
};

inline bool operator == (const order_struct& o1,const order_struct& o2){
	return o1.order_dispatch_id == o2.order_dispatch_id;
}
inline bool operator != (const order_struct& o1,const order_struct& o2){
	return !(o1 == o2);
}

struct LOGIN_MESSAGE 
{
	int client_mac_id;
	char szName[32];
};

struct NET_HEAD 
{
	unsigned long HeadFlag;		//头标志
	unsigned long GameType;		//游戏类型
	unsigned long MesgFullType;	//高字为MESG_TYPE_2、低字为MESG_SUBTYPE_2
	unsigned long PacketLen;	//包长
	unsigned long CheckSum;		//校验和
};

struct ACK_NET_HEAD{
	unsigned long AckMesgFullType;
	unsigned long GameType;
	unsigned long DataSizeRemain; //本次会话剩下需要发送的总数据量
	unsigned short  LastPacket;
};

struct ACQUARE_ORDER_FAILED_ACK_MSG{
	unsigned long error_code;
};

struct ACQUIRE_ORDER_MSG{
	int burden;
};

struct HEART_MSG{ //心跳包附带一个负载信息
	int burden;
};

struct ASSIGN_ORDER_MSG{
	order_struct os;
};

typedef ASSIGN_ORDER_MSG ASSIGN_CONFIRM_MSG;

struct ASSIGN_ORDER_FAILED_MSG{
	int error_code;
};

struct TRADE_STATE_CHANGED_MSG{
	int order_dispatch_id;		//< 标示是哪一个订单
	int state_code;				//< 当前进度信息代码
	int result_id;				//< 客户端执行成功为MAC_PROCESSING_SUCCESS否则表示错误的代码
};

struct CONFIRM_STATE_CHANGED_MSG 
{
	int confirm_dispatch_id;
	int state_code;
	int result_id;
};

struct ACK_TRADE_STATE_CHANGED 
{
	int order_dispatch_id;		//< 标示是哪一个订单
	int error_code;				//< 收到该客户端执行的结果后，服务器的处理结果。
};

typedef ACK_TRADE_STATE_CHANGED ACK_COMFIRM_STATE_CHANGED;

struct ACK_BUDDY_FAILED
{
	int order_dispatch_id;
	short buddy_failed_code;
	short reserve;
};

struct CONFIRM_CANCEL{
	int order_dispatch_id;
};

//< -2，0，-1已经被占用作为特殊用途。不允许使用它作为错误ID。
//< 客户端状态改变，TRADE_STATE_CHANGED的result_id
#define MAC_PROCESSING_SUCCESS									0x201	//<交易的前一步骤成功完成。
//< 引起交易错误的原因ID，0x10X是固定的，0x800之后的自行定义。
enum{
	ECC_GAME_COLLAPSE = 0x100,			//客户端崩溃，具体哪种原因不晓得。注意交易过程不能崩溃，要是崩溃了就麻烦了。
	ECC_LOGIN_SERVER_ERROR = 0x101,		//服务器维护、繁忙、etc.
	ECC_ACC_OR_PSWD_INCORRECT = 0x102,	//账号密码错误
	ECC_ROLE_NOT_EXIST = 0x103,			//丫的角色根本不存在
	ECC_CANNOT_ENTER_TRADE_STAGE_DANGE = 0x104,	//在危险区域导致该角色无法进入交易区域【例如在PK区，或者是在怪物等级很高的区域】
	ECC_CANNOT_ENTER_TRADE_STAGE_OTHER = 0x105,	//其他原因导致角色无法进入交易区域【例如等级需求或者是红名，等等等等】
	ECC_BUYER_MONEY_NOT_ENOUGH = 0x106,	//买方没钱交易个屁
	ECC_SELLER_HAS_NO_WAIR=0x107,			//卖方没有指定的货物
	TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL = 0x108,//服务器要求立即取消订单
	TRADE_FAILED_REASON_SERVER_COMMAND_DENY = 0x109,//服务器拒绝了交易请求
	TRADE_FAILED_REASON_BUDDY_IS_FAILED = 0x10A,//服务器命令取消交易，因为交易对象发生了错误
	TRADE_FAILED_REASON_BUDDY_DISCONNECTED = 0x10B,//服务器命令取消交易，因为交易对象断线
	TRADE_FAILED_REASON_I_AM_DISCONNECTED_NOT_TRADE = 0x10C,//交易客户端意外断线,可以确定它没有交易。
	RETRY_TIME_END = 0x10D,	//达到重试次数仍未交易成功
	TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW = 0x10E,//交易客户端意外断线，不能确定是否已经执行完交易。
	ECC_UNKNOW_1 = 0x800,
	ECC_UNKNOW_2 = 0x801,
	ECC_UNKNOW_3 = 0x802,
	ECC_UNKNOW_4 = 0x803,
	ECC_UNKNOW_5 = 0x804,
	ECC_UNKNOW_6 = 0x805,
};