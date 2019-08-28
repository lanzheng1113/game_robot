
// unbindDlg.cpp : ʵ���ļ�
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
// CunbindDlg �Ի���

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


// CunbindDlg ��Ϣ�������

BOOL CunbindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	CEdit*  pEdit=(CEdit*)GetDlgItem(IDC_EDIT_ACC);//��ȡ��Ӧ�ı༭��ID
	pEdit->SetWindowText(m_strAcc); //����Ĭ����ʾ������ 
	if (m_bIsRemeberPswd)
	{
		char pass[16] = {0};
		GetPrivateProfileString("Option","pswd","",pass,sizeof(pass),path.c_str());
		m_strPswd = pass;
		CEdit*  pEditPswd=(CEdit*)GetDlgItem(IDC_EDIT_PSWD);//��ȡ��Ӧ�ı༭��ID
		pEditPswd->SetWindowText(m_strPswd); //����Ĭ����ʾ������ 
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CunbindDlg::OnPaint()
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
HCURSOR CunbindDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CunbindDlg::OnBnClickedButtonUnbind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strCardID == "")
	{
		MessageBox("��Ҫ����ID����Ϊ�գ�");
		return;
	}
	int iresult = unbindNoCondition(m_strCardID);
	if(iresult == 0)
		MessageBox("���ɹ���");
	else
	{
		if (iresult == 1){
			MessageBox("���ʧ�ܣ�û���ҵ����û�");
		}else if (iresult == 2){
			MessageBox("���ʧ�ܣ����û��Ѿ��ǽ��״̬�ˡ�");
		}else if (iresult == 3){
			MessageBox("���ʧ�ܣ����û�ʣ��ʱ�䲻���Խ��");
		}else if (iresult == 4){
			MessageBox("���ʧ�ܣ���������48Сʱ�������������˺š�");
		}
	}
}

BOOL CunbindDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
				AfxMessageBox("������������ӶϿ��ˡ�");
			}
			g_lastcomm = GetTickCount();
			DebugMessage("send heart!");
		}
	}
	return 1;
}

void CunbindDlg::OnBnClickedButtonlogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("���ӷ�����ʧ���ˣ�");
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
		MessageBox("�������ݰ�����1��");
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
			MessageBox("���ӱ��Ͽ�");
			return;
		}else if (nowRecved == -1){
			MessageBox("���ӶϿ�1");
			return;
		}else{
			DebugMessage("�ͻ���:Recved %d bytes.",nowRecved);
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
	// 		MessageBox("���ӱ��Ͽ���");
	// 		return;
	// 	}else if (irecved == -1)
	// 	{
	// 		MessageBox("���ӶϿ�1��");
	// 		return;
	// 	}else
	{
		if (irecved == sizeof(ACKPACKETHEAD)+2)
		{
			ACKPACKETHEAD* pa = (ACKPACKETHEAD*)(recvbuf+2);
			if (pa->AckMesgFullType != AckMsgAdminCheckOk)
			{
				MessageBox("�������Ϣ��");
				return;
			}else{
				SetWindowText("���ͻ��� - ����Ȩ��");
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
			MessageBox("�������Ϣ2��");
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
		MessageBox("�������ݰ�ʧ���ˡ�");
		return 1;
	}else{
		char recvbuf[5096] = {0};
		char recbufTemp[4096] = {0};
		int irecved = 0;
		while (1)
		{
			int nowRecved = recv(g_sComm,recbufTemp,sizeof(recbufTemp),0);
			if (nowRecved == 0){
				MessageBox("������������ӱ��Ͽ���0");
				return -2;
			}else if (nowRecved == -1){
				MessageBox("������������ӱ��Ͽ���-1");
				return -1;
			}else{
				DebugMessage("�ͻ���:Recved %d bytes.",nowRecved);
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
// 			MessageBox("�������ݰ�ʧ���ˡ�");
// 			return 1;
// 		}
		if (irecved != sizeof(ACKPACKETHEAD)+2)
		{
			DebugMessage("need count = %d,recved count = %d",sizeof(ACKPACKETHEAD)+2,irecved);
			MessageBox("�������ݰ�ʧ���ˣ���С���ԡ�",NULL,MB_OK);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::string path = Path::getApplicationDirPath();
	path += "option.ini";

	if(((CButton*)GetDlgItem(IDC_CHECK_REMEBERPSWD))->GetCheck() == BST_CHECKED){
		m_bIsRemeberPswd = TRUE;
		WritePrivateProfileString("Option","remberpswd","1",path.c_str());
		DebugMessage("�����Զ�ģʽ��");
	}else{
		m_bIsRemeberPswd = FALSE;
		WritePrivateProfileString("Option","remberpswd","0",path.c_str());
		WritePrivateProfileString("Option","pswd","",path.c_str());
		DebugMessage("�ر��Զ�ģʽ��");
	}
}
