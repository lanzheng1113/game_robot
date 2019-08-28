#include "stdafx.h"
#include "fdoffset.h"
#include "debugmessage.h"
#include <Tlhelp32.h>
#include "NVDSNetHeader.h"
#include "NetBase.h"
#include "EX/binsig.h"
#include "offsetindex/OffsetIndexSoul.h"
HANDLE Handle_SoulLogin_dll;	DWORD ModuleSearchArea_SoulLogin_dll;
HANDLE Handle_3dRole_dll;		DWORD ModuleSearchArea_3dRole_dll;
HANDLE Handle_3DGameMap_dll;	DWORD ModuleSearchArea_3DGameMap_dll;
HANDLE Handle_Soul_exe;			DWORD ModuleSearchArea_Soul_exe;
HANDLE Handle_SimFlash_dll;		DWORD ModuleSearchArea_SimFlash_dll;
static DWORD getTheModuleHandles(){
	Handle_SoulLogin_dll = GetModuleHandle("SoulLogin.dll");
	if (Handle_SoulLogin_dll == 0){
		DebugMessage("Get module handle SoulLogin.dll failed with error %d.",GetLastError());
		return 1;
	}

	Handle_3dRole_dll = GetModuleHandle("3dRole.dll");
	if (Handle_3dRole_dll == 0){
		DebugMessage("Get module handle 3dRole.dll faield with error %d",GetLastError());
		return 1;
	}
	Handle_3DGameMap_dll = GetModuleHandle("3DGameMap.dll");
	if (0 == Handle_3DGameMap_dll){
		DebugMessage("Get module handle 3DGameMap.dll failed with error %d",GetLastError());
		return 1;
	}

	Handle_Soul_exe = GetModuleHandle("Soul.exe");
	if (0 == Handle_Soul_exe){
		DebugMessage("Get module handle soul.exe failed with error %d",GetLastError());
		return 1;
	}
	Handle_SimFlash_dll = GetModuleHandle("SimFlash.dll");
	if (0 == Handle_SimFlash_dll){
		DebugMessage("Get module handle SimFlash.dll failed with error %d",GetLastError());
	}
	return 0;
}

static DWORD getModuleSizes()
{
	//SoulLogin.0x2190
	DWORD dwPID = 0;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		DebugMessage( "[getLoginCrackPosition]CreateToolhelp32Snapshot (of modules) %d",GetLastError() );
		return 0;
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof( MODULEENTRY32 );

	// Retrieve information about the first module,
	// and exit if unsuccessful
	if( !Module32First( hModuleSnap, &me32 ) )
	{
		DebugMessage( "Module32First" ); // Show cause of failure
		CloseHandle( hModuleSnap );    // Must clean up the
		//   snapshot object!
		return 0;
	}

	// Now walk the module list of the process,
	// and display information about each module
	int rt = 0;
	do
	{
		if (lstrcmpi("SoulLogin.dll",me32.szModule) == 0){
			ModuleSearchArea_SoulLogin_dll = me32.modBaseSize;
			DebugMessageA("SoulLogin size = %x",ModuleSearchArea_SoulLogin_dll);
		}

		if (lstrcmpi("3DRole.dll",me32.szModule) == 0){
			ModuleSearchArea_3dRole_dll = me32.modBaseSize;
			DebugMessageA("3dRole size = %x",ModuleSearchArea_3dRole_dll);
		}

		if (lstrcmpi("3DGameMap.dll",me32.szModule) == 0){
			ModuleSearchArea_3DGameMap_dll = me32.modBaseSize;
			DebugMessageA("3DGameMap size = %x",ModuleSearchArea_3DGameMap_dll);
		}

		if (lstrcmpi("soul.exe",me32.szModule) == 0){
			ModuleSearchArea_Soul_exe = me32.modBaseSize;
			DebugMessageA("3DGameMap size = %x",ModuleSearchArea_Soul_exe);
		}

		if (lstrcmpi("SimFlash.dll",me32.szModule) == 0){
			ModuleSearchArea_SimFlash_dll = me32.modBaseSize;
			DebugMessageA("SimFlash size = %x",ModuleSearchArea_SimFlash_dll);
		}
	} while( Module32Next( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );
	return rt;
}

DWORD PostOffsetsToServer( DWORD* pOffset, DWORD count ) 
{
	int iResult;
	WSADATA wsaData;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	SOCKET ConnectSocket;
	struct sockaddr_in clientService; 

	int recvbuflen = 4096;
	char sendMessage[100] = "Client: sending data test";
	char recvbuf[4096] = "";
	
	//----------------------
	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	clientService.sin_family = AF_INET;
	char szip[100] = {0};
	GetPrivateProfileString("option","ip","59.125.161.132",szip,sizeof(szip),"D:\\AutoUpdateOpt.ini");
	int port = GetPrivateProfileInt("option","port",0,"D:\\AutoUpdateOpt.ini");
	if (port == 0)
	{
		AfxMessageBox("自动更新的配置文件有错。");
		ExitProcess(0);
	}
	clientService.sin_addr.s_addr = inet_addr( szip );
	clientService.sin_port = htons( (USHORT)port );

	//----------------------
	// Connect to server.
	iResult = connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) );
	if (iResult == SOCKET_ERROR) {
		DebugMessageA( "connect failed with error: %d\n", WSAGetLastError() );
		closesocket(ConnectSocket);
		return 1;
	}

	int i = 0;
	PACKETGENERALHEAD ppg;
	ppg.CheckSum = 0;
	ppg.GameType = 1;
	ppg.GameVersion = 1;
	ppg.HeadFlag = AffairType_Dovalidate;
	ppg.HeadUserID = 0;
	ppg.MesgFullType = MsgRemoteUpdateOffset;
	ppg.PacketLen = sizeof(ppg);
	ppg.WgVersion = 0;

	PacketRemoteUpdateOffset pruo;
	memcpy(pruo.Offset,pOffset,min(sizeof(pruo.Offset),count*sizeof(DWORD)));
	pruo.pgh = ppg;
	
	DebugMessageA("sizeof pruo = %d",sizeof(pruo));
	char sendbuf[2048] = {0};
	//组织数据发送。
	*(WORD*)sendbuf = sizeof(pruo);
	memcpy(sendbuf+2,&pruo,sizeof(pruo));
	iResult = send(ConnectSocket,sendbuf,sizeof(pruo)+2,0);
	if (iResult == SOCKET_ERROR) {
		DebugMessageA("send() failed with error: %d\n", WSAGetLastError());
	}else{
		DebugMessageA("Bytes Sent: %d\n", iResult);
		int iRecvCount = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		DebugMessageA("Byte recved %d",iRecvCount);
		if ( iRecvCount > 0 ){
			AfxMessageBox("恭喜恭喜！更新成功了。");
			closesocket(ConnectSocket);
			return 0;
		}else if ( iRecvCount == 0 ){
			AfxMessageBox("更新失败！与服务器的链接断开。");
			closesocket(ConnectSocket);
			ExitProcess(0);
		}else{
			::MessageBox(NULL,"未知的错误402",NULL,MB_OK);
			closesocket(ConnectSocket);
			ExitProcess(0);
		}
	}
	closesocket(ConnectSocket);
	return 0;
}

static void RunGetOffset(){
	DebugMessage("准备：等待游戏加载所有DLL");
	Sleep(1000);
	int icbc = 0;
	int ret = 0;
	while (icbc++ < 10){ //有些SOB的机器会很迟才加载SoulLogin.dll
		ret = getTheModuleHandles(); /*+ 0x2190*/
		if (ret == 0){
			break;
		}
		Sleep(1000);
		DebugMessage("等待……");
	}

	if (ret != 0){
		DebugMessage("等待发生错误。");
		return;
	}else{
		DebugMessage("Module SoulLogin.dll = 0x%08X",Handle_SoulLogin_dll);
		DebugMessage("Module 3dRole.dll = 0x%08X",Handle_3dRole_dll);
		DebugMessage("Module 3dGameMap.dll = 0x%08X",Handle_3DGameMap_dll);
		DebugMessage("Module Soul.exe = 0x%08X",Handle_Soul_exe);
	}

	//需要的DLL已经全部加载，获得DLL的模块大小载入。
	if (0 != getModuleSizes()){
		DebugMessage("Error in get modules size");
	}

	DWORD g_offsetMapBase = 0;
	DWORD g_GuoLvShangYe = 0;
	DWORD G_GuoLvLoginMessageBox = 0;
	DWORD g_CloseCurWndFunOffset = 0;
	DWORD g_CloseCurWndOffset = 0;
	DWORD g_SpeedUpOffset = 0;
	DWORD g_OffsetDingWeiLiaoTian =0;
	DWORD g_OffsetGetObjHere = 0;
	DWORD g_OffsetGetObjPlayerSet = 0;
	DWORD g_OffsetObjNetwork = 0;
	DWORD g_offsetSelectQuFu = 0;
	DWORD g_offsetQuDiaoMiMaKuangLuanXiaoxi = 0;
	DWORD g_offsetPanDuanFengHao = 0;
	DWORD g_offsetEyeX = 0;
	DWORD g_offsetEyeY = 0;
	DWORD g_szAccountOffset = 0;
	DWORD g_PetEnageOffset = 0;
	DWORD g_PackageOffset = 0;
	DWORD g_cd1Offset = 0;
	DWORD g_cd2Offset = 0;
	DWORD g_RoleState = 0;
	DWORD g_LoginCallOffset = 0;
	DWORD g_MyChangeDateCALlOffset = 0;
	DWORD g_SetPasswordTableCALl = 0;
	DWORD g_loginobjOffset = 0;
	DWORD g_PasswordTableObjOffset = 0;
	DWORD g_PasswordObjOffset = 0;
	DWORD g_AccountObjOffset = 0;
	DWORD g_SimFlashobjOffset = 0;
	DWORD g_SelectLargeBigAreaCAllOffset = 0;
	DWORD g_SelectedSmallAreaCAllOffset = 0;
	DWORD g_SelectedVersionCAllOffset = 0;
	DWORD g_CreateHero3Offset = 0;
	DWORD g_CallCreateHeroOffset = 0;
	DWORD g_CBooththisOffset = 0;
	DWORD g_CreateHeroObjOffset = 0;
	DWORD g_CreateHeroObj2Offset = 0;
	DWORD g_callSetHwndText = 0;
	DWORD g_GameMessageBoxConten = 0;
	/*---------------------------------------------------------------------------*/
	char jzs1[] = "FF15????????84c00f85????????8B83????????"; //过滤商业   .exe
	DebugMessage("processe 过滤商业=%s",jzs1);
	DWORD searchBeginPos = (DWORD)Handle_Soul_exe;
	DWORD searchEndPos = (DWORD)Handle_Soul_exe+ModuleSearchArea_Soul_exe;
	g_GuoLvShangYe = findOffsets(jzs1,searchBeginPos,searchEndPos);
	g_GuoLvShangYe += 0x22;

	/*---------------------------------------------------------------------------*/
	char win222[] = "8bcee8????????85c074326a008bcbe8????????"; //过滤登录弹窗口   .exe
	DebugMessage("processe 过滤Lg弹窗口=%s",win222);
	//		DebugMessage("processe 过滤Lg弹窗口");
	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe+ModuleSearchArea_Soul_exe;
	G_GuoLvLoginMessageBox = findOffsets(win222,searchBeginPos,searchEndPos);
	G_GuoLvLoginMessageBox += 0x09;
	/*---------------------------------------------------------------------------*/
	char winxa[] = "FF92F400000085c00f84????????a1????????8b88????????8b89????????"; //得到关闭窗口的全局变量 .exe
	DebugMessage("processe 得到关闭窗口的全局变量");
	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe+ModuleSearchArea_Soul_exe;
	g_CloseCurWndOffset = findOffsets(winxa,searchBeginPos,searchEndPos);
	g_CloseCurWndOffset += 0x0f;
	g_CloseCurWndOffset = *(DWORD*)g_CloseCurWndOffset;

	/*---------------------------------------------------------------------------*/
	char wndpsr[] = "E8????????8B8F????????6A006A01E8????????";
	DebugMessage("processe 得到关闭窗口的函数");
	searchBeginPos = (DWORD)Handle_Soul_exe;
	//searchEndPos = (DWORD)Handle_Soul_exe+ModuleSearchArea_Soul_exe;
	//g_CloseCurWndFunOffset = findOffsets(wndpsr,searchBeginPos,searchEndPos);
	//int ibc = *(int*)(g_CloseCurWndFunOffset+1);
	//DebugMessage("x2o:%d",ibc);
	//g_CloseCurWndFunOffset = g_CloseCurWndFunOffset + ibc + 5;
	g_CloseCurWndFunOffset = 0;

	/*---------------------------------------------------------------------------*/
	char winxb[] = "df6df4d945d0dc";                              //+速度特征   3DRole //这个是函数+偏移，一般不用更新
	DebugMessage("processe +速度特征=%s",winxb);
	searchBeginPos = (DWORD)Handle_3dRole_dll;
	searchEndPos = (DWORD)Handle_3dRole_dll+ModuleSearchArea_3dRole_dll;
	g_SpeedUpOffset = findOffsets(winxb,searchBeginPos,searchEndPos);
	g_SpeedUpOffset += 0x1A9;
	g_SpeedUpOffset -= searchBeginPos;
	/*---------------------------------------------------------------------------*/
	char winxc[] = "8D85????????506A038B8D????????81C108040000FF15????????8D8D????????51E8";  //定位取聊天信息 3DRole
	DebugMessage("processe 定位取交易信息 %s",winxc);
	searchBeginPos = (DWORD)Handle_3dRole_dll;
	searchEndPos = (DWORD)Handle_3dRole_dll+ModuleSearchArea_3dRole_dll;
	g_OffsetDingWeiLiaoTian = findOffsets(winxc,searchBeginPos,searchEndPos);
	DebugMessage("Test fb = 0x%08X",g_OffsetDingWeiLiaoTian);
	g_OffsetDingWeiLiaoTian += 0x1b;
	DebugMessage("Test f2b = 0x%08X",g_OffsetDingWeiLiaoTian);
	g_OffsetDingWeiLiaoTian -= searchBeginPos;

	/*---------------------------------------------------------------------------*/
	char winxd[] = "E8????????85C0741485FF75188B0D";   //g_objHero/g_objPlayerSet  .exe 
	DebugMessage("processe g_objHero %s",winxd);
	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe+ModuleSearchArea_Soul_exe;
	g_OffsetGetObjPlayerSet = g_OffsetGetObjHere = findOffsets(winxd,searchBeginPos,searchEndPos);
	g_OffsetGetObjHere -= 0x61;
	g_OffsetGetObjHere = *(DWORD*)g_OffsetGetObjHere; //需要再区一次，这里相当与原来的宏。

	g_OffsetGetObjPlayerSet -= 0x1B;
	g_OffsetGetObjPlayerSet = *(DWORD*)g_OffsetGetObjPlayerSet;

	/*---------------------------------------------------------------------------*/
	char winxe[] = "33C9668B4804518B55FC83C20452B9????????";//g_objNetwork 3DRole
	DebugMessage("processe g_objNetwork %s",winxe);
	searchBeginPos = (DWORD)Handle_3dRole_dll;
	searchEndPos = (DWORD)Handle_3dRole_dll+ModuleSearchArea_3dRole_dll;
	g_OffsetObjNetwork = findOffsets(winxe,searchBeginPos,searchEndPos);
	g_OffsetObjNetwork += 0x0f;
	g_OffsetObjNetwork = *(DWORD*)g_OffsetObjNetwork;
	g_OffsetObjNetwork -= searchBeginPos;

	/*---------------------------------------------------------------------------*/
	char winxf[] = "B93F00000033C08DBC247D020000889C247C020000F3AB66AB"; //选择服,区特征 SoulLogi
	DebugMessage("processe 选择服,区 %s",winxf);
	searchBeginPos = (DWORD)Handle_SoulLogin_dll;
	searchEndPos = (DWORD)Handle_SoulLogin_dll+ModuleSearchArea_SoulLogin_dll;
	g_offsetSelectQuFu = findOffsets(winxf,searchBeginPos,searchEndPos);
	g_offsetSelectQuFu -= 0x0e;
	g_offsetSelectQuFu -= searchBeginPos;

	/*---------------------------------------------------------------------------*/
	char winxg[] = "53568B35????????576a018bd9";   //去掉乱输入键盘信息  SoulLogi
	DebugMessage("processe 去掉乱输入键盘信息 %s",winxg);
	searchBeginPos = (DWORD)Handle_SoulLogin_dll;
	searchEndPos = (DWORD)Handle_SoulLogin_dll+ModuleSearchArea_SoulLogin_dll;
	g_offsetQuDiaoMiMaKuangLuanXiaoxi = findOffsets(winxg,searchBeginPos,searchEndPos);
	g_offsetQuDiaoMiMaKuangLuanXiaoxi -= 1;
	g_offsetQuDiaoMiMaKuangLuanXiaoxi -= searchBeginPos;
	/*---------------------------------------------------------------------------*/
	char winxh[] = "81EC????????538B9C24????????555685DB0F84????????8BAC24????????81";  //登录器的recv明文封号判断
	DebugMessage("processe 登录器的recv明文封号判断 %s",winxh);
	searchBeginPos = (DWORD)Handle_SoulLogin_dll;
	searchEndPos = (DWORD)Handle_SoulLogin_dll+ModuleSearchArea_SoulLogin_dll;
	g_offsetPanDuanFengHao = findOffsets(winxh,searchBeginPos,searchEndPos);
	g_offsetPanDuanFengHao = g_offsetPanDuanFengHao - searchBeginPos;
	/*--------------------------------------------------------------------------*/
	char winxz[] = "8BC8E8????????A3????????8B4C240464890D0000000083C410C3"; //   地图基址 3DGameMap
	DebugMessage("processe 地图基址 %s",winxz);
	searchBeginPos = (DWORD)Handle_3DGameMap_dll;
	searchEndPos = (DWORD)Handle_3DGameMap_dll+ModuleSearchArea_3DGameMap_dll;
	g_offsetMapBase = findOffsets(winxz,searchBeginPos,searchEndPos);
	g_offsetMapBase += 8;
	g_offsetMapBase = *(DWORD*)g_offsetMapBase;
	g_offsetMapBase -= searchBeginPos;
	/*--------------------------------------------------------------------------*/
	char winxz1[] = "8B????8B????85??8B91????????8b89????????89????89????"; //   地图基址
	/*
	8B ?? ?? 8B ?? ?? 85 ??
	8B 91 ?? ?? ?? ?? //变量 y
	8b 89 ?? ?? ?? ?? //变量 X
	89????89????
	*/
	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe + ModuleSearchArea_Soul_exe;
	DWORD offsetTemp = findOffsets(winxz1,searchBeginPos,searchEndPos);
	DWORD offsetTempX = offsetTemp + 16;
	g_offsetEyeX = *(DWORD*)offsetTempX;
	DWORD offsetTempY = offsetTemp + 10;
	g_offsetEyeY = *(DWORD*)offsetTempY;
	/*/
	$3DRole.dll
	//													  //变量PackageOffset
	55 8B EC 51 89 4D FC 83 7D 08 0A 75 10 8B 45 FC 8b 88  ?? ?? ?? ?? e8*/
	/*--------------------------------------------------------------------------*/
	char winx_PackageOffset[] = "558BEC51894DFC837D080A75108B45FC8b88????????e8"; // 
	searchBeginPos = (DWORD)Handle_3dRole_dll;
	searchEndPos = (DWORD)Handle_3dRole_dll + ModuleSearchArea_3dRole_dll;
	offsetTemp = findOffsets(winx_PackageOffset,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + 0x12;
	g_PackageOffset = *(DWORD*)offsetTemp;
	
	/*
	68????????6a??E8????????8b??8b??ff????8b0d????????508b09FF15????????85C08945??0F84????????8b??????????8b??<V BB>
	*/
	char winx_PetTiLi[] = "68????????6a??E8????????8b??8b??ff????8b0d????????508b09FF15????????85C08945??0F84????????8b??????????8b??"; //宝宝体力
	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe + ModuleSearchArea_Soul_exe;
	offsetTemp = findOffsets(winx_PetTiLi,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + strlen(winx_PetTiLi)/2;
	g_PetEnageOffset = *(DWORD*)offsetTemp;

	/*
	8B 0D <v loginobj> 85 C9 74 24 8B ?? ???????? 85 C0 74 1A E8 ???????? 85 C0 74 11
	*/
	char winx_szAccount[] = "8B0D????????85C974248B??????????85C0741AE8????????85C07411"; //账号偏移  
	searchBeginPos = (DWORD)Handle_SoulLogin_dll;
	searchEndPos = (DWORD)Handle_SoulLogin_dll + ModuleSearchArea_SoulLogin_dll;
	offsetTemp = findOffsets(winx_szAccount,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + 2;
	g_szAccountOffset = *(DWORD*)offsetTemp;
	g_szAccountOffset = g_szAccountOffset - (DWORD)Handle_SoulLogin_dll;


	/*/
	A1 ?? ?? ?? ?? 8B 00 8B 88 <V CD> 85 C9 75 0A 8B 88 <V CD2> 85 C9 74 10 8B ?? 8B ??
	/*/
	char winx_CD[] = "A1????????8B008B88????????85C9750A8B88????????85C974108B??8B??"; //CD,CD1偏移
	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe + ModuleSearchArea_Soul_exe;
	offsetTemp = findOffsets(winx_CD,searchBeginPos,searchEndPos);
	DWORD cd1offsetTemp = offsetTemp + 9;
	DWORD cd2offsetTemp = offsetTemp + 19;
	g_cd1Offset = *(DWORD*)cd1offsetTemp;
	g_cd2Offset = *(DWORD*)cd2offsetTemp;


// 	HMODULE hins = GetModuleHandle("3DRole.dll");
// 	DWORD TestStatusFun = (DWORD)GetProcAddress(hins,(LPCTSTR)"?TestStatus@CRole@@QBEHVInt256@@@Z");//TestStatusFun
// 	DWORD temp1 = *(BYTE*)(TestStatusFun + (0x017AFB0D - 0x017AFADD) + 2);//=0x10
// 	HMODULE hinsBase = GetModuleHandle("BaseCode.dll");
// 	DWORD XXXXX = (DWORD)GetProcAddress(hinsBase,(LPCTSTR)"??I@YA?AVInt256@@ABV0@0@Z");//TestStatusFun
// 	DWORD temp2 = *(BYTE*)(XXXXX + (0x002AF8B2-0x002AF880) + 2);//=0x20
// 	XXXXX = (DWORD)GetProcAddress(hinsBase,(LPCTSTR)"??I@YA?AVInt128@@ABV0@0@Z");//TestStatusFun
// 	DWORD temp3 = *(BYTE*)(XXXXX + (0x002BD2CF - 0x002BD280) + 2);//=0x8
// 	g_RoleState = temp1 + temp2 + temp3;
	g_RoleState = 0x38;
	/*
	!SoulLogin.dll
	*/
	/*<A LoginCall> 8b ?????????? 85. 74 17 8B 54 24 04 85 D2 74 0F 8B 44 24 08 85 C0 74 07 . .e8*/
	char win_LoginCall[] = "8b??????????85??74178B54240485D2740F8B44240885C07407????e8";
	searchBeginPos = (DWORD)Handle_SoulLogin_dll;
	searchEndPos = (DWORD)Handle_SoulLogin_dll + ModuleSearchArea_SoulLogin_dll;
	offsetTemp = findOffsets(win_LoginCall,searchBeginPos,searchEndPos);
	g_LoginCallOffset = offsetTemp - (DWORD)Handle_SoulLogin_dll;
	/*
	<A MyChangeDateCALl> 81 EC .{9,f} 39 93 . . . . 75 . 8B . . . . . . 50 E8
	. . . . 5B 81 .{5} C2 . . 39 .{6} 74 07 88 .{6} 8D . . . B9 .{4} 88 50 FF 88 10 83 C0 02 49
	*/
	char win_MyChangeDateCALl[] = "	<A MyChangeDateCALl>81EC .{9,f} 39 93 . . . . 75 . 8B . . . . . . 50 E8. . . . 5B 81 .{5} C2 . . 39 .{6} 74 07 88 .{6} 8D . . . B9 .{4} 88 50 FF 88 10 83 C0 02 49";
	binsig::report rpt = binsig::Find(searchBeginPos,searchEndPos,win_MyChangeDateCALl);
	g_MyChangeDateCALlOffset = binsig::Get(rpt);
	g_MyChangeDateCALlOffset -= searchBeginPos;

	/*<A SetPasswordTableCALl> 53 56 .{20,45} 7E 0B 0F . . . 03 D3 40 3B C1 7C F5 . ff15....*/
	char win_SetPasswordTableCALl[] = "<A SetPasswordTableCALl> 53 56 .{20,45} 7E 0B 0F . . . 03 D3 40 3B C1 7C F5 . ff15....";
	binsig::report rtp_SetPasswordTableCALl = binsig::Find(searchBeginPos,searchEndPos,win_SetPasswordTableCALl);
	g_SetPasswordTableCALl = binsig::Get(rtp_SetPasswordTableCALl);
	g_SetPasswordTableCALl -= searchBeginPos;
	
	/*!SoulLogin.dll 相对偏移*/
	/*8B 0D <v loginobj> 85 C9 74 24 8B . .... 85 C0 74 1A E8 .... 85 C0 74 11*/
	char win_loginobj[] = "8B0D????????85C974248B??????????85C0741AE8????????85C07411";
	searchBeginPos = (DWORD)Handle_SoulLogin_dll;
	searchEndPos = (DWORD)Handle_SoulLogin_dll + ModuleSearchArea_SoulLogin_dll;
	offsetTemp = findOffsets(win_loginobj,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + 2;
	g_loginobjOffset = *(DWORD*)offsetTemp;
	g_loginobjOffset = g_loginobjOffset - (DWORD)Handle_SoulLogin_dll;

	/*$SoulLogin.dll 绝对值*/
	/*7D . 33 . 85 . 0F 9C C0 48 23 C1 EB . 8B ... 6A 00 55 8D . <v PasswordTableObjOffset> 53 89 .. C7*/
	char win_PasswordTableObj[] = "7D??33??85??0F9CC04823C1EB??8B??????6A00558D?? ?? ?? ?? ?? 5389????C7";
	offsetTemp = findOffsets(win_PasswordTableObj,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + 23;
	g_PasswordTableObjOffset = *(DWORD*)offsetTemp;

	/*$SoulLogin.dll 绝对值*/
	/*39 9D . . . . 75 7F 8D 8C . . . . . FF 15 . . . . 8B 3D . . . . 8D B5 <v PasswordObjOffset>  56 8D 8C 24 . . . . 89 9C 24 . . . . FF D7 68 . . . . 8B CE*/
	char win_PasswordObjOffset[] = "39 9D ?? ?? ?? ?? 75 7F 8D 8C ?? ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 8B 3D ?? ?? ?? ?? 8D B5 ?? ?? ?? ??  56 8D 8C 24 ?? ?? ?? ?? 89 9C 24 ?? ?? ?? ?? FF D7 68 ?? ?? ?? ?? 8B CE";
	offsetTemp = findOffsets(win_PasswordObjOffset,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + 29;
	g_PasswordObjOffset = *(DWORD*)offsetTemp;
	
	/*$SoulLogin.dll*/
	/*F3 . 66 . 68 00 01 00 00 52 8D . <v AccountObjOffset> 89 .{5}  AA FF 15 .{4}  8D .{6}
	8D .{5} 50 FF D5 8B .{5} 3B C3 74 0A 6A 03*/
	char win_AccountObjOffset[] = "F3 ?? 66 ?? 68 00 01 00 00 52 8D ?? ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? AA FF 15 ?? ?? ?? ??  8D ?? ?? ?? ?? ?? ?? 8D ?? ?? ?? ?? ?? \
								  50 FF D5 8B ?? ?? ?? ?? ?? 3B C3 74 0A 6A 03";
	offsetTemp = findOffsets(win_AccountObjOffset,searchBeginPos,searchEndPos);
	offsetTemp = offsetTemp + 12;
	g_AccountObjOffset = *(DWORD*)offsetTemp;
	/*!SimFlash.dll*/
	/*8B . <v SimFlashobj> E8 .... 8B 4C 24 04 3B C8 7D 0C 51 8B 0D .... E8 ....    C2 04 00*/
	char win_SimFlashobj[] = "8B ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 4C 24 04 3B C8 7D 0C 51 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C2 04 00";
	searchBeginPos = (DWORD)Handle_SimFlash_dll;
	searchEndPos = (DWORD)Handle_SimFlash_dll + ModuleSearchArea_SimFlash_dll;
	offsetTemp = findOffsets(win_SimFlashobj,searchBeginPos,searchEndPos);
	offsetTemp = *(DWORD*)(offsetTemp+2);
	g_SimFlashobjOffset = offsetTemp - searchBeginPos;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	/*
	<A SelectLargeBigAreaCAll> 55 8B EC 81 EC .{20,40} FF . 6A 05 8D . . . . . FF . B9 3F . . . 33 C0 8D . . . . . C6 .
	. . . . 00 F3 AB 66 AB AA 8D 85 . . . . 6A 0A
	*/
	char win_SelectLargeBigAreaCAll[] = "<A SelectLargeBigAreaCAll> 55 8B EC 81 EC .{20,40} FF . 6A 05 8D . . . . . FF . B9 3F . . . 33 C0 8D . . . . . C6 . . . . . 00 F3 AB 66 AB AA 8D 85 . . . . 6A 0A";
	binsig::report rptSelectLargeBigAreaCAll = binsig::Find(searchBeginPos,searchEndPos,win_SelectLargeBigAreaCAll);
	g_SelectLargeBigAreaCAllOffset = binsig::Get(rptSelectLargeBigAreaCAll);
	g_SelectLargeBigAreaCAllOffset -= searchBeginPos;
	/*
	<A SelectedSmallAreaCAll> 55 8B EC 81 EC . . . . 53 8B . . 56 8B . 57 6A 00 8D . . . . . 89 . . . . . FF
	. . . . . B9 3F . . .  33 C0 8D . . . . . C6 . . . . . . F3 AB
	*/
	char win_SelectedSmallAreaCAll[] = "<A SelectedSmallAreaCAll> 55 8B EC 81 EC . . . . 53 8B . . 56 8B . 57 6A 00 8D . . . . . 89 . . . . . FF . . . . . B9 3F . . .  33 C0 8D . . . . . C6 . . . . . . F3 AB";
	binsig::report rptSelectedSmallAreaCAll = binsig::Find(searchBeginPos,searchEndPos,win_SelectedSmallAreaCAll);
	g_SelectedSmallAreaCAllOffset = binsig::Get(rptSelectedSmallAreaCAll);
	g_SelectedSmallAreaCAllOffset -= searchBeginPos;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	/*
	<A SelectedVersionCAll> 55 8B EC 81 EC .{12,15} 6A 00 8D .{5} 89
	.{5} FF . 8B . E8 . . . . 8D . . . . . 6A 05 8B . FF . B9 3F . . . 33
	C0 8D . . . . . C6 . 
	*/
	char win_SelectedVersionCAll[] = "<A SelectedVersionCAll> 55 8B EC 81 EC .{12,15} 6A 00 8D .{5} 89 .{5} FF . 8B . E8 . . . . 8D . . . . . 6A 05 8B . FF . B9 3F . . . 33 C0 8D . . . . . C6 . ";
	binsig::report rptSelectedVersionCAll = binsig::Find(searchBeginPos,searchEndPos,win_SelectedVersionCAll);
	g_SelectedVersionCAllOffset = binsig::Get(rptSelectedVersionCAll);
	g_SelectedVersionCAllOffset -= searchBeginPos;

	/* !
	<A callSetHwndText>   55 8B EC .{4-8} 85 . 74 . 8B .. 85 . 74 . 8B .{2-8} 85 . 74 . . . FF .
	{4-10}C2 
	*/
	char win_callSetHwndText[] = "<A callSetHwndText> 55 8B EC .{4-8} 85 . 74 . 8B .. 85 . 74 . 8B .{2-8} 85 . 74 . . . FF ";
	binsig::report rptcallSetHwndText = binsig::Find(searchBeginPos,searchEndPos,win_callSetHwndText);
	g_callSetHwndText = binsig::Get(rptcallSetHwndText);
	g_callSetHwndText -= searchBeginPos;


	searchBeginPos = (DWORD)Handle_Soul_exe;
	searchEndPos = (DWORD)Handle_Soul_exe+ModuleSearchArea_Soul_exe;
	DebugMessage("*****************************************************************************");
	DebugMessage("soul.exe B-E(%x,%x)",searchBeginPos,searchEndPos);
	/*$soul.exe*/
	//	/
	//	85 . 74 . 8B . <v CreateHero3> 8D .{2-8} FF .{5-15} 85 . 74 .{4-15} 
	//FF .{5}  EB . E8 .{4} 85 . 74 .
	//	/
	char win_CreateHero3[] = "85 . 74 . 8B . <V CreateHero3> 8D .{2-8} FF .{5-15} 85 . 74 .{4-15} FF .{5}  EB . E8 .{4} 85 . 74 .";
	binsig::report rptCreateHeroObj3 = binsig::Find(searchBeginPos,searchEndPos,win_CreateHero3);
	g_CreateHero3Offset = binsig::Get(rptCreateHeroObj3);
	//	/
	//	<A CallCreateHero>.8b.68.{4}ff15.{2-10}85.74.6a.ff.{2-10}ff15.{4}50
	//	/
	char win_CallCreateHero[] = "<A CallCreateHero> . 8b . 68 .{4} ff 15 .{2-10} 85 . 74 . 6a . ff .{2-10} ff 15 .{4} 50";
	binsig::report rptCallCreateHero = binsig::Find(searchBeginPos,searchEndPos,win_CallCreateHero);
	g_CallCreateHeroOffset = binsig::Get(rptCallCreateHero);

	///
	//	8B 16 50 8B 45 0C 6A 01 50 8B CE FF 52 08 50 A1  .... 8B 08 . .  <v CBooththis> FF 15 ....
	///
	char win_CBooththis[] = "8B 16 50 8B 45 0C 6A 01 50 8B CE FF 52 08 50 A1  .... 8B 08 . .  <V CBooththis> FF 15 ....";
	binsig::report rptCBooththis = binsig::Find(searchBeginPos,searchEndPos,win_CBooththis);
	g_CBooththisOffset = binsig::Get(rptCBooththis);
	///
	//	3b.73.{2-10}ff15.{4-10} c2..ff.{5-13} a1<V CreateHeroObj>
	//	6a.8b.<C CreateHeroObj2>
	//	/
	char win_CreateHeroObj[] = "3b . 73 .{2-10} ff 15 .{4-10} c2 . . ff .{5-13} a1 <V CreateHeroObj> 6a . 8b . <C CreateHeroObj2>";
	binsig::report rptCreateHeroObj = binsig::Find(searchBeginPos,searchEndPos,win_CreateHeroObj);
	g_CreateHeroObjOffset = binsig::Get(rptCreateHeroObj,"CreateHeroObj");
	g_CreateHeroObj2Offset = binsig::Get(rptCreateHeroObj,"CreateHeroObj2");

	//定位 消息提示对话框
	//68 00 01 00 00 8B 4d 08 51 8b 55 f8 81 c2 <V GameMessageBoxContent> 52 ff 15 .... 83 c4 0c
	//
	searchBeginPos = (DWORD)Handle_3dRole_dll;
	searchEndPos = (DWORD)Handle_3dRole_dll+ModuleSearchArea_3dRole_dll;
	char GameMessageBoxContent[] = "68 00 01 00 00 8B 4d 08 51 8b 55 f8 81 c2 <V GameMessageBoxContent> 52 ff 15 .... 83 c4 0c";
	binsig::report rptGameMessageBoxContent = binsig::Find(searchBeginPos,searchEndPos,GameMessageBoxContent);
	g_GameMessageBoxConten = binsig::Get(rptGameMessageBoxContent,"GameMessageBoxContent");
	DebugMessage("*****************************************************************************");
	/*$ 绝对值
	  ! 偏移，减去模块基址*/
	DebugMessage("################################Dump 偏移################################");
	DebugMessage("全局地图BASE			g_offsetMapBase = 0x%08X",g_offsetMapBase);
	DebugMessage("全局人物对象			g_OffsetGetObjHere = 0x%08X",g_OffsetGetObjHere);
	DebugMessage("目标集合对象			g_OffsetGetObjPlayerSet = 0x%08X",g_OffsetGetObjPlayerSet);
	DebugMessage("全局网络对象			g_OffsetObjNetwork = 0x%08X",g_OffsetObjNetwork);
	DebugMessage("过滤商业弹框			g_GuoLvShangYe = 0x%08X",g_GuoLvShangYe);
	DebugMessage("过滤登录弹窗			G_GuoLvLoginMessageBox = 0x%08X",G_GuoLvLoginMessageBox);
	DebugMessage("关闭窗口函数			g_CloseCurWndFunOffset = 0x%08X",g_CloseCurWndFunOffset);
	DebugMessage("关闭窗口变量			offset_global_data_gpDlgShell = 0x%08X",g_CloseCurWndOffset);
	DebugMessage("人物加速偏移			g_SpeedUpOffset = 0x%08X",g_SpeedUpOffset);
	DebugMessage("定位交易信息			g_OffsetDingWeiLiaoTian = 0x%08X",g_OffsetDingWeiLiaoTian);
	DebugMessage("选择区服偏移			g_offsetSelectQuFu = 0x%08X",g_offsetSelectQuFu);
	DebugMessage("去乱敲密码框			g_offsetQuDiaoMiMaKuangLuanXiaoxi = 0x%08X",g_offsetQuDiaoMiMaKuangLuanXiaoxi);
	DebugMessage("判断封号偏移			g_offsetPanDuanFengHao = 0x%08X",g_offsetPanDuanFengHao);
	DebugMessage("眼睛X设定				g_offsetEyeX = 0x%08X",g_offsetEyeX);
	DebugMessage("眼睛Y设定				g_offsetEyeY = 0x%08X",g_offsetEyeY);
	DebugMessage("包裹人物偏移			g_PackageOffset = 0x%08X",g_PackageOffset);
	DebugMessage("宝宝体力偏移			g_PetEnageOffset = 0x%08X",g_PetEnageOffset);
	DebugMessage("游戏账号偏移			g_szAccountOffset = 0x%08X",g_szAccountOffset);
	DebugMessage("技能CD1偏移			g_cd1Offset = 0x%08X",g_cd1Offset);
	DebugMessage("技能CD2偏移			g_cd2Offset = 0x%08X",g_cd2Offset);
	DebugMessage("RoleStete				g_RoleState = 0x%08X",g_RoleState);
	DebugMessage("摊位this				g_CBooththisOffset = 0x%08X",g_CBooththisOffset);
	DebugMessage("登录确认删除角色		g_CreateHero3Offset = 0x%08X",g_CreateHero3Offset);
	DebugMessage("登录确认删除角色		g_CreateHeroObjOffset = 0x%08X",g_CreateHeroObjOffset);
	DebugMessage("登录确认删除角色		g_CreateHeroObj2Offset = 0x%08X",g_CreateHeroObj2Offset);
	DebugMessage("登录确认删除CALL		g_CallCreateHeroOffset = 0x%08X",g_CallCreateHeroOffset);
	DebugMessage("登录选服				g_SelectedVersionCAllOffset = 0x%08X",g_SelectedVersionCAllOffset);
	DebugMessage("登录选小区			g_SelectedSmallAreaCAllOffset = 0x%08X",g_SelectedSmallAreaCAllOffset);
	DebugMessage("登录选大区			g_SelectLargeBigAreaCAllOffset = 0x%08X",g_SelectLargeBigAreaCAllOffset);
	DebugMessage("登录执行				g_LoginCallOffset = 0x%08X",g_LoginCallOffset);
	DebugMessage("登录选区服对象		g_SimFlashobjOffset = 0x%08X",g_SimFlashobjOffset);
	DebugMessage("登录输入账号对象		g_AccountObjOffset = 0x%08X",g_AccountObjOffset);
	DebugMessage("登录转换密码表		g_PasswordObjOffset = 0x%08X",g_PasswordObjOffset);
	DebugMessage("登录设置密码表		g_PasswordTableObjOffset = 0x%08X",g_PasswordTableObjOffset);
	DebugMessage("登录对象偏移			g_loginobjOffset = 0x%08X",g_loginobjOffset);
	DebugMessage("设置密码表CALL		g_SetPasswordTableCALl = 0x%08X",g_SetPasswordTableCALl);
	DebugMessage("登录转换密码表CALL	g_MyChangeDateCALlOffset = 0x%08X",g_MyChangeDateCALlOffset);
	DebugMessage("创建角色				g_callSetHwndText =  0x%08X",g_callSetHwndText);
	DebugMessage("游戏消息框内容        g_GameMessageBoxConten = 0x%08X",g_GameMessageBoxConten);
	DWORD ItemsAddresOffset = 0x15fc;
	DWORD pOffset[40] = {0};
	
	pOffset[index_offsetMapBase] = g_offsetMapBase;
	pOffset[index_OffsetGetObjHere] = g_OffsetGetObjHere;
	pOffset[index_OffsetGetObjPlayerSet] = g_OffsetGetObjPlayerSet;
	pOffset[index_ItemsAddresOffset] = ItemsAddresOffset;
	pOffset[index_offsetEyeX] = g_offsetEyeX;
	pOffset[index_offsetEyeY] = g_offsetEyeY;
	pOffset[index_PackageOffset] = g_PackageOffset;
	pOffset[index_PetEnageOffset] = g_PetEnageOffset;
	pOffset[index_szAccountOffset] = g_szAccountOffset;
	pOffset[index_cd1Offset] = g_cd1Offset;
	pOffset[index_cd2Offset] = g_cd2Offset;
	pOffset[index_RoleState] = g_RoleState;
	pOffset[index_OffsetObjNetwork] = g_OffsetObjNetwork;
	pOffset[index_BooththisOffset] = g_CBooththisOffset;
	pOffset[index_CreateHero3Offset] = g_CreateHero3Offset;
	pOffset[index_CreateHeroObjOffset] = g_CreateHeroObjOffset;
	pOffset[index_CreateHeroObj2Offset] = g_CreateHeroObj2Offset;
	pOffset[index_CallCreateHeroOffset] = g_CallCreateHeroOffset;
	pOffset[index_SelectedVersionCAll] = g_SelectedVersionCAllOffset;
	pOffset[index_SelectedSmallAreaCAll] = g_SelectedSmallAreaCAllOffset;
	pOffset[index_SelectLargeBigAreaCAll] = g_SelectLargeBigAreaCAllOffset;
	pOffset[index_LoginCall] = g_LoginCallOffset;
	pOffset[index_SimFlashobj] = g_SimFlashobjOffset;
	pOffset[index_AccountObj] = g_AccountObjOffset;
	pOffset[index_PasswordObj] = g_PasswordObjOffset;
	pOffset[index_PasswordTableObj] = g_PasswordTableObjOffset;
	pOffset[index_loginobjOffset] = g_loginobjOffset;
	pOffset[index_SetPasswordTableCALl] = g_SetPasswordTableCALl;
	pOffset[index_MyChangeDateCALlOffset] = g_MyChangeDateCALlOffset;
	pOffset[index_GuoLvShangYe] = g_GuoLvShangYe;
	pOffset[index_GuoLvLoginMessageBox] = G_GuoLvLoginMessageBox;
	pOffset[index_CloseCurWndFunOffset] = g_CloseCurWndFunOffset;
	pOffset[index_CloseCurWndOffset] = g_CloseCurWndOffset;
	pOffset[index_SpeedUpOffset] = g_SpeedUpOffset;
	pOffset[index_OffsetDingWeiLiaoTian] = g_OffsetDingWeiLiaoTian;
	pOffset[index_OffsetPanDuanFengHao] = g_offsetPanDuanFengHao;
	pOffset[index_callSetHwndText] = g_callSetHwndText;
	pOffset[index_offsetGameWndMessageContent] = g_GameMessageBoxConten;
	pOffset[index_end] = 0;
	PostOffsetsToServer(pOffset,_countof(pOffset));
}

DWORD WINAPI MyRemoteThread(LPVOID lpParam){
	DebugMessage("Enter remote thread.");
	RunGetOffset();
	return 1;
}