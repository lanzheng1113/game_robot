#pragma once

#define _Harddisk_ID_MAXLEN		200

#include <ATLCOMTIME.H>

class TableUserInfo
{
public:
	TableUserInfo(void);
	~TableUserInfo(void);
public:
	DWORD		dwUser_ID;					// 1.外挂序号
	wchar_t		szUserPswd[32];				// 2.外挂密码
	wchar_t		szCorrespondingGameID[32];	// 3.对应游戏账号
	wchar_t		szUserSystemInfo[200];		// 4.用户系统（硬件）信息
	DWORD		dwCorrespondingGameType;	// 5.对应游戏
	BOOL		bUseAble;					// 6.是否可用（是否被禁用）
	DWORD		dwDllHash;					// 7.DLL Hash
	DWORD		dwAccSuccessCount;			// 8.成功验证次数
	DWORD		dwUseTotalCount;			// 9.总验证次数
	DWORD		dwAccType;					// 10.外挂账号类型	
	DWORD		vol;						// 11卷标 -- not use
	COleDateTime OleDtUserDeadLine;			// 12.使用期限
	COleDateTime OleDtRegTime;				// 13.用户注册时间
	COleDateTime OleDtReActiveTime;			// 14.用户重新可用时间（被封xx天后可用的日期）
	wchar_t		szProxyName[64];			// 15.代理商名
};
//DWORD	dwUserID;
//wchar_t	szPswd[_UserPSWMaxLen];
//DWORD	GameType;
//DWORD	KardType;
//BOOL	BisUsable;
//DWORD	vol;
//DWORD	dll_hash;
//char	HardDiskID[_Harddisk_ID_MAXLEN];
//wchar_t	szGameID[_GameIDMaxLEN]; 
//wchar_t	EnDeCryptKey[_RC5EnDeCryptKeyMaxLen+1];
//FILETIME	regtime;
//FILETIME	EndTime;
//FILETIME	ReActiveTime;
//DWORD	dwAccSuccessCount;
//DWORD	dwAccTotalCount; 
//DWORD	IP;