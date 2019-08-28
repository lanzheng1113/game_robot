#ifndef DVDT_NET_HEADER_H
#define DVDT_NET_HEADER_H

/*通用数据头*/
//|HEADFLAG_4|GAME_TYPE_4|GAME_VERSION_4|WG_VERSION_4|MESG_TYPE_2|MESG_SUBTYPE_2| USER_ID_4|PACKETLEN_4|CHECK_SUM_4|
//如对于First Knock:MESG_TYPE_2 = NETMESG_TYPE_MAIN_LG_DVDT、MESG_SUBTYPE_2 = NETMESG_LGDVDT_1ST.
struct PACKETGENERALHEAD 
{
	DWORD HeadFlag;		//头标志
	DWORD GameType;		//游戏类型
	DWORD GameVersion;	//游戏版本
	DWORD WgVersion;	//外挂版本
	DWORD MesgFullType;	//高字为MESG_TYPE_2、低字为MESG_SUBTYPE_2
	DWORD HeadUserID;	//用户ID
	DWORD PacketLen;	//包长
	DWORD CheckSum;		//校验和
};

struct PacketChkBaiTan
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
	DWORD Offset[20];
};

struct PacketRemoteUpdateOffset
{
	PACKETGENERALHEAD pgh;
	DWORD Offset[40];
};

struct ACKPACKETHEAD{
	DWORD AckMesgFullType;
	DWORD GameType;
	DWORD GameVersion;
	DWORD WgVersion;
	DWORD DataSizeRemain; //本次会话剩下需要发送的总数据量
	WORD  LastPacket;
};
/*登陆验证的FIRST_KNOCK的结构*/
typedef struct tag_FIRSTKNOCK
{
	PACKETGENERALHEAD GeneralHead;
	union
	{
		unsigned char Seed;			//Month + Year
		unsigned long ulCmdID;		//Steps或者自定义使用
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
	MsgAdminUnbindUsrLogin = 0x08544400,
	MsgAdminCheckPswd = 0x1884f556,
	MsgNeedUserInfo = 0x1884f557,
	MsgMoreUserInfo = 0x1884f558,
	MsgKaiKa,
	MsgUnbind,
	MsgRemoteUpdateOffset = 0x19000000,
};

enum{
	AckMsgAdminCheckOk = 0x01,
	AckMsgAdminOpenFileFailed = 0x10880000,
	AckMsgAdminPostUserData = 0x10211230,
	AckMsgAdminNoMoreUserData = 0x10211231,
	AckMsgAdminAllUserFileSame,
	AckMsgAdminKaikaFailed,
	AckMsgAdminKaikaSuccessed,
	AckMsgUnbindSuccess,
	AckMsgUnbindFailed,
	AckMsgRemoteUpdateOk,
};
#define _GAME_TYPE_DNF				0x1FFFF	//131071
#define _GAME_VERSION				0x10000101
#endif