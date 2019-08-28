#pragma once
#include "util/Singleton.h"
#include <vector>
#include "../common/UserInforDef.h"

class CMyNineXUsr;

class CUserTransfer : public Singleton<CUserTransfer>
{
public:
	CUserTransfer(void);
	~CUserTransfer(void);
	//��CMyNineXUsrת��ΪUserInfo���ڴ��͸��ͻ��ˡ�
	BOOL FromNineXUserVec(const std::vector<CMyNineXUsr*>& vec);

	UserInfo* GetUserInfo() const { return _userInfoBuffer; }
	DWORD GetCountOfUserInfo() const { return _countOfUserInfo; }
private:
	UserInfo* _userInfoBuffer; //װ�û���Ϣ�Ļ�������
	DWORD _countOfUserInfo;//��������UserInfo�ĸ�����
};
