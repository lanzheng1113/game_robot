// iphlpapi.h : iphlpapi DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CiphlpapiApp
// �йش���ʵ�ֵ���Ϣ������� iphlpapi.cpp
//

class CiphlpapiApp : public CWinApp
{
public:
	CiphlpapiApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
