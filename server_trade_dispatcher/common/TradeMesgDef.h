#pragma once

/*PACKET GENERAL HEAD中填充headFlag的标志，表示是哪一种业务。*/
#define message_tag_trade	0x12102011
#define message_tag_certain	0x12102012

/*填充PACKET GENERAL HEAD的Message Full Type，表示是该业务的具体什么类型的消息*/
#define MSG_TYPE_LOGIN				0x12345678 //< 登录
#define MSG_TYPE_LOGOUT				0x87654321 //< 登出

//关于审核订单分配的消息
#define		EMSG_DISPATCH_CONFIRM_SUCCESS							0x21100000	//< 服务器发给客户端，分配了审核订单
#define		EMSG_CONFIRM_STATE_CHANGED								0x21100001	//< 客户端发给服务器，订单状态改变
#define		EMSG_ACK_CONFIRM_STATE_CHANGED							0x21100002	//< 服务器发给客户端，订单状态改变的消息
#define		EMSG_CANCAL_ALL_CONFIRM_ORDERS							0x21100003  //< 服务器发给客户端，取消所有审核订单
#define		EMSG_CANCEL_A_CONFIRM_ORDER								0x21100004	//< 服务器发给客户端，取消一个客户端。
#define		EMSG_ACK_CONFIRM_HAS_GET_ORDER_X						0x21100005	//< 服务器发给客户端，state change espc for has_get_order!

//关于订单分配的MESSAGE FULL TYPE消息
#define MSG_TYPE_ACQUARE_ORDER									0x68551781	//< 客户端请求一个订单
#define EMSG_DISPATCH_SUCCESS									0x1000000	//成功返回一个订单
#define EMSG_DISPATCH_FAILED									0x1000011	//分配订单失败，以下是错误码
	#define EMSG_DISPATCH_ERROR_GAME_NOT_FOUND						0x1000001	//发生错误，该游戏未在服务列表中
	#define EMSG_DISPATCH_ERROR_NOT_FOUND_BUDDY						0x1000002	//..
	#define EMSG_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN			0x1000003	//..
	#define EMSG_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR 0x1000004	//..
	#define EMSG_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_BUYER_ERROR	0x1000005	//..

#define EMSG_CANCEL_ALL_ORDERS									0x1000100		//< 请退出所有正在处理的订单。
#define EMSG_CLIENT_CANCEL_ALL_OK								0x1000101		//< 附带正在处理的订单哪些已经完成，当收到这个响应后，服务器关闭与客户机的连接。

//客户端状态改变 客户端==>服务器，TRADE_STATE_CHANGED的MessageFullType
#define EMSG_MAC_STATE_CHANGE									0x1000201		//< 客户端状态改变，一下附加消息为TRADE_STATE_CHANGED.state_code
#define EMSG_ORDER_STATE_CHANGED_ACK							0x1000301		//< 客户端发来订单状态改变的消息后，发送给客户端的回应
#define EMSG_ACK_TRADE_HAS_GET_ORDER_X							0x1000401		//< state change espc for has_get_order!
#define EMSG_
//EMSG_ORDER_STATE_CHANGED_ACK消息携带的表示服务器响应的一个CODE
enum{
	E_ERRORCODE_SUCCESS,			//服务器成功记录订单的当前状态
	E_ERRORCODE_SERVER_REMOVED,		//发生错误，服务器已经将该订单从客户机的当前处理列表中删除。
};

enum EClientOrderProgress{
	EOP_standby,					//< 待命，准备接收账号密码
	EOP_order_received,				//< 客户端收到了账号，密码等信息
	EOP_game_lunched,				//< 客户端游戏已经开启
	EOP_in_game_role,				//< 客户端进入了游戏角色
	EOP_at_trade_stage,				//< 角色进入了指定场景
	EOP_ready_to_trade,				//< 客户端已经准备好交易 -- 只差双方按下交易按钮，等待服务器允许交易的命令。
	EOP_trading,					//< 正在交易
	EOP_trade_end,					//< 交易完成
};

enum EClientConfirmProgress{
	ECCP_standby,					//< 待命，准备接收账号密码
	ECCP_order_received,			//< 客户端收到了账号，密码等信息
	ECCP_game_lunched,				//< 客户端游戏已经开启
	ECCP_in_game_role,				//< 客户端进入了游戏角色
	ECCP_end,						//< 确认完成
};

//交易的一方以及发生错误，请立即中断该交易
#define EMSG_BUDDY_FAILED											0x4000401
//交易的一方已经掉线，立即中断该交易
#define EMSG_BUDDY_DISCONNECTED										0x4000501
#define msg_error_unknow_command 0xcccccccc

#define EMSG_HEART 0xcb29ad
