
// recordreader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CrecordreaderApp:
// �йش����ʵ�֣������ recordreader.cpp
//

class CrecordreaderApp : public CWinAppEx
{
public:
	CrecordreaderApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CrecordreaderApp theApp;