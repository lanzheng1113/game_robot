// iphlpapi.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "iphlpapi.h"
#include "debugmessage.h"
#include <Tlhelp32.h>
#include <locale.h>
#include "fdoffset.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CiphlpapiApp

BEGIN_MESSAGE_MAP(CiphlpapiApp, CWinApp)
END_MESSAGE_MAP()


// CiphlpapiApp 构造

CiphlpapiApp::CiphlpapiApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CiphlpapiApp 对象

CiphlpapiApp theApp;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#ifdef EXPORT
#undef EXPORT
#endif
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:AddIPAddress=_AheadLib_AddIPAddress,@1")
#pragma comment(linker, "/EXPORT:AllocateAndGetArpEntTableFromStack=_AheadLib_AllocateAndGetArpEntTableFromStack,@2")
#pragma comment(linker, "/EXPORT:AllocateAndGetIfTableFromStack=_AheadLib_AllocateAndGetIfTableFromStack,@3")
#pragma comment(linker, "/EXPORT:AllocateAndGetIpAddrTableFromStack=_AheadLib_AllocateAndGetIpAddrTableFromStack,@4")
#pragma comment(linker, "/EXPORT:AllocateAndGetIpForwardTableFromStack=_AheadLib_AllocateAndGetIpForwardTableFromStack,@5")
#pragma comment(linker, "/EXPORT:AllocateAndGetIpNetTableFromStack=_AheadLib_AllocateAndGetIpNetTableFromStack,@6")
#pragma comment(linker, "/EXPORT:AllocateAndGetTcpExTable2FromStack=_AheadLib_AllocateAndGetTcpExTable2FromStack,@7")
#pragma comment(linker, "/EXPORT:AllocateAndGetTcpExTableFromStack=_AheadLib_AllocateAndGetTcpExTableFromStack,@8")
#pragma comment(linker, "/EXPORT:AllocateAndGetTcpTableFromStack=_AheadLib_AllocateAndGetTcpTableFromStack,@9")
#pragma comment(linker, "/EXPORT:AllocateAndGetUdpExTable2FromStack=_AheadLib_AllocateAndGetUdpExTable2FromStack,@10")
#pragma comment(linker, "/EXPORT:AllocateAndGetUdpExTableFromStack=_AheadLib_AllocateAndGetUdpExTableFromStack,@11")
#pragma comment(linker, "/EXPORT:AllocateAndGetUdpTableFromStack=_AheadLib_AllocateAndGetUdpTableFromStack,@12")
#pragma comment(linker, "/EXPORT:CancelIPChangeNotify=_AheadLib_CancelIPChangeNotify,@13")
#pragma comment(linker, "/EXPORT:CreateIpForwardEntry=_AheadLib_CreateIpForwardEntry,@14")
#pragma comment(linker, "/EXPORT:CreateIpNetEntry=_AheadLib_CreateIpNetEntry,@15")
#pragma comment(linker, "/EXPORT:CreateProxyArpEntry=_AheadLib_CreateProxyArpEntry,@16")
#pragma comment(linker, "/EXPORT:DeleteIPAddress=_AheadLib_DeleteIPAddress,@17")
#pragma comment(linker, "/EXPORT:DeleteIpForwardEntry=_AheadLib_DeleteIpForwardEntry,@18")
#pragma comment(linker, "/EXPORT:DeleteIpNetEntry=_AheadLib_DeleteIpNetEntry,@19")
#pragma comment(linker, "/EXPORT:DeleteProxyArpEntry=_AheadLib_DeleteProxyArpEntry,@20")
#pragma comment(linker, "/EXPORT:DisableMediaSense=_AheadLib_DisableMediaSense,@21")
#pragma comment(linker, "/EXPORT:EnableRouter=_AheadLib_EnableRouter,@22")
#pragma comment(linker, "/EXPORT:FlushIpNetTable=_AheadLib_FlushIpNetTable,@23")
#pragma comment(linker, "/EXPORT:FlushIpNetTableFromStack=_AheadLib_FlushIpNetTableFromStack,@24")
#pragma comment(linker, "/EXPORT:GetAdapterIndex=_AheadLib_GetAdapterIndex,@25")
#pragma comment(linker, "/EXPORT:GetAdapterOrderMap=_AheadLib_GetAdapterOrderMap,@26")
#pragma comment(linker, "/EXPORT:GetAdaptersAddresses=_AheadLib_GetAdaptersAddresses,@27")
#pragma comment(linker, "/EXPORT:GetAdaptersInfo=_AheadLib_GetAdaptersInfo,@28")
#pragma comment(linker, "/EXPORT:GetBestInterface=_AheadLib_GetBestInterface,@29")
#pragma comment(linker, "/EXPORT:GetBestInterfaceEx=_AheadLib_GetBestInterfaceEx,@30")
#pragma comment(linker, "/EXPORT:GetBestInterfaceFromStack=_AheadLib_GetBestInterfaceFromStack,@31")
#pragma comment(linker, "/EXPORT:GetBestRoute=_AheadLib_GetBestRoute,@32")
#pragma comment(linker, "/EXPORT:GetBestRouteFromStack=_AheadLib_GetBestRouteFromStack,@33")
#pragma comment(linker, "/EXPORT:GetExtendedTcpTable=_AheadLib_GetExtendedTcpTable,@34")
#pragma comment(linker, "/EXPORT:GetExtendedUdpTable=_AheadLib_GetExtendedUdpTable,@35")
#pragma comment(linker, "/EXPORT:GetFriendlyIfIndex=_AheadLib_GetFriendlyIfIndex,@36")
#pragma comment(linker, "/EXPORT:GetIcmpStatistics=_AheadLib_GetIcmpStatistics,@37")
#pragma comment(linker, "/EXPORT:GetIcmpStatisticsEx=_AheadLib_GetIcmpStatisticsEx,@38")
#pragma comment(linker, "/EXPORT:GetIcmpStatsFromStack=_AheadLib_GetIcmpStatsFromStack,@39")
#pragma comment(linker, "/EXPORT:GetIcmpStatsFromStackEx=_AheadLib_GetIcmpStatsFromStackEx,@40")
#pragma comment(linker, "/EXPORT:GetIfEntry=_AheadLib_GetIfEntry,@41")
#pragma comment(linker, "/EXPORT:GetIfEntryFromStack=_AheadLib_GetIfEntryFromStack,@42")
#pragma comment(linker, "/EXPORT:GetIfTable=_AheadLib_GetIfTable,@43")
#pragma comment(linker, "/EXPORT:GetIfTableFromStack=_AheadLib_GetIfTableFromStack,@44")
#pragma comment(linker, "/EXPORT:GetIgmpList=_AheadLib_GetIgmpList,@45")
#pragma comment(linker, "/EXPORT:GetInterfaceInfo=_AheadLib_GetInterfaceInfo,@46")
#pragma comment(linker, "/EXPORT:GetIpAddrTable=_AheadLib_GetIpAddrTable,@47")
#pragma comment(linker, "/EXPORT:GetIpAddrTableFromStack=_AheadLib_GetIpAddrTableFromStack,@48")
#pragma comment(linker, "/EXPORT:GetIpErrorString=_AheadLib_GetIpErrorString,@49")
#pragma comment(linker, "/EXPORT:GetIpForwardTable=_AheadLib_GetIpForwardTable,@50")
#pragma comment(linker, "/EXPORT:GetIpForwardTableFromStack=_AheadLib_GetIpForwardTableFromStack,@51")
#pragma comment(linker, "/EXPORT:GetIpNetTable=_AheadLib_GetIpNetTable,@52")
#pragma comment(linker, "/EXPORT:GetIpNetTableFromStack=_AheadLib_GetIpNetTableFromStack,@53")
#pragma comment(linker, "/EXPORT:GetIpStatistics=_AheadLib_GetIpStatistics,@54")
#pragma comment(linker, "/EXPORT:GetIpStatisticsEx=_AheadLib_GetIpStatisticsEx,@55")
#pragma comment(linker, "/EXPORT:GetIpStatsFromStack=_AheadLib_GetIpStatsFromStack,@56")
#pragma comment(linker, "/EXPORT:GetIpStatsFromStackEx=_AheadLib_GetIpStatsFromStackEx,@57")
#pragma comment(linker, "/EXPORT:GetNetworkParams=_AheadLib_GetNetworkParams,@58")
#pragma comment(linker, "/EXPORT:GetNumberOfInterfaces=_AheadLib_GetNumberOfInterfaces,@59")
#pragma comment(linker, "/EXPORT:GetOwnerModuleFromTcp6Entry=_AheadLib_GetOwnerModuleFromTcp6Entry,@60")
#pragma comment(linker, "/EXPORT:GetOwnerModuleFromTcpEntry=_AheadLib_GetOwnerModuleFromTcpEntry,@61")
#pragma comment(linker, "/EXPORT:GetOwnerModuleFromUdp6Entry=_AheadLib_GetOwnerModuleFromUdp6Entry,@62")
#pragma comment(linker, "/EXPORT:GetOwnerModuleFromUdpEntry=_AheadLib_GetOwnerModuleFromUdpEntry,@63")
#pragma comment(linker, "/EXPORT:GetPerAdapterInfo=_AheadLib_GetPerAdapterInfo,@64")
#pragma comment(linker, "/EXPORT:GetRTTAndHopCount=_AheadLib_GetRTTAndHopCount,@65")
#pragma comment(linker, "/EXPORT:GetTcpExTable2FromStack=_AheadLib_GetTcpExTable2FromStack,@66")
#pragma comment(linker, "/EXPORT:GetTcpStatistics=_AheadLib_GetTcpStatistics,@67")
#pragma comment(linker, "/EXPORT:GetTcpStatisticsEx=_AheadLib_GetTcpStatisticsEx,@68")
#pragma comment(linker, "/EXPORT:GetTcpStatsFromStack=_AheadLib_GetTcpStatsFromStack,@69")
#pragma comment(linker, "/EXPORT:GetTcpStatsFromStackEx=_AheadLib_GetTcpStatsFromStackEx,@70")
#pragma comment(linker, "/EXPORT:GetTcpTable=_AheadLib_GetTcpTable,@71")
#pragma comment(linker, "/EXPORT:GetTcpTableFromStack=_AheadLib_GetTcpTableFromStack,@72")
#pragma comment(linker, "/EXPORT:GetUdpExTable2FromStack=_AheadLib_GetUdpExTable2FromStack,@73")
#pragma comment(linker, "/EXPORT:GetUdpStatistics=_AheadLib_GetUdpStatistics,@74")
#pragma comment(linker, "/EXPORT:GetUdpStatisticsEx=_AheadLib_GetUdpStatisticsEx,@75")
#pragma comment(linker, "/EXPORT:GetUdpStatsFromStack=_AheadLib_GetUdpStatsFromStack,@76")
#pragma comment(linker, "/EXPORT:GetUdpStatsFromStackEx=_AheadLib_GetUdpStatsFromStackEx,@77")
#pragma comment(linker, "/EXPORT:GetUdpTable=_AheadLib_GetUdpTable,@78")
#pragma comment(linker, "/EXPORT:GetUdpTableFromStack=_AheadLib_GetUdpTableFromStack,@79")
#pragma comment(linker, "/EXPORT:GetUniDirectionalAdapterInfo=_AheadLib_GetUniDirectionalAdapterInfo,@80")
#pragma comment(linker, "/EXPORT:Icmp6CreateFile=_AheadLib_Icmp6CreateFile,@81")
#pragma comment(linker, "/EXPORT:Icmp6ParseReplies=_AheadLib_Icmp6ParseReplies,@82")
#pragma comment(linker, "/EXPORT:Icmp6SendEcho2=_AheadLib_Icmp6SendEcho2,@83")
#pragma comment(linker, "/EXPORT:IcmpCloseHandle=_AheadLib_IcmpCloseHandle,@84")
#pragma comment(linker, "/EXPORT:IcmpCreateFile=_AheadLib_IcmpCreateFile,@85")
#pragma comment(linker, "/EXPORT:IcmpParseReplies=_AheadLib_IcmpParseReplies,@86")
#pragma comment(linker, "/EXPORT:IcmpSendEcho2=_AheadLib_IcmpSendEcho2,@87")
#pragma comment(linker, "/EXPORT:IcmpSendEcho=_AheadLib_IcmpSendEcho,@88")
#pragma comment(linker, "/EXPORT:InternalCreateIpForwardEntry=_AheadLib_InternalCreateIpForwardEntry,@89")
#pragma comment(linker, "/EXPORT:InternalCreateIpNetEntry=_AheadLib_InternalCreateIpNetEntry,@90")
#pragma comment(linker, "/EXPORT:InternalDeleteIpForwardEntry=_AheadLib_InternalDeleteIpForwardEntry,@91")
#pragma comment(linker, "/EXPORT:InternalDeleteIpNetEntry=_AheadLib_InternalDeleteIpNetEntry,@92")
#pragma comment(linker, "/EXPORT:InternalGetIfTable=_AheadLib_InternalGetIfTable,@93")
#pragma comment(linker, "/EXPORT:InternalGetIpAddrTable=_AheadLib_InternalGetIpAddrTable,@94")
#pragma comment(linker, "/EXPORT:InternalGetIpForwardTable=_AheadLib_InternalGetIpForwardTable,@95")
#pragma comment(linker, "/EXPORT:InternalGetIpNetTable=_AheadLib_InternalGetIpNetTable,@96")
#pragma comment(linker, "/EXPORT:InternalGetTcpTable=_AheadLib_InternalGetTcpTable,@97")
#pragma comment(linker, "/EXPORT:InternalGetUdpTable=_AheadLib_InternalGetUdpTable,@98")
#pragma comment(linker, "/EXPORT:InternalSetIfEntry=_AheadLib_InternalSetIfEntry,@99")
#pragma comment(linker, "/EXPORT:InternalSetIpForwardEntry=_AheadLib_InternalSetIpForwardEntry,@100")
#pragma comment(linker, "/EXPORT:InternalSetIpNetEntry=_AheadLib_InternalSetIpNetEntry,@101")
#pragma comment(linker, "/EXPORT:InternalSetIpStats=_AheadLib_InternalSetIpStats,@102")
#pragma comment(linker, "/EXPORT:InternalSetTcpEntry=_AheadLib_InternalSetTcpEntry,@103")
#pragma comment(linker, "/EXPORT:IpReleaseAddress=_AheadLib_IpReleaseAddress,@104")
#pragma comment(linker, "/EXPORT:IpRenewAddress=_AheadLib_IpRenewAddress,@105")
#pragma comment(linker, "/EXPORT:IsLocalAddress=_AheadLib_IsLocalAddress,@106")
#pragma comment(linker, "/EXPORT:NTPTimeToNTFileTime=_AheadLib_NTPTimeToNTFileTime,@107")
#pragma comment(linker, "/EXPORT:NTTimeToNTPTime=_AheadLib_NTTimeToNTPTime,@108")
#pragma comment(linker, "/EXPORT:NhGetGuidFromInterfaceName=_AheadLib_NhGetGuidFromInterfaceName,@109")
#pragma comment(linker, "/EXPORT:NhGetInterfaceNameFromDeviceGuid=_AheadLib_NhGetInterfaceNameFromDeviceGuid,@110")
#pragma comment(linker, "/EXPORT:NhGetInterfaceNameFromGuid=_AheadLib_NhGetInterfaceNameFromGuid,@111")
#pragma comment(linker, "/EXPORT:NhpAllocateAndGetInterfaceInfoFromStack=_AheadLib_NhpAllocateAndGetInterfaceInfoFromStack,@112")
#pragma comment(linker, "/EXPORT:NhpGetInterfaceIndexFromStack=_AheadLib_NhpGetInterfaceIndexFromStack,@113")
#pragma comment(linker, "/EXPORT:NotifyAddrChange=_AheadLib_NotifyAddrChange,@114")
#pragma comment(linker, "/EXPORT:NotifyRouteChange=_AheadLib_NotifyRouteChange,@115")
#pragma comment(linker, "/EXPORT:NotifyRouteChangeEx=_AheadLib_NotifyRouteChangeEx,@116")
#pragma comment(linker, "/EXPORT:RestoreMediaSense=_AheadLib_RestoreMediaSense,@133")
#pragma comment(linker, "/EXPORT:SendARP=_AheadLib_SendARP,@134")
#pragma comment(linker, "/EXPORT:SetAdapterIpAddress=_AheadLib_SetAdapterIpAddress,@135")
#pragma comment(linker, "/EXPORT:SetBlockRoutes=_AheadLib_SetBlockRoutes,@136")
#pragma comment(linker, "/EXPORT:SetIfEntry=_AheadLib_SetIfEntry,@137")
#pragma comment(linker, "/EXPORT:SetIfEntryToStack=_AheadLib_SetIfEntryToStack,@138")
#pragma comment(linker, "/EXPORT:SetIpForwardEntry=_AheadLib_SetIpForwardEntry,@139")
#pragma comment(linker, "/EXPORT:SetIpForwardEntryToStack=_AheadLib_SetIpForwardEntryToStack,@140")
#pragma comment(linker, "/EXPORT:SetIpMultihopRouteEntryToStack=_AheadLib_SetIpMultihopRouteEntryToStack,@141")
#pragma comment(linker, "/EXPORT:SetIpNetEntry=_AheadLib_SetIpNetEntry,@142")
#pragma comment(linker, "/EXPORT:SetIpNetEntryToStack=_AheadLib_SetIpNetEntryToStack,@143")
#pragma comment(linker, "/EXPORT:SetIpRouteEntryToStack=_AheadLib_SetIpRouteEntryToStack,@144")
#pragma comment(linker, "/EXPORT:SetIpStatistics=_AheadLib_SetIpStatistics,@145")
#pragma comment(linker, "/EXPORT:SetIpStatsToStack=_AheadLib_SetIpStatsToStack,@146")
#pragma comment(linker, "/EXPORT:SetIpTTL=_AheadLib_SetIpTTL,@147")
#pragma comment(linker, "/EXPORT:SetProxyArpEntryToStack=_AheadLib_SetProxyArpEntryToStack,@148")
#pragma comment(linker, "/EXPORT:SetRouteWithRef=_AheadLib_SetRouteWithRef,@149")
#pragma comment(linker, "/EXPORT:SetTcpEntry=_AheadLib_SetTcpEntry,@150")
#pragma comment(linker, "/EXPORT:SetTcpEntryToStack=_AheadLib_SetTcpEntryToStack,@151")
#pragma comment(linker, "/EXPORT:UnenableRouter=_AheadLib_UnenableRouter,@152")
#pragma comment(linker, "/EXPORT:do_echo_rep=_AheadLib_do_echo_rep,@153")
#pragma comment(linker, "/EXPORT:do_echo_req=_AheadLib_do_echo_req,@154")
#pragma comment(linker, "/EXPORT:register_icmp=_AheadLib_register_icmp,@155")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// 原始模块句柄
	DWORD m_dwReturn[155] = {0};	// 原始函数返回地址

	// 加载原始模块
	inline BOOL WINAPI Load()
	{
		DebugMessage("load....");
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\iphlpapi"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("无法加载 %s，程序无法正常运行。lastError=%d"), tzPath,GetLastError());
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		}
		DebugMessage("load. end...");
		return (m_hModule != NULL);	
	}

	// 释放原始模块
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}

	// 获取原始函数地址
	FARPROC WINAPI GetAddress(PCSTR pszProcName)
	{
		FARPROC fpAddress;
		CHAR szProcName[16];
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				wsprintf(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			wsprintf(tzTemp, TEXT("无法找到函数 %hs，程序无法正常运行。"), pszProcName);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
using namespace AheadLib;
DWORD WINAPI MyRemoteThread(LPVOID lpParam);
namespace Hook_Ls {
	// Hook_Ls
	inline BOOL WINAPI Hook_Ls(INT iHookId = WH_CALLWNDPROC){ //WH_KEYBOARD)
		DebugMessage("xoxoxoxoxxoxoxxo");
		HANDLE hProcessSnap;
		PROCESSENTRY32 pe32;
		BOOL isHooked = FALSE;
		//Hook(&GetVolumeInformationA,7,HookRoutine_GetVolumeInformationA,true);
		hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		pe32.dwSize = sizeof( PROCESSENTRY32 );
		if( !Process32First( hProcessSnap, &pe32 ) )
		{
			CloseHandle( hProcessSnap );    // Must clean up the
			return FALSE;
		}
		do
		{
			if( lstrcmpi(_T("soul.exe"),pe32.szExeFile)==0)//client.dat
			{
				HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
				THREADENTRY32 te32;
				hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
				if( hThreadSnap == INVALID_HANDLE_VALUE ) 
					return FALSE; 
				te32.dwSize = sizeof(THREADENTRY32 ); 
				if( !Thread32First( hThreadSnap, &te32 ) ) {
					CloseHandle( hThreadSnap );    // Must clean up the
					return FALSE;
				}
				do { 
					if( te32.th32OwnerProcessID == pe32.th32ProcessID && GetCurrentThreadId()==te32.th32ThreadID)
					{
						HANDLE hThread= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MyRemoteThread,NULL,NULL,NULL);//这个线程就会建立一个外挂对话框窗口
						if(hThread!=NULL)
						{
							CloseHandle(hThread);
						}
						break;
					}
				} while( Thread32Next(hThreadSnap, &te32 ) ); 
				CloseHandle( hThreadSnap );
			}
		} while( Process32Next( hProcessSnap, &pe32 ) );

		CloseHandle( hProcessSnap );
		return (TRUE);
	}
}

ALCDECL AheadLib_AddIPAddress(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[0 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AddIPAddress")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[0 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetArpEntTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[1 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetArpEntTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[1 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetIfTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[2 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetIfTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[2 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetIpAddrTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[3 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetIpAddrTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[3 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetIpForwardTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[4 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetIpForwardTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[4 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetIpNetTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[5 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetIpNetTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[5 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetTcpExTable2FromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[6 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetTcpExTable2FromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[6 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetTcpExTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[7 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetTcpExTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[7 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetTcpTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[8 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetTcpTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[8 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetUdpExTable2FromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[9 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetUdpExTable2FromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[9 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetUdpExTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[10 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetUdpExTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[10 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_AllocateAndGetUdpTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[11 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("AllocateAndGetUdpTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[11 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_CancelIPChangeNotify(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[12 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("CancelIPChangeNotify")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[12 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_CreateIpForwardEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[13 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("CreateIpForwardEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[13 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_CreateIpNetEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[14 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("CreateIpNetEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[14 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_CreateProxyArpEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[15 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("CreateProxyArpEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[15 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_DeleteIPAddress(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[16 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("DeleteIPAddress")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[16 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_DeleteIpForwardEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[17 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("DeleteIpForwardEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[17 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_DeleteIpNetEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[18 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("DeleteIpNetEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[18 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_DeleteProxyArpEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[19 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("DeleteProxyArpEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[19 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_DisableMediaSense(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[20 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("DisableMediaSense")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[20 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_EnableRouter(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[21 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("EnableRouter")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[21 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_FlushIpNetTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[22 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("FlushIpNetTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[22 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_FlushIpNetTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[23 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("FlushIpNetTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[23 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetAdapterIndex(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[24 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetAdapterIndex")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[24 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数

ALCDECL AheadLib_GetAdapterOrderMap(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[25 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetAdapterOrderMap")();
	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[25 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetAdaptersAddresses(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[26 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetAdaptersAddresses")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[26 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}


static void checkPatch() 
{
	//DebugMessage("s1");
	static bool isPatched = false;
	if (isPatched){
		return;
	}
	//DebugMessage("s2");
	isPatched = true;
	OSVERSIONINFO osi;
	memset(&osi,0,sizeof(osi));
	osi.dwOSVersionInfoSize = sizeof(osi);
	GetVersionEx(&osi);
	DebugMessageA("h = %d,l=%d,lasterror=%d",osi.dwMajorVersion,osi.dwMinorVersion,GetLastError());
	if (osi.dwMajorVersion == 6 && osi.dwMinorVersion == 1) //Win7
	{
		HMODULE hc = GetModuleHandle("dhcpcsvc.dll");
		if (!hc){	
			hc = LoadLibrary("dhcpcsvc.dll");
			if (!hc)
			{
				DebugMessageA("没有找到Module of dhcpcsvc.");
				return;
			}
		}
		//DebugMessage("s3");
		TCHAR tzPath[MAX_PATH];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\iphlpapi"));
		HMODULE m_hModuleiphlpapi = GetModuleHandle(tzPath);
		if (m_hModuleiphlpapi == NULL){
			DebugMessage("没有加载 %s，程序无法Patch。", tzPath);
			return;
		}
		//DebugMessage("s4");
		DWORD dwProcAddr = (DWORD)GetProcAddress(m_hModuleiphlpapi,"ConvertInterfaceNameToLuidW");
		if (dwProcAddr == 0){
			DebugMessage("没有找到ConvertInterfaceNameToLuidW函数的地址。");
			return;
		}
		//DebugMessage("s5");
		char jzs1[] = "6683387bff750c500f84????????e8";
		DWORD sizeOfModule = 0x12000-sizeof(jzs1);
		DWORD searchBeginPos = (DWORD)hc;
		DWORD searchEndPos = (DWORD)hc+sizeOfModule;
		DWORD g_GuoLvShangYe = findOffsets(jzs1,searchBeginPos,searchEndPos);
		if (g_GuoLvShangYe == 0){
			DebugMessageA("没有找到P地址。");
			return;
		}else{
			//DebugMessage("s6,g_GuoLvShangYe=%d",g_GuoLvShangYe);
			DWORD pAddr = g_GuoLvShangYe + 15;

			int pRelateOffset = *(int*)pAddr;
			DWORD destAddr = pAddr - 1 + pRelateOffset + 5;
			DebugMessageA("FindAddr = 0x%X,DestAddr = 0x%X",pAddr,destAddr);
			//72B31B22- FF25 00F0B372       jmp     dword ptr [0x72B3F000]     修改的是这个静态变量的值为ConvertInterfaceNameToLuidW他的地址
			DWORD jmpcodeAddr = destAddr+2;
			DWORD addrToPatch = *(DWORD*)jmpcodeAddr;//0x72B3F000
			DebugMessageA("addr to patch = 0x%X",addrToPatch);

			DWORD Param=0;
			VirtualProtect((LPVOID)addrToPatch, 4, PAGE_READWRITE, &Param);
			*(DWORD*)addrToPatch = dwProcAddr;
			VirtualProtect((LPVOID)addrToPatch, 4, Param, &Param);

		}
	}else{
		return;
	}//not win7

}


ALCDECL AheadLib_GetAdaptersInfo(void)
{
	// 保存返回地址到 TLS
	// 	__asm mov xx,eax
	// 	__asm mov eax,[esp+4]
	// 	__asm mov pAdapter,eax //保存参数1  __out		PIP_ADAPTER_INFO pAdapterInfo
	// 	__asm mov eax,[esp+8]  
	// 	__asm mov xxoo,eax	   //保存参数2  __in_out    PULONG pOutBufLen
	// 	__asm mov eax,xx

	__asm PUSH m_dwReturn[27 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	//在这里Patch。
	checkPatch();
	GetAddress("GetAdaptersInfo")();
	// 	__asm{
	// 		mov xx,eax
	// 		pushad
	// 		push eax			//return by GetAdaptersInfo
	// 		mov eax,xxoo
	// 		push eax			//len
	// 		mov eax,pAdapter
	// 		push eax			//pAdapter
	// 		call ChangeMe;
	// 		popad
	// 		mov eax,xx
	// 	}
	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[27 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetBestInterface(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[28 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetBestInterface")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[28 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetBestInterfaceEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[29 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetBestInterfaceEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[29 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetBestInterfaceFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[30 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetBestInterfaceFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[30 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetBestRoute(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[31 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetBestRoute")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[31 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetBestRouteFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[32 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetBestRouteFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[32 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetExtendedTcpTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[33 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetExtendedTcpTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[33 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetExtendedUdpTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[34 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetExtendedUdpTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[34 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetFriendlyIfIndex(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[35 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetFriendlyIfIndex")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[35 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIcmpStatistics(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[36 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIcmpStatistics")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[36 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIcmpStatisticsEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[37 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIcmpStatisticsEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[37 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIcmpStatsFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[38 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIcmpStatsFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[38 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIcmpStatsFromStackEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[39 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIcmpStatsFromStackEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[39 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIfEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[40 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIfEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[40 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIfEntryFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[41 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIfEntryFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[41 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIfTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[42 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIfTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[42 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIfTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[43 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIfTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[43 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIgmpList(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[44 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIgmpList")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[44 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetInterfaceInfo(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[45 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetInterfaceInfo")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[45 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpAddrTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[46 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpAddrTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[46 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpAddrTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[47 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpAddrTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[47 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpErrorString(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[48 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpErrorString")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[48 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpForwardTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[49 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpForwardTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[49 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpForwardTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[50 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpForwardTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[50 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpNetTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[51 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpNetTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[51 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpNetTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[52 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpNetTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[52 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpStatistics(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[53 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpStatistics")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[53 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpStatisticsEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[54 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpStatisticsEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[54 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpStatsFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[55 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpStatsFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[55 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetIpStatsFromStackEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[56 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetIpStatsFromStackEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[56 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetNetworkParams(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[57 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetNetworkParams")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[57 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetNumberOfInterfaces(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[58 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetNumberOfInterfaces")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[58 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetOwnerModuleFromTcp6Entry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[59 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetOwnerModuleFromTcp6Entry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[59 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetOwnerModuleFromTcpEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[60 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetOwnerModuleFromTcpEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[60 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetOwnerModuleFromUdp6Entry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[61 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetOwnerModuleFromUdp6Entry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[61 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetOwnerModuleFromUdpEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[62 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetOwnerModuleFromUdpEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[62 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetPerAdapterInfo(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[63 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetPerAdapterInfo")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[63 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetRTTAndHopCount(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[64 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetRTTAndHopCount")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[64 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpExTable2FromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[65 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpExTable2FromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[65 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpStatistics(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[66 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpStatistics")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[66 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpStatisticsEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[67 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpStatisticsEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[67 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpStatsFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[68 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpStatsFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[68 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpStatsFromStackEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[69 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpStatsFromStackEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[69 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[70 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[70 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetTcpTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[71 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetTcpTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[71 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpExTable2FromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[72 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpExTable2FromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[72 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpStatistics(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[73 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpStatistics")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[73 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpStatisticsEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[74 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpStatisticsEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[74 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpStatsFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[75 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpStatsFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[75 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpStatsFromStackEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[76 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpStatsFromStackEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[76 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[77 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[77 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUdpTableFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[78 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUdpTableFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[78 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_GetUniDirectionalAdapterInfo(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[79 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("GetUniDirectionalAdapterInfo")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[79 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_Icmp6CreateFile(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[80 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("Icmp6CreateFile")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[80 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_Icmp6ParseReplies(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[81 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("Icmp6ParseReplies")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[81 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_Icmp6SendEcho2(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[82 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("Icmp6SendEcho2")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[82 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IcmpCloseHandle(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[83 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IcmpCloseHandle")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[83 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IcmpCreateFile(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[84 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IcmpCreateFile")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[84 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IcmpParseReplies(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[85 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IcmpParseReplies")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[85 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IcmpSendEcho2(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[86 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IcmpSendEcho2")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[86 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IcmpSendEcho(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[87 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IcmpSendEcho")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[87 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalCreateIpForwardEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[88 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalCreateIpForwardEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[88 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalCreateIpNetEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[89 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalCreateIpNetEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[89 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalDeleteIpForwardEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[90 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalDeleteIpForwardEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[90 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalDeleteIpNetEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[91 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalDeleteIpNetEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[91 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalGetIfTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[92 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalGetIfTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[92 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalGetIpAddrTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[93 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalGetIpAddrTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[93 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalGetIpForwardTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[94 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalGetIpForwardTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[94 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalGetIpNetTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[95 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalGetIpNetTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[95 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalGetTcpTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[96 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalGetTcpTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[96 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalGetUdpTable(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[97 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalGetUdpTable")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[97 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalSetIfEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[98 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalSetIfEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[98 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalSetIpForwardEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[99 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalSetIpForwardEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[99 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalSetIpNetEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[100 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalSetIpNetEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[100 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalSetIpStats(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[101 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalSetIpStats")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[101 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InternalSetTcpEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[102 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("InternalSetTcpEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[102 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IpReleaseAddress(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[103 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IpReleaseAddress")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[103 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IpRenewAddress(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[104 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IpRenewAddress")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[104 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IsLocalAddress(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[105 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("IsLocalAddress")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[105 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NTPTimeToNTFileTime(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[106 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NTPTimeToNTFileTime")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[106 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NTTimeToNTPTime(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[107 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NTTimeToNTPTime")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[107 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NhGetGuidFromInterfaceName(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[108 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NhGetGuidFromInterfaceName")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[108 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NhGetInterfaceNameFromDeviceGuid(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[109 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NhGetInterfaceNameFromDeviceGuid")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[109 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NhGetInterfaceNameFromGuid(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[110 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NhGetInterfaceNameFromGuid")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[110 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NhpAllocateAndGetInterfaceInfoFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[111 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NhpAllocateAndGetInterfaceInfoFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[111 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NhpGetInterfaceIndexFromStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[112 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NhpGetInterfaceIndexFromStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[112 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NotifyAddrChange(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[113 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NotifyAddrChange")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[113 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NotifyRouteChange(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[114 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NotifyRouteChange")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[114 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_NotifyRouteChangeEx(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[115 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("NotifyRouteChangeEx")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[115 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfAddFiltersToInterface(long, long, long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[116 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfAddFiltersToInterface@24")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[116 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfAddGlobalFilterToInterface(long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[117 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfAddGlobalFilterToInterface@8")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[117 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfBindInterfaceToIPAddress(long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[118 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfBindInterfaceToIPAddress@12")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[118 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfBindInterfaceToIndex(long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[119 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfBindInterfaceToIndex@16")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[119 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfCreateInterface(long, long, long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[120 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfCreateInterface@24")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[120 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfDeleteInterface(long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[121 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfDeleteInterface@4")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[121 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfDeleteLog(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[122 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfDeleteLog@0")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[122 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfGetInterfaceStatistics(long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[123 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfGetInterfaceStatistics@16")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[123 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfMakeLog(long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[124 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfMakeLog@4")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[124 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfRebindFilters(long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[125 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfRebindFilters@8")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[125 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfRemoveFilterHandles(long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[126 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfRemoveFilterHandles@12")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[126 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfRemoveFiltersFromInterface(long, long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[127 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfRemoveFiltersFromInterface@20")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[127 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfRemoveGlobalFilterFromInterface(long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[128 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfRemoveGlobalFilterFromInterface@8")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[128 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfSetLogBuffer(long, long, long, long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[129 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfSetLogBuffer@28")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[129 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfTestPacket(long, long, long, long, long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[130 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfTestPacket@20")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[130 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALSTD PfUnBindInterface(long)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[131 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("_PfUnBindInterface@4")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[131 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_RestoreMediaSense(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[132 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("RestoreMediaSense")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[132 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SendARP(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[133 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SendARP")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[133 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetAdapterIpAddress(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[134 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetAdapterIpAddress")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[134 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetBlockRoutes(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[135 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetBlockRoutes")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[135 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIfEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[136 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIfEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[136 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIfEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[137 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIfEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[137 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpForwardEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[138 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpForwardEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[138 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpForwardEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[139 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpForwardEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[139 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpMultihopRouteEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[140 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpMultihopRouteEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[140 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpNetEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[141 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpNetEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[141 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpNetEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[142 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpNetEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[142 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpRouteEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[143 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpRouteEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[143 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpStatistics(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[144 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpStatistics")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[144 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpStatsToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[145 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpStatsToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[145 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetIpTTL(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[146 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetIpTTL")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[146 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetProxyArpEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[147 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetProxyArpEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[147 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetRouteWithRef(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[148 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetRouteWithRef")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[148 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetTcpEntry(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[149 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetTcpEntry")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[149 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetTcpEntryToStack(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[150 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("SetTcpEntryToStack")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[150 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_UnenableRouter(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[151 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("UnenableRouter")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[151 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_do_echo_rep(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[152 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("do_echo_rep")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[152 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_do_echo_req(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[153 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("do_echo_req")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[153 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_register_icmp(void)
{
	// 保存返回地址到 TLS
	__asm PUSH m_dwReturn[154 * TYPE long];
	__asm CALL DWORD PTR [TlsSetValue];

	// 调用原始函数
	GetAddress("register_icmp")();

	// 获取返回地址并返回
	__asm PUSH EAX;
	__asm PUSH m_dwReturn[154 * TYPE long];
	__asm CALL DWORD PTR [TlsGetValue];
	__asm XCHG EAX, [ESP];
	__asm RET;
}

// CiphlpapiApp 初始化

BOOL CiphlpapiApp::InitInstance()
{
	CWinApp::InitInstance();
	DebugMessage("123312321323");
	for (INT i = 0; i < sizeof(m_dwReturn) / sizeof(DWORD); i++)
	{
		m_dwReturn[i] = TlsAlloc();
	}
	setlocale( LC_ALL, "chs" );
	DebugMessage("123312321323");
	Hook_Ls::Hook_Ls();
	DebugMessage("123312321323_2");
	return Load();
}
