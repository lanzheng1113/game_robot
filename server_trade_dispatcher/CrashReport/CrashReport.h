
// CrashReport.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCrashReportApp:
// �йش����ʵ�֣������ CrashReport.cpp
//

class CCrashReportApp : public CWinAppEx
{
public:
	CCrashReportApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCrashReportApp theApp;