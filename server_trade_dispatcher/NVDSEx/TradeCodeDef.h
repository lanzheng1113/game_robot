#pragma once
//���Ǳ�־�ɷ����̴���Ĵ���
enum EDispatchError{
	EDE_DISPATCH_SUCCESS = 0,
	EDE_DISPATCH_ERROR_GAME_NOT_FOUND,				//< ��������֧�ֿͻ���ָ������Ϸ����
	EDE_DISPATCH_ERROR_NOT_FOUND_BUDDY,				//< û���ҵ����ʵĽ��׶�������Ѿ�����
	EDE_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN,	//< û�п�ȡ�Ķ���
	EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR,//< ����㷢�Ͷ��������˴��󣬴���δ����
	EDE_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_BUYER_ERROR,//< ����㷢�Ͷ��������˴��󣬴���δ����
};