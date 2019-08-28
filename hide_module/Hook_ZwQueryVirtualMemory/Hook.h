#pragma once

#include <windows.h> 
#include <winternl.h> 

#define STATUS_SUCCESS					((NTSTATUS)0x00000000L)
#define STATUS_INVALID_INFO_CLASS       ((NTSTATUS)0xC0000003L)

BOOL IsWin7();

typedef enum _MEMORY_INFORMATION_CLASS   
{  
	MemoryBasicInformation,  
	MemoryWorkingSetList,  
	MemorySectionName  
} MEMORY_INFORMATION_CLASS; 

typedef
	NTSTATUS
	(WINAPI *ZWQUERYVIRTUALMEMORY)
	(  
	IN HANDLE ProcessHandle,   
	IN PVOID BaseAddress,   
	IN MEMORY_INFORMATION_CLASS MemoryInformationClass,   
	OUT PMEMORY_BASIC_INFORMATION MemoryInformation,   
	IN ULONG MemoryInformationLength,   
	OUT PULONG ReturnLength OPTIONAL   
	);

typedef
	NTSTATUS 
	(WINAPI *ZWSETINFOMATIONTHREAD)
	(
	IN HANDLE ThreadHandle,
	IN THREADINFOCLASS ThreadInformationClass,
	IN PVOID ThreadInformation,
	IN ULONG ThreadInformationLength
	);

void HookSystem();

NTSTATUS WINAPI MyZwQueryVirtualMemory(
	HANDLE ProcessHandle,
	PVOID BaseAddress,
	MEMORY_INFORMATION_CLASS MemoryInformationClass,
	PMEMORY_BASIC_INFORMATION MemoryInformation,
	ULONG MemoryInformationLength,
	PULONG ReturnLength);

NTSTATUS WINAPI MyZwSetInformationThread(
	HANDLE ThreadHandle,
	THREADINFOCLASS ThreadInformationClass,
	PVOID ThreadInformation,
	ULONG ThreadInformationLength
	);

void UnHookSystem();