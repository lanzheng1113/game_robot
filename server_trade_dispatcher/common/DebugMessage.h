#pragma once

#ifdef UNICODE
#define DebugMessage DebugMessageW
#else
#define DebugMessage DebugMessageA
#endif

void DebugMessageA(char * szDebugMsg,...);
void DebugMessageW(wchar_t * szDebugMsg,...);

//发行版，去掉调试信息
//#define __RELEASE_VERSION

//=====================================================================================================================================
//	Name:	DebugMessage
//	Desc:		输出调试信息
//
//	Params:
//				szDebugMsg	要输出的信息
//				...					format参数
//
//	Usage:	DebugMessageA("错误编号:%d",2);
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
	//该函数返回格式化后的字符串长度，不计算Terminating Null
	len=_vscprintf(szDebugMsg,va_args)+1;		

	buffer=new char[len*sizeof(char)];
	if(!buffer)
	{
		OutputDebugStringA("输出调试信息错误，分配空间失败\n");
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
		OutputDebugStringW(L"输出调试信息错误，分配空间失败\n");
		return;
	}
	vswprintf_s(buffer,len,szDebugMsg,va_args);
	OutputDebugStringW(buffer);

	delete[] buffer;
#else
	return;
#endif
}