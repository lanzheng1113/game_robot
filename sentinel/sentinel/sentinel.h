
// sentinel.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CsentinelApp:
// �йش����ʵ�֣������ sentinel.cpp
//

class CsentinelApp : public CWinAppEx
{
public:
	CsentinelApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CsentinelApp theApp;