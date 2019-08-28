
// testcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testc.h"
#include "testcDlg.h"
#include "../common/NetBase.h"
#include "util/Path.h"
#include "../common/crypt.h"
#include "../common/NVDSNetHeader.h"
#include "DebugMessage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtestcDlg 对话框




CtestcDlg::CtestcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CtestcDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CtestcDlg 消息处理程序

BOOL CtestcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int GenCode();
extern BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
extern UINT uSystemInfoLen; // 在程序执行完毕后，此处存储取得的系统特征码的长度
void CtestcDlg::OnBnClickedButton1()
{
	yz();
}

DWORD CtestcDlg::yz()
{
	// TODO: 在此添加控件通知处理程序代码
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
	clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	clientService.sin_port = htons( 4230 );

	//----------------------
	// Connect to server.
	iResult = connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) );
	if (iResult == SOCKET_ERROR) {
		printf( "connect failed with error: %d\n", WSAGetLastError() );
		closesocket(ConnectSocket);
		return 1;
	}

	GenCode();
	int i = 0;
	PACKETGENERALHEAD ppg;
	ppg.CheckSum = 0;
	ppg.GameType = 1;
	ppg.GameVersion = 1;
	ppg.HeadFlag = AffairType_Dovalidate;
	ppg.HeadUserID = 0;
	ppg.MesgFullType = 0;
	ppg.PacketLen = sizeof(ppg);
	ppg.WgVersion = 0;

	PacketS1 ps1;
	ps1.pgh = ppg;
	std::string appDir = Path::getApplicationDirPath();
	appDir += "id.ini";

	char szId[256] = {0};
	GetPrivateProfileString("ID","账号",NULL,szId,sizeof(szId),appDir.c_str());

	DebugMessage("%s",szId);
	SimpleXor_Crype((char*)ps1.id,(char*)szId,sizeof(ps1.id));
	//memcpy(ps1.id,szId,sizeof(ps1.id));
	//在GenCode的时候要对szSystemInfo进行加密。
	memcpy(ps1.sqm_netcard_diskid,szSystemInfo,sizeof(ps1.sqm_netcard_diskid));
	char sendbuf[2048] = {0};
	//组织数据发送。
	*(WORD*)sendbuf = sizeof(ps1);memcpy(sendbuf+2,&ps1,sizeof(ps1));
	iResult = send(ConnectSocket,sendbuf,sizeof(ps1)+2,0);
	if (iResult == SOCKET_ERROR) {
		printf("send() failed with error: %d\n", WSAGetLastError());
	}else{
		//printf("Bytes Sent: %d\n", iResult);
		int iRecvCount = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iRecvCount > 0 ){
			//printf("Bytes received: %d\n", iRecvCount);
			if (iRecvCount == sizeof(PacketS1RetError)){
				PacketS1RetError* p = (PacketS1RetError*)recvbuf;
				switch (p->ErrorCode){
				case 1:
					::MessageBox(NULL,"不存在的账号。",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				case 2:
					::MessageBox(NULL,"账号已经过期。",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				case 3:
					::MessageBox(NULL,"服务器繁忙。",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				default:
					::MessageBox(NULL,"未知的错误404。",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				}
			}else if(iRecvCount == sizeof(PacketS1RetOffset)+2){
				HANDLE hMapFile;
				PVOID pBuf;
				char szName[] = "p6cool";
				hMapFile = CreateFileMapping(
					INVALID_HANDLE_VALUE,    // use paging file
					NULL,                    // default security 
					PAGE_READWRITE,          // read/write access
					0,                       // max. object size 
					40,						 // buffer size  
					szName);                 // name of mapping object
				if (hMapFile == NULL) { 
					printf("Could not create file mapping object (%d).\n", GetLastError());
					return 1;
				}
				pBuf =  MapViewOfFile(hMapFile,   // handle to map object
												FILE_MAP_ALL_ACCESS, // read/write permission
												0,                   
												0,                   
												40);

				if (pBuf == NULL) { 
					printf("Could not map view of file (%d).\n", GetLastError()); 
					return 2;
				}
				
				PacketS1RetOffset* p = (PacketS1RetOffset*)(recvbuf+2);
				memcpy(pBuf,p->Offset,40);
				for (int i=0; i!=10; i++)
				{
					DebugMessage("o 0x%X",p->Offset[i]);
				}
				UnmapViewOfFile(pBuf);
			}else{
				::MessageBox(NULL,"未知的错误401",NULL,MB_OK);
				closesocket(ConnectSocket);
				ExitProcess(0);
			}
		}else if ( iRecvCount == 0 ){
			printf("Connection closed\n");
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