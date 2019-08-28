#include "stdafx.h"
#include "CpuUsage.h"
#include <stdio.h>

PROCNTQSI NtQuerySystemInformation;


//注：NtQuerySystemInformation函数时NTDLL.dll中的用来查询系统信息的函数。 GetCpuUsage成功返回CPU使用率，失败返回-1。

double GetCpuUsage()
{ 
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo; 
	SYSTEM_TIME_INFORMATION SysTimeInfo; 
	SYSTEM_BASIC_INFORMATION SysBaseInfo; 
	double dbIdleTime = 0.0; 
	double dbSystemTime; 
	LONG status; 
	static LARGE_INTEGER liOldIdleTime = {0,0}; 
	static LARGE_INTEGER liOldSystemTime = {0,0};

	NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandleA("ntdll"),"NtQuerySystemInformation");

	if (!NtQuerySystemInformation) 
		return -1;

	// get number of processors in the system 
	status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if (status != NO_ERROR) 
		return -1;

	// get new system time 
	status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0); 
	if (status!=NO_ERROR) 
		return -1;

	// get new CPU's idle time 
	status =NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if (status != NO_ERROR) 
		return -1;

	// if it's a first call - skip it 
	if (liOldIdleTime.QuadPart != 0) 
	{ 
		// CurrentValue = NewValue - OldValue 
		dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
		dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);

		// CurrentCpuIdle = IdleTime / SystemTime 
		dbIdleTime = dbIdleTime / dbSystemTime;

		// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors 
		dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;

	}

	// store new CPU's idle and system time 
	liOldIdleTime = SysPerfInfo.liIdleTime; 
	liOldSystemTime = SysTimeInfo.liKeSystemTime;

	return dbIdleTime;
}