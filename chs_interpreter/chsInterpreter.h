
// chsInterpreter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CchsInterpreterApp:
// �йش����ʵ�֣������ chsInterpreter.cpp
//

class CchsInterpreterApp : public CWinAppEx
{
public:
	CchsInterpreterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CchsInterpreterApp theApp;