#pragma once

#define _DATABASE_NAME _T("UserInfo_DB.mdb")
#define _DATABASE_PASSWORD _T("4260201asdf")

#define PROXY_INFO_TABLE_NAME _T("ProxyInfoTable")

#define _Harddisk_ID_MAXLEN		200

#define test_card_hours			3

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
	wchar_t		szUserSystemInfo[_Harddisk_ID_MAXLEN];		// 4.用户系统（硬件）信息
	wchar_t szRC5EnDeCryptKey[32];					//key
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
	wchar_t		szUserClientName[64];		// 16.终端客户名称
	COleDateTime OleDtAccCreateTime;		// 17.该账号被创建的时间
};

class ProxyInfo
{
public:
	ProxyInfo();
	~ProxyInfo();
public:
	int id;
	wchar_t szProxyAcc[32];
	wchar_t szProxyPswd[32];
	DWORD	Privilege;
	double		Balance;
	wchar_t szProxyName[64];
	wchar_t szProxyDetail[255];
	int		TestKardRemain;
	double	ZheKou;
	DWORD	maxUserId;
	DWORD	minUserId;
protected:
private:
};

struct ProxyInformationStruct{
	char account[32];
	char name[32];
	double balance;
	int testKardRemain;
	double ZheKou;
	int maxUserId;
	int minUserId;
};

enum{
	YanZhengFangShi_anJi,
	YanZhengFangShi_anHao,
};

DWORD getYanZhengFangShi(DWORD WgVersion);