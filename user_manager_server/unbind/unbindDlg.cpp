
// unbindDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "unbind.h"
#include "unbindDlg.h"
#include "winsock2.h"
#include "DebugMessage.h"
#include "../common/NVDSNetHeader.h"
#include "../common/NetBase.h"
#include "util/StringEx.h"
#include "util/Path.h"
#include "util/File.h"
#include "util/Crc32.h"
#include "../common/MyUserMgr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET g_sComm;
// CunbindDlg 对话框

DWORD g_lastcomm = 0;


CunbindDlg::CunbindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CunbindDlg::IDD, pParent)
	, m_strCardID(_T(""))
	, m_strPswd(_T(""))
	, m_strAcc(_T(""))
	, m_bIsRemeberPswd(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CunbindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strCardID);
	DDX_Control(pDX, IDC_STATIC_Lable, m_static);
	DDX_Control(pDX, IDC_EDIT3, m_editPswd);
	DDX_Control(pDX, IDC_BUTTON_login, m_buttonLogin);
	DDX_Text(pDX, IDC_EDIT3, m_strPswd);
	DDX_Text(pDX, IDC_EDIT_ACC, m_strAcc);
	DDV_MaxChars(pDX, m_strAcc, 16);
}

BEGIN_MESSAGE_MAP(CunbindDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_UNBIND, &CunbindDlg::OnBnClickedButtonUnbind)
	ON_BN_CLICKED(IDC_BUTTON_login, &CunbindDlg::OnBnClickedButtonlogin)
	ON_BN_CLICKED(IDC_CHECK_REMEBERPSWD, &CunbindDlg::OnBnClickedCheckRemeberpswd)
END_MESSAGE_MAP()


// CunbindDlg 消息处理程序

BOOL CunbindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//WritePrivateProfileString("Option","remberpswd","1",path.c_str());
	std::string path = Path::getApplicationDirPath();
	path += "option.ini";
	int b = GetPrivateProfileInt("Option","remberpswd",0,path.c_str());
	if (b != 0)
	{
		((CButton*)GetDlgItem(IDC_CHECK_REMEBERPSWD))->SetCheck(BST_CHECKED);
		m_bIsRemeberPswd = TRUE;
	}else
	{
		m_bIsRemeberPswd = FALSE;
	}
	
	char acc[16] = {0};
	GetPrivateProfileString("Option","acc","",acc,sizeof(acc),path.c_str());
	m_strAcc = acc;
	CEdit*  pEdit=(CEdit*)GetDlgItem(IDC_EDIT_ACC);//获取相应的编辑框ID
	pEdit->SetWindowText(m_strAcc); //设置默认显示的内容 
	if (m_bIsRemeberPswd)
	{
		char pass[16] = {0};
		GetPrivateProfileString("Option","pswd","",pass,sizeof(pass),path.c_str());
		m_strPswd = pass;
		CEdit*  pEditPswd=(CEdit*)GetDlgItem(IDC_EDIT_PSWD);//获取相应的编辑框ID
		pEditPswd->SetWindowText(m_strPswd); //设置默认显示的内容 
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CunbindDlg::OnPaint()
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
HCURSOR CunbindDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CunbindDlg::OnBnClickedButtonUnbind()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strCardID == "")
	{
		MessageBox("需要解绑的ID不能为空！");
		return;
	}
	int iresult = unbindNoCondition(m_strCardID);
	if(iresult == 0)
		MessageBox("解绑成功！");
	else
	{
		if (iresult == 1){
			MessageBox("解绑失败！没有找到该用户");
		}else if (iresult == 2){
			MessageBox("解绑失败！该用户已经是解绑状态了。");
		}else if (iresult == 3){
			MessageBox("解绑失败！该用户剩余时间不足以解绑。");
		}else if (iresult == 4){
			MessageBox("解绑失败！不允许在48小时内连续解绑这个账号。");
		}
	}
}

BOOL CunbindDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

UINT UpdateThread( LPVOID thdParam ){
	while (1)
	{
		if (GetTickCount() - g_lastcomm > 25000 && g_sComm)
		{
			char sendbuf[8] = {0};
			*(USHORT*)sendbuf = 6;
			*(DWORD*)(sendbuf+2) = AffairType_Dovalidate;
			*(WORD*)(sendbuf+6) = heart;
			send(g_sComm,sendbuf,8,0);
			char recvbuf[1024] = {0};
			int recvc = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
			if (recvc == 0 || recvc == -1){
				AfxMessageBox("与服务器的链接断开了。");
			}
			g_lastcomm = GetTickCount();
			DebugMessage("send heart!");
		}
	}
	return 1;
}

void CunbindDlg::OnBnClickedButtonlogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	std::string path = Path::getApplicationDirPath();
	path += "option.ini";
// 	char acc[16] = {0};
// 	GetPrivateProfileString("Option","acc","",acc,sizeof(acc),path.c_str());
// 	m_strAcc = acc;
// 	
// 	if (m_bIsRemeberPswd)
// 	{
// 		char pass[16] = {0};
// 		GetPrivateProfileString("Option","pswd","",pass,sizeof(pass),path.c_str());
// 		m_strPswd = pass;
// 	}

	WritePrivateProfileString("Option","acc",(LPCTSTR)m_strAcc,path.c_str());
	if (m_bIsRemeberPswd){
		WritePrivateProfileString("Option","pswd",(LPCTSTR)m_strPswd,path.c_str());
	}

	if (g_sComm)
	{
		closesocket(g_sComm);
		g_sComm = 0;
	}

	static bool firsttimerun = true;
	if (firsttimerun){
		firsttimerun = false;
		WSADATA wsaData;
		//----------------------
		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != NO_ERROR) {
			DebugMessage("[GameDll]WSAStartup failed with error: %d\n", iResult);
			MessageBox("WSAStartup failed with error");
			return;
		}
	}


	//----------------------
	// Create a SOCKET for connecting to server
	g_sComm = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sComm == INVALID_SOCKET) {
		DebugMessage("[Dll][nt]socket failed with error: %ld\n", WSAGetLastError());
		MessageBox("socket failed with error");
		return;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	struct sockaddr_in clientService; 
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( /*"127.0.0.1"*/"119.252.245.74" );
#ifdef COMPILE_9XAUTO
//	MessageBox("1");
	clientService.sin_port = htons( THE_9X_PORT );
#endif
#ifdef COMPILE_9XNotAUTO
	clientService.sin_port = htons( 6656 );
#endif
#ifdef COMPILE_5X
	clientService.sin_port = htons( 41026 );
#endif
	//----------------------
	// Connect to server.
	int iResult = connect( g_sComm, (SOCKADDR*) &clientService, sizeof(clientService) );
	if (iResult == SOCKET_ERROR) {
		DebugMessage( "[Dll][nt]connect failed with error: %d\n", WSAGetLastError() );
		MessageBox("连接服务器失败了！");
		closesocket(g_sComm);
		return;
	}

	PacketProxyPswdCheck papc;
	papc.pgh.CheckSum = 0;
	papc.pgh.GameType = 0;
	papc.pgh.GameVersion = 1;
	papc.pgh.HeadFlag = AffairType_Dovalidate;
	papc.pgh.HeadUserID = 0;
	papc.pgh.MesgFullType = MsgAdminUnbindUsrLogin;
	papc.pgh.PacketLen = sizeof(papc);
	papc.pgh.WgVersion = 1;

	char pswd[16] = {0};
	char acc[16] = {0};
	strncpy(acc,(LPCTSTR)m_strAcc,16);
	strncpy(pswd,(LPCTSTR)m_strPswd,16);
	acc[15] = 0;
	pswd[15] = 0;
	strncpy((char*)papc.acc,acc,16);
	strncpy((char*)papc.pswd,pswd,16);

	char sendbuf[4096] = {0};
	*(WORD*)sendbuf = sizeof(papc);
	memcpy(sendbuf+2,&papc,sizeof(papc));
	int isend = send(g_sComm,sendbuf,sizeof(papc)+2,0);
	if (isend == 0 || isend == -1){
		MessageBox("发送数据包错误1。");
		return;
	}
	g_lastcomm = GetTickCount();

	bool successed_verify = false;
	char recvbuf[5096] = {0};

	char recbufTemp[4096] = {0};
	int irecved = 0;
	while (1)
	{
		int nowRecved = recv(g_sComm,recbufTemp,sizeof(recbufTemp),0);
		if (nowRecved == 0){
			MessageBox("连接被断开");
			return;
		}else if (nowRecved == -1){
			MessageBox("连接断开1");
			return;
		}else{
			DebugMessage("客户端:Recved %d bytes.",nowRecved);
			memcpy(recvbuf+irecved,recbufTemp,nowRecved);
			irecved += nowRecved;
			if (irecved == *(WORD*)recvbuf+2){
				break;
			}
		}
	}

	// 	int irecved = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
	// 	if (irecved == 0)
	// 	{
	// 		MessageBox("连接被断开。");
	// 		return;
	// 	}else if (irecved == -1)
	// 	{
	// 		MessageBox("连接断开1。");
	// 		return;
	// 	}else
	{
		if (irecved == sizeof(ACKPACKETHEAD)+2)
		{
			ACKPACKETHEAD* pa = (ACKPACKETHEAD*)(recvbuf+2);
			if (pa->AckMesgFullType != AckMsgAdminCheckOk)
			{
				MessageBox("错误的消息！");
				return;
			}else{
				SetWindowText("解绑客户端 - 已授权。");
				successed_verify = true;
				GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_UNBIND)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_Lable)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_login)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_6)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_ACC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_REMEBERPSWD)->ShowWindow(SW_HIDE);
				AfxBeginThread(UpdateThread,NULL);
			}

		}else{
			MessageBox("错误的消息2！");
			return;
		}
	}
}

int CunbindDlg::unbindNoCondition( const CString& strId )
{
	std::string kardId((LPCTSTR)strId);
	char sendbuf[1024] = {0};
	PacketUnbind pu;
	pu.pgh.CheckSum = 0;
	pu.pgh.GameType = 1;
	pu.pgh.GameVersion = 1;
	pu.pgh.HeadFlag = AffairType_Dovalidate;
	pu.pgh.HeadUserID = 1;
	pu.pgh.MesgFullType = MsgProxyUnbind;
	pu.isSub3Days = 1;
	strncpy(pu.AuthId,kardId.c_str(),sizeof(pu.AuthId));
	pu.AuthId[9] = 0;
	*(WORD*)sendbuf = sizeof(pu);
	memcpy(sendbuf+2,&pu,sizeof(pu));
	int isend = send(g_sComm,sendbuf,2+sizeof(pu),0);
	g_lastcomm = GetTickCount();
	if (isend != 2+sizeof(pu)){
		MessageBox("发送数据包失败了。");
		return 1;
	}else{
		char recvbuf[5096] = {0};
		char recbufTemp[4096] = {0};
		int irecved = 0;
		while (1)
		{
			int nowRecved = recv(g_sComm,recbufTemp,sizeof(recbufTemp),0);
			if (nowRecved == 0){
				MessageBox("与服务器的连接被断开：0");
				return -2;
			}else if (nowRecved == -1){
				MessageBox("与服务器的连接被断开：-1");
				return -1;
			}else{
				DebugMessage("客户端:Recved %d bytes.",nowRecved);
				memcpy(recvbuf+irecved,recbufTemp,nowRecved);
				irecved += nowRecved;
				if (irecved == *(WORD*)recvbuf+2){
					break;
				}
			}
		}

// 		char recvbuf[1024] = {0};
// 		int irecv = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
// 		if (irecv == 0 || irecv == -1){
// 			MessageBox("接收数据包失败了。");
// 			return 1;
// 		}
		if (irecved != sizeof(ACKPACKETHEAD)+2)
		{
			DebugMessage("need count = %d,recved count = %d",sizeof(ACKPACKETHEAD)+2,irecved);
			MessageBox("接收数据包失败了，大小不对。",NULL,MB_OK);
			return 1;
		}
		ACKPACKETHEAD* pAck = (ACKPACKETHEAD*)(recvbuf+2);
		if (pAck->AckMesgFullType == AckMsgUnbindSuccess){
			return 0;
		}else{
			return pAck->WgVersion;
		}
	}
}

void CunbindDlg::OnBnClickedCheckRemeberpswd()
{
	// TODO: 在此添加控件通知处理程序代码
	std::string path = Path::getApplicationDirPath();
	path += "option.ini";

	if(((CButton*)GetDlgItem(IDC_CHECK_REMEBERPSWD))->GetCheck() == BST_CHECKED){
		m_bIsRemeberPswd = TRUE;
		WritePrivateProfileString("Option","remberpswd","1",path.c_str());
		DebugMessage("开启自动模式。");
	}else{
		m_bIsRemeberPswd = FALSE;
		WritePrivateProfileString("Option","remberpswd","0",path.c_str());
		WritePrivateProfileString("Option","pswd","",path.c_str());
		DebugMessage("关闭自动模式。");
	}
}
