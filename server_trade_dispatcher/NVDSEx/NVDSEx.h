
// NVDSEx.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNVDSExApp:
// �йش����ʵ�֣������ NVDSEx.cpp
//

class CNVDSExApp : public CWinAppEx
{
public:
	CNVDSExApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNVDSExApp theApp;