// MyAutoUpdate.h : MyAutoUpdate DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyAutoUpdateApp
// �йش���ʵ�ֵ���Ϣ������� MyAutoUpdate.cpp
//

class CMyAutoUpdateApp : public CWinApp
{
public:
	CMyAutoUpdateApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
