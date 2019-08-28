#ifndef _ACTIVE_CONNECTION_INFO_H
#define _ACTIVE_CONNECTION_INFO_H
/**
* @brief 活动用户信息，显示在主界面列表上。
*/
struct ActiveUserInfo 
{
	DWORD dwUserID;
	wchar_t szIp[32];
	wchar_t ProxyName[32];
	wchar_t ConnectTime[32];
};

#endif