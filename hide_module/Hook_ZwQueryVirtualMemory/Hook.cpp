#include "stdafx.h"
#include "Hook.h"
#include "debugmessage.h"

extern DWORD g_hInstance;
BYTE Code_ZwQueryVirtualMemory[100];
BYTE Code_ZwSetInformationThread[100];
BYTE OldCode_ZwQueryVirtualMemory[5] = {0};
BYTE OldCode_ZwSetInformationThread[5] = {0};
LPVOID FunAddr_ZwQueryVirtualMemory = NULL;
LPVOID FunAddr_ZwSetInformationThread = NULL;

BOOL IsWin7()
{
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if (osvi.dwMajorVersion == 6)
	{
		return TRUE;
	}
	return FALSE;
}

void HookSystem()
{
	BYTE JumpCode[5] = {0xE9,0,0,0,0};
	FunAddr_ZwQueryVirtualMemory = (LPVOID)GetProcAddress(GetModuleHandleA(("ntdll.dll")),"ZwQueryVirtualMemory");
	DebugMessage("ZwQueryVirtualMemory:%08X", (DWORD)FunAddr_ZwQueryVirtualMemory);
	ReadProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwQueryVirtualMemory,(LPVOID)Code_ZwQueryVirtualMemory,100,NULL);
	VirtualProtect((LPVOID)Code_ZwQueryVirtualMemory,100,PAGE_EXECUTE_READWRITE,NULL);
	*(DWORD*)&JumpCode[1] = (DWORD)MyZwQueryVirtualMemory - (DWORD)FunAddr_ZwQueryVirtualMemory - 5;
	ReadProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwQueryVirtualMemory,(LPVOID)OldCode_ZwQueryVirtualMemory,5,NULL);
	WriteProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwQueryVirtualMemory,(LPVOID)JumpCode,5,NULL);

#ifdef DEVELOP_MODE
	FunAddr_ZwSetInformationThread = (LPVOID)GetProcAddress(GetModuleHandleA(("ntdll.dll")),"ZwSetInformationThread");
	SetLog(L"ZwSetInformationThread:%08X", (DWORD)FunAddr_ZwSetInformationThread);
	ReadProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwSetInformationThread,(LPVOID)Code_ZwSetInformationThread,100,NULL);
	VirtualProtect((LPVOID)Code_ZwSetInformationThread,100,PAGE_EXECUTE_READWRITE,NULL);
	*(DWORD*)&JumpCode[1] = (DWORD)MyZwSetInformationThread - (DWORD)FunAddr_ZwSetInformationThread - 5;
	ReadProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwSetInformationThread,(LPVOID)OldCode_ZwSetInformationThread,5,NULL);
	WriteProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwSetInformationThread,(LPVOID)JumpCode,5,NULL);
#endif
}

void UnHookSystem()
{
	WriteProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwQueryVirtualMemory,(LPVOID)OldCode_ZwQueryVirtualMemory,5,NULL);
}

NTSTATUS WINAPI MyZwQueryVirtualMemory(
									   HANDLE ProcessHandle,
									   PVOID BaseAddress,
									   MEMORY_INFORMATION_CLASS MemoryInformationClass,
									   PMEMORY_BASIC_INFORMATION MemoryInformation,
									   ULONG MemoryInformationLength,
									   PULONG ReturnLength)
{
	NTSTATUS nRet = ((ZWQUERYVIRTUALMEMORY)&Code_ZwQueryVirtualMemory)(ProcessHandle,BaseAddress,MemoryInformationClass,MemoryInformation,MemoryInformationLength,ReturnLength);
	if (!MemoryInformationClass && MemoryInformation && MemoryInformationLength > 0 && (DWORD)MemoryInformation->AllocationBase == g_hInstance)
	{
		return STATUS_INVALID_INFO_CLASS;
	}
	return nRet;
}

NTSTATUS WINAPI MyZwSetInformationThread(
									   HANDLE ThreadHandle,
									   THREADINFOCLASS ThreadInformationClass,
									   PVOID ThreadInformation,
									   ULONG ThreadInformationLength
									   )
{
	if (ThreadInformationClass == (THREADINFOCLASS)17)
	{
		WriteProcessMemory(INVALID_HANDLE_VALUE,FunAddr_ZwSetInformationThread,(LPVOID)OldCode_ZwSetInformationThread,5,NULL);
		DebugMessage("ThreadInformationClass:%d", 17);
		return STATUS_SUCCESS;
	}
	else
	{
		return ((ZWSETINFOMATIONTHREAD)&Code_ZwSetInformationThread)(ThreadHandle,ThreadInformationClass,ThreadInformation,ThreadInformationLength);		
	}
}