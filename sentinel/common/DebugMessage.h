#pragma once

#ifdef UNICODE
#define DebugMessage DebugMessageW
#else
#define DebugMessage DebugMessageA
#endif

void DebugMessageA(char * szDebugMsg,...);
void DebugMessageW(wchar_t * szDebugMsg,...);

//���а棬ȥ��������Ϣ
//#define __RELEASE_VERSION

//=====================================================================================================================================
//	Name:	DebugMessage
//	Desc:		���������Ϣ
//
//	Params:
//				szDebugMsg	Ҫ�������Ϣ
//				...					format����
//
//	Usage:	DebugMessageA("������:%d",2);
//
//	Author:	UNI
//	Date:		2009-11-7
//=====================================================================================================================================
inline void DebugMessageA(char * szDebugMsg,...)
{
#ifndef __RELEASE_VERSION
	va_list va_args;
	int len;
	char * buffer;

	va_start(va_args,szDebugMsg);
	//�ú������ظ�ʽ������ַ������ȣ�������Terminating Null
	len=_vscprintf(szDebugMsg,va_args)+1;		

	buffer=new char[len*sizeof(char)];
	if(!buffer)
	{
		OutputDebugStringA("���������Ϣ���󣬷���ռ�ʧ��\n");
		return;
	}
	vsprintf_s(buffer,len,szDebugMsg,va_args);
	OutputDebugStringA(buffer);
	delete[] buffer;
#else
	return;
#endif
}

inline void DebugMessageW(wchar_t * szDebugMsg,...)
{
#ifndef __RELEASE_VERSION
	va_list va_args;
	int len;
	wchar_t * buffer;

	va_start(va_args,szDebugMsg);
	len=_vscwprintf(szDebugMsg,va_args)+1;

	buffer=new wchar_t[len*sizeof(wchar_t)];
	if(!buffer)
	{
		OutputDebugStringW(L"���������Ϣ���󣬷���ռ�ʧ��\n");
		return;
	}
	vswprintf_s(buffer,len,szDebugMsg,va_args);
	OutputDebugStringW(buffer);

	delete[] buffer;
#else
	return;
#endif
}