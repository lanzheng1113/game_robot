#ifndef _ACTIVE_CONNECTION_INFO_H
#define _ACTIVE_CONNECTION_INFO_H
/**
* @brief ��û���Ϣ����ʾ���������б��ϡ�
*/
struct ActiveUserInfo 
{
	DWORD dwUserID;
	wchar_t szIp[32];
	wchar_t ProxyName[32];
	wchar_t ConnectTime[32];
};

#endif