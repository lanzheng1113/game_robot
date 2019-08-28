#include "stdafx.h"
#include "DBdef.h"
#include "NVDSNetHeader.h"
TableUserInfo::TableUserInfo( void )
:OleDtUserDeadLine(1900,1,1,0,0,0)
,OleDtReActiveTime(1900,1,1,0,0,0)
,OleDtRegTime(2019,1,1,0,0,0)
{
	dwUser_ID = 0;
	dwCorrespondingGameType = 0;
	bUseAble = FALSE;				// 6.是否可用（是否被禁用）
	dwDllHash = 0;					// 7.DLL Hash
	dwAccSuccessCount = 0;			// 8.成功验证次数
	dwUseTotalCount = 0;			// 9.总验证次数
	dwAccType = 0;					// 10.外挂账号类型	
	vol = 0;						// 11卷标 -- not use
	memset(szUserPswd,0,sizeof(szUserPswd));
	memset(szUserSystemInfo,0,sizeof(szUserSystemInfo));
	memset(szCorrespondingGameID,0,sizeof(szCorrespondingGameID));
	memset(szProxyName,0,sizeof(szProxyName));
}

TableUserInfo::~TableUserInfo( void )
{

}

ProxyInfo::ProxyInfo()
{
	// 	wchar_t szProxyAcc[32];
	// 	wchar_t szProxyPswd[32];
	// 	DWORD	Privilege;
	// 	int		Balance;
	// 	wchar_t szProxyName[64];
	// 	wchar_t szProxyDetail[255];
	id = 0;
	memset(szProxyAcc,0,sizeof(szProxyAcc));
	memset(szProxyPswd,0,sizeof(szProxyPswd));
	Privilege = 0;
	Balance = 0;
	memset(szProxyName,0,sizeof(szProxyName));
	memset(szProxyDetail,0,sizeof(szProxyDetail));
}

ProxyInfo::~ProxyInfo()
{

}

DWORD getYanZhengFangShi( DWORD WgVersion )
{
	DWORD dwRet = 0;
	switch (WgVersion)
	{
	case _WG_VERSION_QILIN:
	case _WG_VERSION_SHENGLIN:
	case _WG_VERSION_YULU:
	case _WG_VERSION_ARAGON:
	case _WG_VERSION_ASSIST:
	case _WG_VERSION_LOGINER_Y:
	case _WG_VERSION_SUMMER:
		dwRet = YanZhengFangShi_anJi;
		break;
	default:
		dwRet = YanZhengFangShi_anJi;
		break;
	}
	return dwRet;
}
