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
	DWORD		dwUser_ID;					// 1.������
	wchar_t		szUserPswd[32];				// 2.�������
	wchar_t		szCorrespondingGameID[32];	// 3.��Ӧ��Ϸ�˺�
	wchar_t		szUserSystemInfo[_Harddisk_ID_MAXLEN];		// 4.�û�ϵͳ��Ӳ������Ϣ
	wchar_t szRC5EnDeCryptKey[32];					//key
	DWORD		dwCorrespondingGameType;	// 5.��Ӧ��Ϸ
	BOOL		bUseAble;					// 6.�Ƿ���ã��Ƿ񱻽��ã�
	DWORD		dwDllHash;					// 7.DLL Hash
	DWORD		dwAccSuccessCount;			// 8.�ɹ���֤����
	DWORD		dwUseTotalCount;			// 9.����֤����
	DWORD		dwAccType;					// 10.����˺�����	
	DWORD		vol;						// 11��� -- not use
	COleDateTime OleDtUserDeadLine;			// 12.ʹ������
	COleDateTime OleDtRegTime;				// 13.�û�ע��ʱ��
	COleDateTime OleDtReActiveTime;			// 14.�û����¿���ʱ�䣨����xx�����õ����ڣ�
	wchar_t		szProxyName[64];			// 15.��������
	wchar_t		szUserClientName[64];		// 16.�ն˿ͻ�����
	COleDateTime OleDtAccCreateTime;		// 17.���˺ű�������ʱ��
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