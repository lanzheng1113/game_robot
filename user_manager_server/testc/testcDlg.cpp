
// testcDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CtestcDlg �Ի���




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


// CtestcDlg ��Ϣ�������

BOOL CtestcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int GenCode();
extern BYTE szSystemInfo[4096]; // �ڳ���ִ����Ϻ󣬴˴��洢ȡ�õ�ϵͳ������
extern UINT uSystemInfoLen; // �ڳ���ִ����Ϻ󣬴˴��洢ȡ�õ�ϵͳ������ĳ���
void CtestcDlg::OnBnClickedButton1()
{
	yz();
}

DWORD CtestcDlg::yz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	GetPrivateProfileString("ID","�˺�",NULL,szId,sizeof(szId),appDir.c_str());

	DebugMessage("%s",szId);
	SimpleXor_Crype((char*)ps1.id,(char*)szId,sizeof(ps1.id));
	//memcpy(ps1.id,szId,sizeof(ps1.id));
	//��GenCode��ʱ��Ҫ��szSystemInfo���м��ܡ�
	memcpy(ps1.sqm_netcard_diskid,szSystemInfo,sizeof(ps1.sqm_netcard_diskid));
	char sendbuf[2048] = {0};
	//��֯���ݷ��͡�
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
					::MessageBox(NULL,"�����ڵ��˺š�",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				case 2:
					::MessageBox(NULL,"�˺��Ѿ����ڡ�",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				case 3:
					::MessageBox(NULL,"��������æ��",NULL,MB_OK);
					closesocket(ConnectSocket);
					ExitProcess(0);
					break;
				default:
					::MessageBox(NULL,"δ֪�Ĵ���404��",NULL,MB_OK);
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
				::MessageBox(NULL,"δ֪�Ĵ���401",NULL,MB_OK);
				closesocket(ConnectSocket);
				ExitProcess(0);
			}
		}else if ( iRecvCount == 0 ){
			printf("Connection closed\n");
			ExitProcess(0);
		}else{
			::MessageBox(NULL,"δ֪�Ĵ���402",NULL,MB_OK);
			closesocket(ConnectSocket);
			ExitProcess(0);
		}
	}
	closesocket(ConnectSocket);
	return 0;
}