// TestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "../common/NetBase.h"
#include "winsock2.h"

#pragma comment(lib,"WS2_32.LIB")
#pragma comment(lib,"Advapi32.lib")
#include "CpuUsage.h"
#include "../common/shareStruct.h"
#include "../common/TradeMesgDef.h"
#include "StringEx.h"
#include "CSend.h"
#include "Logs.h"
#include <iostream>
using namespace std;

#include "NetworkRecver.h"

DWORD WINAPI timer_thread(LPVOID lpvoid);

LONG g_iThdreadCount = 0;
double g_fCpuUsage = 0.0;

int _tmain(int argc, _TCHAR* argv[])
{
	Logs::getInstance().setLogFileName("test client game client log file.txt");
	C_log("Main START.");

	CTradeClientNetWork *nt = new CTradeClientNetWork();
	if(!nt->Go())
		return 1;
	
	printf("test 1\n");
	int i = 0;
	HANDLE hdThd=CreateThread(NULL,0,timer_thread,&i,0,NULL);  
	CloseHandle(hdThd);
	
	while (1){
		if (nt->getSessionCount() == 0){
			if (g_fCpuUsage < 60.0){
				string str = "##########post request to acquire a order.##########";
				C_log(str);
				nt->PM_INeedAOrder((int)(g_fCpuUsage));
			}else{
				printf("==wait cause g_fCpuUsage*100 > 60.0,g_fCpuUsage=%f\n",g_fCpuUsage);
			}
		}else{
			printf("==wait cause nt->getSessionCount()>0\n");
		}
		Sleep(2000);
	}

	char x;
	std::cin >> x;
}

DWORD WINAPI timer_thread(LPVOID lpvoid){
	// Start the message loop. 
	MSG msg;
	SetTimer(NULL,111,1000,NULL);
	while( GetMessage( &msg, NULL, 0, 0 ))
	{ 
		if(msg.message == WM_TIMER)
		{
			g_fCpuUsage = GetCpuUsage();
			printf("CPU Usage:%3.2f%%\n",g_fCpuUsage);
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} 
	return 0;
}