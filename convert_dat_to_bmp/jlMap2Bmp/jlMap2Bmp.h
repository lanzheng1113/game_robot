
// jlMap2Bmp.h : jlMap2Bmp Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CjlMap2BmpApp:
// �йش����ʵ�֣������ jlMap2Bmp.cpp
//

class CjlMap2BmpApp : public CWinAppEx
{
public:
	CjlMap2BmpApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CjlMap2BmpApp theApp;
