#pragma once

/*PACKET GENERAL HEAD�����headFlag�ı�־����ʾ����һ��ҵ��*/
#define message_tag_trade	0x12102011
#define message_tag_certain	0x12102012

/*���PACKET GENERAL HEAD��Message Full Type����ʾ�Ǹ�ҵ��ľ���ʲô���͵���Ϣ*/
#define MSG_TYPE_LOGIN				0x12345678 //< ��¼
#define MSG_TYPE_LOGOUT				0x87654321 //< �ǳ�

//������˶����������Ϣ
#define		EMSG_DISPATCH_CONFIRM_SUCCESS							0x21100000	//< �����������ͻ��ˣ���������˶���
#define		EMSG_CONFIRM_STATE_CHANGED								0x21100001	//< �ͻ��˷���������������״̬�ı�
#define		EMSG_ACK_CONFIRM_STATE_CHANGED							0x21100002	//< �����������ͻ��ˣ�����״̬�ı����Ϣ
#define		EMSG_CANCAL_ALL_CONFIRM_ORDERS							0x21100003  //< �����������ͻ��ˣ�ȡ��������˶���
#define		EMSG_CANCEL_A_CONFIRM_ORDER								0x21100004	//< �����������ͻ��ˣ�ȡ��һ���ͻ��ˡ�
#define		EMSG_ACK_CONFIRM_HAS_GET_ORDER_X						0x21100005	//< �����������ͻ��ˣ�state change espc for has_get_order!

//���ڶ��������MESSAGE FULL TYPE��Ϣ
#define MSG_TYPE_ACQUARE_ORDER									0x68551781	//< �ͻ�������һ������
#define EMSG_DISPATCH_SUCCESS									0x1000000	//�ɹ�����һ������
#define EMSG_DISPATCH_FAILED									0x1000011	//���䶩��ʧ�ܣ������Ǵ�����
	#define EMSG_DISPATCH_ERROR_GAME_NOT_FOUND						0x1000001	//�������󣬸���Ϸδ�ڷ����б���
	#define EMSG_DISPATCH_ERROR_NOT_FOUND_BUDDY						0x1000002	//..
	#define EMSG_DISPATCH_ERROR_NO_MORE_ORDER_CAN_ASSIGN			0x1000003	//..
	#define EMSG_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_SELLER_ERROR 0x1000004	//..
	#define EMSG_DISPATCH_ERROR_COMMUNICATE_FAIED_SEND_BUYER_ERROR	0x1000005	//..

#define EMSG_CANCEL_ALL_ORDERS									0x1000100		//< ���˳��������ڴ���Ķ�����
#define EMSG_CLIENT_CANCEL_ALL_OK								0x1000101		//< �������ڴ���Ķ�����Щ�Ѿ���ɣ����յ������Ӧ�󣬷������ر���ͻ��������ӡ�

//�ͻ���״̬�ı� �ͻ���==>��������TRADE_STATE_CHANGED��MessageFullType
#define EMSG_MAC_STATE_CHANGE									0x1000201		//< �ͻ���״̬�ı䣬һ�¸�����ϢΪTRADE_STATE_CHANGED.state_code
#define EMSG_ORDER_STATE_CHANGED_ACK							0x1000301		//< �ͻ��˷�������״̬�ı����Ϣ�󣬷��͸��ͻ��˵Ļ�Ӧ
#define EMSG_ACK_TRADE_HAS_GET_ORDER_X							0x1000401		//< state change espc for has_get_order!
#define EMSG_
//EMSG_ORDER_STATE_CHANGED_ACK��ϢЯ���ı�ʾ��������Ӧ��һ��CODE
enum{
	E_ERRORCODE_SUCCESS,			//�������ɹ���¼�����ĵ�ǰ״̬
	E_ERRORCODE_SERVER_REMOVED,		//�������󣬷������Ѿ����ö����ӿͻ����ĵ�ǰ�����б���ɾ����
};

enum EClientOrderProgress{
	EOP_standby,					//< ������׼�������˺�����
	EOP_order_received,				//< �ͻ����յ����˺ţ��������Ϣ
	EOP_game_lunched,				//< �ͻ�����Ϸ�Ѿ�����
	EOP_in_game_role,				//< �ͻ��˽�������Ϸ��ɫ
	EOP_at_trade_stage,				//< ��ɫ������ָ������
	EOP_ready_to_trade,				//< �ͻ����Ѿ�׼���ý��� -- ֻ��˫�����½��װ�ť���ȴ������������׵����
	EOP_trading,					//< ���ڽ���
	EOP_trade_end,					//< �������
};

enum EClientConfirmProgress{
	ECCP_standby,					//< ������׼�������˺�����
	ECCP_order_received,			//< �ͻ����յ����˺ţ��������Ϣ
	ECCP_game_lunched,				//< �ͻ�����Ϸ�Ѿ�����
	ECCP_in_game_role,				//< �ͻ��˽�������Ϸ��ɫ
	ECCP_end,						//< ȷ�����
};

//���׵�һ���Լ����������������жϸý���
#define EMSG_BUDDY_FAILED											0x4000401
//���׵�һ���Ѿ����ߣ������жϸý���
#define EMSG_BUDDY_DISCONNECTED										0x4000501
#define msg_error_unknow_command 0xcccccccc

#define EMSG_HEART 0xcb29ad
