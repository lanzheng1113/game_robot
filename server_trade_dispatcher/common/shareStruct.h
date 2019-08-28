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
	char role_pswd[40];//��ɫ����
	char ware_pswd[40];//�ֿ�����
	int login_model;//��¼ģʽ ֵΪ1��ʾQQ��¼��ֵΪ2��ʾ���׵�¼
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
	unsigned long HeadFlag;		//ͷ��־
	unsigned long GameType;		//��Ϸ����
	unsigned long MesgFullType;	//����ΪMESG_TYPE_2������ΪMESG_SUBTYPE_2
	unsigned long PacketLen;	//����
	unsigned long CheckSum;		//У���
};

struct ACK_NET_HEAD{
	unsigned long AckMesgFullType;
	unsigned long GameType;
	unsigned long DataSizeRemain; //���λỰʣ����Ҫ���͵���������
	unsigned short  LastPacket;
};

struct ACQUARE_ORDER_FAILED_ACK_MSG{
	unsigned long error_code;
};

struct ACQUIRE_ORDER_MSG{
	int burden;
};

struct HEART_MSG{ //����������һ��������Ϣ
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
	int order_dispatch_id;		//< ��ʾ����һ������
	int state_code;				//< ��ǰ������Ϣ����
	int result_id;				//< �ͻ���ִ�гɹ�ΪMAC_PROCESSING_SUCCESS�����ʾ����Ĵ���
};

struct CONFIRM_STATE_CHANGED_MSG 
{
	int confirm_dispatch_id;
	int state_code;
	int result_id;
};

struct ACK_TRADE_STATE_CHANGED 
{
	int order_dispatch_id;		//< ��ʾ����һ������
	int error_code;				//< �յ��ÿͻ���ִ�еĽ���󣬷������Ĵ�������
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

//< -2��0��-1�Ѿ���ռ����Ϊ������;��������ʹ������Ϊ����ID��
//< �ͻ���״̬�ı䣬TRADE_STATE_CHANGED��result_id
#define MAC_PROCESSING_SUCCESS									0x201	//<���׵�ǰһ����ɹ���ɡ�
//< �����״����ԭ��ID��0x10X�ǹ̶��ģ�0x800֮������ж��塣
enum{
	ECC_GAME_COLLAPSE = 0x100,			//�ͻ��˱�������������ԭ�����á�ע�⽻�׹��̲��ܱ�����Ҫ�Ǳ����˾��鷳�ˡ�
	ECC_LOGIN_SERVER_ERROR = 0x101,		//������ά������æ��etc.
	ECC_ACC_OR_PSWD_INCORRECT = 0x102,	//�˺��������
	ECC_ROLE_NOT_EXIST = 0x103,			//Ѿ�Ľ�ɫ����������
	ECC_CANNOT_ENTER_TRADE_STAGE_DANGE = 0x104,	//��Σ�������¸ý�ɫ�޷����뽻������������PK�����������ڹ���ȼ��ܸߵ�����
	ECC_CANNOT_ENTER_TRADE_STAGE_OTHER = 0x105,	//����ԭ���½�ɫ�޷����뽻����������ȼ���������Ǻ������ȵȵȵȡ�
	ECC_BUYER_MONEY_NOT_ENOUGH = 0x106,	//��ûǮ���׸�ƨ
	ECC_SELLER_HAS_NO_WAIR=0x107,			//����û��ָ���Ļ���
	TRADE_FAILED_REASON_SERVER_COMMAND_CANCEL = 0x108,//������Ҫ������ȡ������
	TRADE_FAILED_REASON_SERVER_COMMAND_DENY = 0x109,//�������ܾ��˽�������
	TRADE_FAILED_REASON_BUDDY_IS_FAILED = 0x10A,//����������ȡ�����ף���Ϊ���׶������˴���
	TRADE_FAILED_REASON_BUDDY_DISCONNECTED = 0x10B,//����������ȡ�����ף���Ϊ���׶������
	TRADE_FAILED_REASON_I_AM_DISCONNECTED_NOT_TRADE = 0x10C,//���׿ͻ����������,����ȷ����û�н��ס�
	RETRY_TIME_END = 0x10D,	//�ﵽ���Դ�����δ���׳ɹ�
	TRADE_FAILED_REASON_I_AM_DISCONNECTED_TRADE_UNKNOW = 0x10E,//���׿ͻ���������ߣ�����ȷ���Ƿ��Ѿ�ִ���꽻�ס�
	ECC_UNKNOW_1 = 0x800,
	ECC_UNKNOW_2 = 0x801,
	ECC_UNKNOW_3 = 0x802,
	ECC_UNKNOW_4 = 0x803,
	ECC_UNKNOW_5 = 0x804,
	ECC_UNKNOW_6 = 0x805,
};