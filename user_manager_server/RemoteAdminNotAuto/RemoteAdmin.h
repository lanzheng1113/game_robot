
// RemoteAdmin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRemoteAdminApp:
// �йش����ʵ�֣������ RemoteAdmin.cpp
//

class CRemoteAdminApp : public CWinAppEx
{
public:
	CRemoteAdminApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRemoteAdminApp theApp;