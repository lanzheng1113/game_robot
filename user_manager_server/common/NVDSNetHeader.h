#ifndef DVDT_NET_HEADER_H
#define DVDT_NET_HEADER_H

/*ͨ������ͷ*/
//|HEADFLAG_4|GAME_TYPE_4|GAME_VERSION_4|WG_VERSION_4|MESG_TYPE_2|MESG_SUBTYPE_2| USER_ID_4|PACKETLEN_4|CHECK_SUM_4|
//�����First Knock:MESG_TYPE_2 = NETMESG_TYPE_MAIN_LG_DVDT��MESG_SUBTYPE_2 = NETMESG_LGDVDT_1ST.
struct PACKETGENERALHEAD 
{
	DWORD HeadFlag;		//ͷ��־
	DWORD GameType;		//��Ϸ����
	DWORD GameVersion;	//��Ϸ�汾
	DWORD WgVersion;	//��Ұ汾
	DWORD MesgFullType;	//����ΪMESG_TYPE_2������ΪMESG_SUBTYPE_2
	DWORD HeadUserID;	//�û�ID
	DWORD PacketLen;	//����
	DWORD CheckSum;		//У���
};

struct PacketChkBaiTan
{
	PACKETGENERALHEAD pgh;
	BYTE id[9];
};

struct PacketGetSeed
{
	PACKETGENERALHEAD pgh;
	BYTE id[9];
};

struct PacketS1 
{
	PACKETGENERALHEAD pgh;
	BYTE id[9];
	BYTE sqm_netcard_diskid[128];
};

struct PacketAdminPswdCheck
{
	PACKETGENERALHEAD pgh;
	BYTE pswd[8];
};

struct PacketProxyPswdCheck 
{
	PACKETGENERALHEAD pgh;
	BYTE acc[16];
	BYTE pswd[16];
};

struct PacketKaiKa
{
	PACKETGENERALHEAD pgh;
	char name[32];
	DWORD days;
	DWORD count;
};

struct PacketUnbind
{
	PACKETGENERALHEAD pgh;
	char AuthId[10];
	WORD isSub3Days;
};

struct PacketS1RetError
{
	WORD len;
	WORD ErrorCode;
};

struct PacketS1RetOffset
{
	WORD len;
	WORD ErrorCode/*=0*/;
	DWORD Offset[40];
};

struct PacketRemoteUpdateOffset
{
	PACKETGENERALHEAD pgh;
	DWORD Offset[40];
};

struct PacketGetNotice 
{
	PACKETGENERALHEAD pgh;
	DWORD noticeCrc;
};

struct ACKPACKETHEAD{
	DWORD AckMesgFullType;
	DWORD GameType;
	DWORD GameVersion;
	DWORD WgVersion;
	DWORD DataSizeRemain; //���λỰʣ����Ҫ���͵���������
	WORD  LastPacket;
};

struct ACK_GetSeed
{
	ACKPACKETHEAD ack;
	DWORD seedHigh;
	DWORD seedLow;
};

struct ACKPACKET_SENDNOTICE
{
	ACKPACKETHEAD ack;
	WORD NoticeCharLen;
};
/*��½��֤��FIRST_KNOCK�Ľṹ*/
typedef struct tag_FIRSTKNOCK
{
	PACKETGENERALHEAD GeneralHead;
	union
	{
		unsigned char Seed;			//Month + Year
		unsigned long ulCmdID;		//Steps�����Զ���ʹ��
	};
	unsigned long USER_ID;			//User_ID
	unsigned char Reserve[32];		//Reserve
	unsigned long ulHash;			//Hash
}FIRSTKNOCK,*PFIRSTKNOCK;

enum{
 _WG_VERSION_QILIN		=	2010003UL,
 _WG_VERSION_SHENGLIN	=	2020003UL,
 _WG_VERSION_YULU		=	1010003UL,
 _WG_VERSION_ARAGON		=	1010004UL,
 _WG_VERSION_ASSIST		=	1111UL,
 _WG_VERSION_LOGINER_Y	=	2020004UL,
 _WG_VERSION_SUMMER		=	2020005UL,
};
enum{
	MsgCheckBaiTanUser = 0x10017777,
};

enum{
	MsgGetSeed = 0x00860611,
	MsgAdminUnbindUsrLogin = 0x08544400,
	MsgAdminCheckPswd = 0x1884f556,
	MsgNeedUserInfo = 0x1884f557,
	MsgMoreUserInfo = 0x1884f558,
	MsgKaiKa,
	MsgUnbind,
	MsgRemoteUpdateOffset = 0x19000000,
	MsgGetNotice,
	MsgProxyUnbind=0x20140000,
};

enum{
	AckMsgAdminCheckOk = 0x01,
	AckMsgGetSeed = 0x00002263,
	AckMsgAdminOpenFileFailed = 0x10880000,
	AckMsgAdminPostUserData = 0x10211230,
	AckMsgAdminNoMoreUserData = 0x10211231,
	AckMsgAdminAllUserFileSame,
	AckMsgAdminKaikaFailed,
	AckMsgAdminKaikaSuccessed,
	AckMsgUnbindSuccess,
	AckMsgUnbindFailed,
	AckMsgRemoteUpdateOk,
	AckMsgNoticeNoNewer,
	AckMsgHasNewNotice,
};
#define _GAME_TYPE_DNF				0x1FFFF	//131071
#define _GAME_VERSION				0x10000101
#endif