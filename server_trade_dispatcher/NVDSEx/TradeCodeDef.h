#pragma once
//这是标志派发过程错误的代码
enum EDispatchError{
	EDE_DISPATCH_SUCCESS = 0,
	EDE_DISPATCH_ERROR_GAME_NOT_FOUND,				//< 服务器不支持客户端指定的游戏类型
	EDE_DISPATCH_ERROR_NOT_FOUND_BUDDY,				//< 没有找到合适的交易对象机，已经废弃
	EDE_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN,	//< 没有可取的订单
	EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR,//< 网络层发送订单发生了错误，此项未处理。
	EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_BUYER_ERROR,//< 网络层发送订单发生了错误，此项未处理。
};