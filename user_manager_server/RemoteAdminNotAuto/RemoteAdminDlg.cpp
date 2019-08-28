
// RemoteAdminDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteAdmin.h"
#include "RemoteAdminDlg.h"
#include "winsock2.h"
#include "DebugMessage.h"
#include "../common/NVDSNetHeader.h"
#include "../common/NetBase.h"
#include "util/StringEx.h"
#include "util/Path.h"
#include "util/File.h"
#include "util/Crc32.h"
#include "../common/MyUserMgr.h"
#include "DlgGenNewCard.h"
MyUserMgr g_mum;

#pragma comment(lib,"ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
SOCKET g_sComm;

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


// CRemoteAdminDlg 对话框




CRemoteAdminDlg::CRemoteAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteAdminDlg::IDD, pParent)
	, m_strPswd(_T(""))
	, m_UnbindID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USERS, m_ListCtrlUsers);
	DDX_Text(pDX, IDC_EDIT_PSWD, m_strPswd);
	DDV_MaxChars(pDX, m_strPswd, 8);
	DDX_Control(pDX, IDC_COMBO_USERSELECT, m_ComboName);
	DDX_Text(pDX, IDC_EDIT_UNBINDID, m_UnbindID);
}

BEGIN_MESSAGE_MAP(CRemoteAdminDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CRemoteAdminDlg::OnBnClickedButtonLogin)
	ON_CBN_SELCHANGE(IDC_COMBO_USERSELECT, &CRemoteAdminDlg::OnCbnSelchangeComboUserselect)
	ON_BN_CLICKED(IDC_BUTTONNewCard, &CRemoteAdminDlg::OnBnClickedButtonnewcard)
	ON_MESSAGE(DATA_UPDATED,&CRemoteAdminDlg::OnDataUpdated)
	ON_BN_CLICKED(IDC_BUTTON4, &CRemoteAdminDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CRemoteAdminDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CRemoteAdminDlg 消息处理程序

BOOL CRemoteAdminDlg::OnInitDialog()
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

	setlocale( LC_ALL, "chs" );

	m_ListCtrlUsers.InsertColumn(0,"计数",0,40);
	m_ListCtrlUsers.InsertColumn(1,"授权码",0,80);
	m_ListCtrlUsers.InsertColumn(2,"卡类型",0,50);
	m_ListCtrlUsers.InsertColumn(3,"硬件ID",0,120);
	m_ListCtrlUsers.InsertColumn(4,"到期时间",0,100);
	LONG lStyle = m_ListCtrlUsers.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_ListCtrlUsers.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText("半自动九星开卡管理");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRemoteAdminDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRemoteAdminDlg::OnPaint()
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
HCURSOR CRemoteAdminDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


struct abc 
{
	char name[32];
	unsigned int crc32s;
};

int openfile_calcCRC32(const std::string& name){
	String s(name);
	String s2 = s.toUpperCase();
	if (!s2.endsWith(".TXT")){
		return -1;
	}

	FILE* fp = fopen(name.c_str(),"r");
	if (!fp){
		DebugMessage("打开文件失败了。");
		return -1;
	}else{
		fseek(fp,0,SEEK_END);
		int len = ftell(fp);
		char* _content = new char[len];
		memset(_content,0,len);

		fseek(fp,0,SEEK_SET);
		int read_actully = fread(_content,1,len,fp);
		fclose(fp);

		int crc = Crc32::cal_crc((unsigned char*)_content,read_actully);
		delete [] _content;
		return crc;
	}
}

void CRemoteAdminDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
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
	clientService.sin_addr.s_addr = inet_addr( "59.125.161.132" );
	clientService.sin_port = htons( 6656 );

	//----------------------
	// Connect to server.
	int iResult = connect( g_sComm, (SOCKADDR*) &clientService, sizeof(clientService) );
	if (iResult == SOCKET_ERROR) {
		DebugMessage( "[Dll][nt]connect failed with error: %d\n", WSAGetLastError() );
		MessageBox("连接服务器失败了！");
		closesocket(g_sComm);
		return;
	}

	PacketAdminPswdCheck papc;
	papc.pgh.CheckSum = 0;
	papc.pgh.GameType = 0;
	papc.pgh.GameVersion = 1;
	papc.pgh.HeadFlag = AffairType_Dovalidate;
	papc.pgh.HeadUserID = 0;
	papc.pgh.MesgFullType = MsgAdminCheckPswd;
	papc.pgh.PacketLen = sizeof(papc);
	papc.pgh.WgVersion = 1;

	char pswd[8] = {0};
	strncpy(pswd,(LPCTSTR)m_strPswd,8);
	pswd[7] = 0;
	strncpy((char*)papc.pswd,pswd,8);

	char sendbuf[4096] = {0};
	*(WORD*)sendbuf = sizeof(papc);
	memcpy(sendbuf+2,&papc,sizeof(papc));
	int isend = send(g_sComm,sendbuf,sizeof(papc)+2,0);
	if (isend == 0 || isend == -1){
		MessageBox("发送数据包错误1。");
		return;
	}
	
	bool successed_verify = false;
	char recvbuf[5096] = {0};
	int irecved = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
	if (irecved == 0)
	{
		MessageBox("连接被断开。");
		return;
	}else if (irecved == -1)
	{
		MessageBox("连接断开1。");
		return;
	}else{
		if (irecved == sizeof(ACKPACKETHEAD)+2)
		{
			ACKPACKETHEAD* pa = (ACKPACKETHEAD*)(recvbuf+2);
			if (pa->AckMesgFullType != AckMsgAdminCheckOk)
			{
				MessageBox("错误的消息！");
				return;
			}else{
				SetWindowText("远程客户端 - 成功连接服务器。");
				successed_verify = true;
			}

		}else{
			MessageBox("错误的消息2！");
			return;
		}
	}

	if (successed_verify)
	{
		//首先清零原有的内容
		std::vector<abc> l_bbc;

		//便利目录下的文件，重新生成一份。
		std::string str = Path::getApplicationDirPath();
		str += "CEnables\\";
		File::createPath(str);
		StringList sl = File(str).getFileListRecursive();
		int iLoad = 0;
		if (!sl.empty()){
			for (StringList::const_iterator it = sl.begin(); it!=sl.end(); ++it){
				int iCrc = openfile_calcCRC32(*it);
				bool load_success = false;
				if (iCrc == -1){
					continue;
				}else{
					File f(*it);
					std::string name = f.getFileName();
					abc bxc;
					strncpy(bxc.name,name.c_str(),sizeof(bxc.name));
					bxc.name[31] = 0;
					bxc.crc32s = iCrc;
					DebugMessage("Client name = %s,CRC=0x%08X",name.c_str(),bxc.crc32s);
					l_bbc.push_back(bxc);
				}
			}

		}

		PACKETGENERALHEAD pgh;
		pgh.CheckSum = 0;
		pgh.GameType = 0;
		pgh.GameVersion = 1;
		pgh.HeadFlag = AffairType_Dovalidate;
		pgh.HeadUserID = 0;
		pgh.MesgFullType = MsgNeedUserInfo;
		pgh.PacketLen = sizeof(pgh);
		pgh.WgVersion = 1;

		*(WORD*)sendbuf = sizeof(pgh)+l_bbc.size()*36;
		memcpy(sendbuf+2,&pgh,sizeof(pgh));

		for (int i=0; i!=l_bbc.size(); i++){
			DebugMessage("client file name = %s,icrc = 0x%X",l_bbc[i].name,l_bbc[i].crc32s);
			memcpy(sendbuf+2+sizeof(PACKETGENERALHEAD)+i*36,l_bbc[i].name,32);
			*(DWORD*)(sendbuf+2+sizeof(PACKETGENERALHEAD)+i*36+32) = l_bbc[i].crc32s;
		}

		isend = send(g_sComm,sendbuf,sizeof(pgh)+l_bbc.size()*36+2,0);
		if (isend == 0 || isend == -1)
		{
			MessageBox("发送数据错误3");
			return;
		}

		irecved = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
		DebugMessage("Receive bytes %d,total bytes %d",irecved,(*(WORD*)recvbuf));
		if (irecved == 0){
			MessageBox("连接断开3");
			return;
		}else if (irecved == -1){
			MessageBox("连接断开4");
			return;
		}else{
			bool isFirstDataPacket = true;
			bool moreData = true;
			char* pUserData = NULL;
			int dataRecvedLen = 0;
			while (moreData)
			{
				if (irecved < sizeof(ACKPACKETHEAD)+2)
				{
					MessageBox("数据错误31");
					return;
				}else{
					ACKPACKETHEAD* aph = (ACKPACKETHEAD*)(recvbuf+2);
					if (isFirstDataPacket && (aph->AckMesgFullType == AckMsgAdminNoMoreUserData))
					{
						isFirstDataPacket = false;
						MessageBox("服务器没有数据可以提供。");
						return;
					}

					if (AckMsgAdminAllUserFileSame == aph->AckMesgFullType){
						PostMessage(DATA_UPDATED);
						MessageBox("成功同步服务器与客户端的账号文件");
						return;
					}

					if (AckMsgAdminOpenFileFailed == aph->AckMesgFullType)
					{
						MessageBox("服务器繁忙，账号文件正被占用，请稍后再试。");
						return;
					}

					if (aph->AckMesgFullType == AckMsgAdminPostUserData)
					{
						if (isFirstDataPacket)
						{ //第一个数据包。申请缓冲区。
							isFirstDataPacket = false;
							int datalen = 0;
							if (aph->DataSizeRemain == 0){ //没有更多的数据了。
								datalen = irecved - 2 - sizeof(ACKPACKETHEAD);
								if (datalen == 0){
									MessageBox("没有数据2.");
									return;
								}
								pUserData = new char [datalen];
								memcpy(pUserData,recvbuf+2+sizeof(ACKPACKETHEAD),irecved - 2 - sizeof(ACKPACKETHEAD));
								dataRecvedLen += irecved - 2 - sizeof(ACKPACKETHEAD);
								break; //第一个数据包就是全部数据。
							}else{
								datalen = irecved - 2 - sizeof(ACKPACKETHEAD) + aph->DataSizeRemain;
								pUserData = new char [datalen];
								memcpy(pUserData,recvbuf+2+sizeof(ACKPACKETHEAD),irecved - 2 - sizeof(ACKPACKETHEAD));
								dataRecvedLen += irecved - 2- sizeof(ACKPACKETHEAD);

								PACKETGENERALHEAD  pa1;
								pa1.CheckSum = 0;
								pa1.GameType = 0;
								pa1.GameVersion = 1;
								pa1.HeadFlag = AffairType_Dovalidate;
								pa1.HeadUserID = 0;
								pa1.MesgFullType = MsgMoreUserInfo;
								pa1.PacketLen = sizeof(pa1);
								pa1.WgVersion =1;
								*(WORD*)sendbuf = sizeof(pa1);
								memcpy(sendbuf+2,&pa1,sizeof(pa1));
								send(g_sComm,sendbuf,sizeof(pa1)+2,0);//有更多的数据需要去取1。
							}
						}else{
							//不是第一个数据包。
							int datalen = 0;
							datalen = irecved - 2 - sizeof(ACKPACKETHEAD);
							memcpy(pUserData+dataRecvedLen,recvbuf+2+sizeof(ACKPACKETHEAD),datalen);
							dataRecvedLen += datalen;

							if (aph->DataSizeRemain == 0)
							{//没有更多数据，退出循环。
								break;
							}else{
								PACKETGENERALHEAD  pa1;
								pa1.CheckSum = 0;
								pa1.GameType = 0;
								pa1.GameVersion = 1;
								pa1.HeadFlag = AffairType_Dovalidate;
								pa1.HeadUserID = 0;
								pa1.MesgFullType = MsgMoreUserInfo;
								pa1.PacketLen = sizeof(pa1);
								pa1.WgVersion =1;
								*(WORD*)sendbuf = sizeof(pa1);
								memcpy(sendbuf+2,&pa1,sizeof(pa1));
								send(g_sComm,sendbuf,sizeof(pa1)+2,0);//有更多的数据需要去取2。
							}
						}
					}else{
						if (AckMsgAdminNoMoreUserData == aph->AckMesgFullType){
							break;
						}else{
							char btss[255];
							sprintf(btss,"未知消息%x",aph->AckMesgFullType);
							MessageBox(btss);
						}
					}
				}

				irecved = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
				DebugMessage("recv bytes %d",irecved);
				if (irecved == 0){
					MessageBox("连接断开13");
					return;
				}else if (irecved == -1){
					MessageBox("连接断开14");
					return;
				}
			}

			if(dataRecvedLen>0){
				//[8字节 "TagBegin"]+[32字节 name] + [4字节 Crc32] + [f.len 文件内容]
				parseFileInfos(pUserData,dataRecvedLen);
				PostMessage(DATA_UPDATED);
			}
		}
	}
}

struct FileInfo {
	std::string name;
	unsigned int size_file;
	unsigned int crc;
	char* pContent;
};

void CRemoteAdminDlg::parseFileInfos( char* pUserData, int dataRecvedLen )
{
	BYTE* readPos = (BYTE*)pUserData;
	int len = dataRecvedLen;
	if (pUserData == NULL || dataRecvedLen == 0){
		return;
	}
	std::vector<FileInfo> _fi_vec;
	while(1){
		if (len <= 0){
			break;
		}

		FileInfo f;

		char tag[8] = {0};
		memcpy(tag,readPos,8);tag[7] = 0;
		if(strcmp(tag,"TagBegi")){
			MessageBox("因为读取到错误的TAG标记，解析从服务器下载的数据失败。");
			break;
		}
		readPos += 8; len -= 8;

		char name[32] = {0};
		memcpy(name,readPos,32);name[31] = 0;
		DebugMessage("parse file.name = %s",name);
		f.name = name;
		readPos += 32; len -= 32;

		f.crc = *(DWORD*)readPos;
		readPos += 4; len -= 4;
		DebugMessage("parse file.crc = %d",f.crc);
		
		f.size_file = *(DWORD*)readPos;
		readPos += 4; len -= 4;
		DebugMessage("parse file.size = %d",f.size_file);
		
		_fi_vec.push_back(f);
		CreateFileWithBuf(readPos,name,f.size_file);
		readPos += f.size_file; len -= f.size_file;
	}
// 	for (int i=0; i!=_fi_vec.size(); i++){
// 		FileInfo f = _fi_vec[i];
// 		if (clientFiles[f.name] != f.crc ){
// 			//CRC不一样，那么需要传文件过去。
// 			//[8字节 "TagBegin"]+[32字节 name] + [4字节 Crc32] + [f.len 文件内容]
// 			char tag[8] = "TagBegi";
// 			memcpy(writePos,tag,8);						writePos += 8;
// 			char name[32] = {0};
// 			strncpy(name,f.name.c_str(),32); name[31] = 0;
// 			memcpy(writePos,name,32);					writePos += 32;
// 			*(DWORD*)writePos = f.crc;					writePos += 4;
// 			memcpy(writePos,f.pContent,f.size_file);	writePos += f.size_file;
// 			DebugMessage("拷贝文件NAME=%s,CRC32=0x%X,所需长度=%d",f.name.c_str(),f.crc,f.size_file+44);
// 		}
// 	}
}

BOOL CRemoteAdminDlg::CreateFileWithBuf( BYTE* readPos,LPCTSTR filename,int size_file )
{
	std::string script_file = Path::getApplicationDirPath();
	script_file += "CEnables\\";
	script_file += filename;
	FILE* fp = fopen(script_file.c_str(),"w");
	if (!fp)
	{
		char bxx[255] = {0};
		sprintf(bxx,"写文件时候打开文件%s失败\n",script_file.c_str());
		MessageBox(bxx);
		return 0;
	}
	fwrite(readPos,1,size_file,fp);
	fclose(fp);
	return TRUE;
}
BOOL CRemoteAdminDlg::PreTranslateMessage(MSG* pMsg)
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

void CRemoteAdminDlg::LoadLocalFiles()
{
	m_ComboName.ResetContent();
	m_nameset.clear();

	g_mum.load();
	for (int i=0; i!=g_mum._usrs.size(); i++){
		m_nameset.insert(g_mum._usrs[i]->getName());	
	}

	for (std::set<std::string>::const_iterator it = m_nameset.begin(); it!=m_nameset.end();++it){
		m_ComboName.AddString((*it).c_str());
	}
}
void CRemoteAdminDlg::OnCbnSelchangeComboUserselect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	m_ComboName.GetLBText(m_ComboName.GetCurSel(),cs);
	std::string name = (LPCTSTR)cs;
	std::vector<CMyNineXUsr*> _usrs_with_name;
	for (int i=0; i!=g_mum._usrs.size(); i++)
	{
		if (g_mum._usrs[i]->getName() == name){
			_usrs_with_name.push_back(g_mum._usrs[i]);
		}
	}

	m_ListCtrlUsers.DeleteAllItems();

	for (int i=0; i!=_usrs_with_name.size(); i++)
	{
		CString sText;
		sText.Format("%d",i+1);
		m_ListCtrlUsers.InsertItem(i,sText);
		sText = _usrs_with_name[i]->getAuthId().c_str();
		m_ListCtrlUsers.SetItemText(i,1,sText);
		sText = _usrs_with_name[i]->getCardDaysType().c_str();
		m_ListCtrlUsers.SetItemText(i,2,sText);
		sText = _usrs_with_name[i]->getHarddisk().c_str();
		m_ListCtrlUsers.SetItemText(i,3,sText);
		sText = _usrs_with_name[i]->getDate_of_end().c_str();
		m_ListCtrlUsers.SetItemText(i,4,sText);
	}
}

void CRemoteAdminDlg::OnBnClickedButtonnewcard()
{
	// TODO: 在此添加控件通知处理程序代码
	DlgGenNewCard d;
	d.m_nameset = m_nameset;
	if (d.DoModal() == IDOK)
	{
		if (IDYES == MessageBox("开卡完毕，但是账号信息并未下载到本地，是否立即更新？按“是”立即更新，按“否”下次登录时再更新。","更新账号",MB_YESNO))
		{
			SyncFilesWithServer();
		}

		m_ComboName.ResetContent();
		UpdateData(FALSE);
		for (int i=0; i!=g_mum._usrs.size(); i++){
			m_nameset.insert(g_mum._usrs[i]->getName());	
		}

		for (std::set<std::string>::const_iterator it = m_nameset.begin(); it!=m_nameset.end();++it){
			m_ComboName.AddString((*it).c_str());
		}
	}
}

void CRemoteAdminDlg::SyncFilesWithServer()
{
	bool successed_verify = true;
	char sendbuf[4096] = {0};
	int isend = 0;
	char recvbuf[4096] = {0};
	int irecved = 0;
	m_ListCtrlUsers.DeleteAllItems();
	if (successed_verify)
	{
		//首先清零原有的内容
		std::vector<abc> l_bbc;

		//便利目录下的文件，重新生成一份。
		std::string str = Path::getApplicationDirPath();
		str += "CEnables\\";
		File::createPath(str);
		StringList sl = File(str).getFileListRecursive();
		int iLoad = 0;
		if (!sl.empty()){
			for (StringList::const_iterator it = sl.begin(); it!=sl.end(); ++it){
				int iCrc = openfile_calcCRC32(*it);
				bool load_success = false;
				if (iCrc == -1){
					continue;
				}else{
					File f(*it);
					std::string name = f.getFileName();
					abc bxc;
					strncpy(bxc.name,name.c_str(),sizeof(bxc.name));
					bxc.name[31] = 0;
					bxc.crc32s = iCrc;
					DebugMessage("Client name = %s,CRC=0x%08X",name.c_str(),bxc.crc32s);
					l_bbc.push_back(bxc);
				}
			}

		}

		PACKETGENERALHEAD pgh;
		pgh.CheckSum = 0;
		pgh.GameType = 0;
		pgh.GameVersion = 1;
		pgh.HeadFlag = AffairType_Dovalidate;
		pgh.HeadUserID = 0;
		pgh.MesgFullType = MsgNeedUserInfo;
		pgh.PacketLen = sizeof(pgh);
		pgh.WgVersion = 1;

		*(WORD*)sendbuf = sizeof(pgh)+l_bbc.size()*36;
		memcpy(sendbuf+2,&pgh,sizeof(pgh));

		for (int i=0; i!=l_bbc.size(); i++){
			DebugMessage("client file name = %s,icrc = 0x%X",l_bbc[i].name,l_bbc[i].crc32s);
			memcpy(sendbuf+2+sizeof(PACKETGENERALHEAD)+i*36,l_bbc[i].name,32);
			*(DWORD*)(sendbuf+2+sizeof(PACKETGENERALHEAD)+i*36+32) = l_bbc[i].crc32s;
		}

		isend = send(g_sComm,sendbuf,sizeof(pgh)+l_bbc.size()*36+2,0);
		if (isend == 0 || isend == -1)
		{
			MessageBox("发送数据错误3");
			return;
		}

		irecved = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
		DebugMessage("Receive bytes %d,total bytes %d",irecved,(*(WORD*)recvbuf));
		if (irecved == 0){
			MessageBox("连接断开3");
			return;
		}else if (irecved == -1){
			MessageBox("连接断开4");
			return;
		}else{
			bool isFirstDataPacket = true;
			bool moreData = true;
			char* pUserData = NULL;
			int dataRecvedLen = 0;
			while (moreData)
			{
				if (irecved < sizeof(ACKPACKETHEAD)+2)
				{
					MessageBox("数据错误31");
					return;
				}else{
					ACKPACKETHEAD* aph = (ACKPACKETHEAD*)(recvbuf+2);
					if (isFirstDataPacket && (aph->AckMesgFullType == AckMsgAdminNoMoreUserData))
					{
						isFirstDataPacket = false;
						MessageBox("服务器没有数据可以提供。");
						return;
					}

					if (AckMsgAdminAllUserFileSame == aph->AckMesgFullType){
						PostMessage(DATA_UPDATED);
						MessageBox("成功同步服务器与客户端的账号文件");
						return;
					}

					if (AckMsgAdminOpenFileFailed == aph->AckMesgFullType)
					{
						MessageBox("服务器繁忙，账号文件正被占用，请稍后再试。");
						return;
					}

					if (aph->AckMesgFullType == AckMsgAdminPostUserData)
					{
						if (isFirstDataPacket)
						{ //第一个数据包。申请缓冲区。
							isFirstDataPacket = false;
							int datalen = 0;
							if (aph->DataSizeRemain == 0){ //没有更多的数据了。
								datalen = irecved - 2 - sizeof(ACKPACKETHEAD);
								if (datalen == 0){
									MessageBox("没有数据2.");
									return;
								}
								pUserData = new char [datalen];
								memcpy(pUserData,recvbuf+2+sizeof(ACKPACKETHEAD),irecved - 2 - sizeof(ACKPACKETHEAD));
								dataRecvedLen += irecved - 2 - sizeof(ACKPACKETHEAD);
								break; //第一个数据包就是全部数据。
							}else{
								datalen = irecved - 2 - sizeof(ACKPACKETHEAD) + aph->DataSizeRemain;
								pUserData = new char [datalen];
								memcpy(pUserData,recvbuf+2+sizeof(ACKPACKETHEAD),irecved - 2 - sizeof(ACKPACKETHEAD));
								dataRecvedLen += irecved - 2- sizeof(ACKPACKETHEAD);

								PACKETGENERALHEAD  pa1;
								pa1.CheckSum = 0;
								pa1.GameType = 0;
								pa1.GameVersion = 1;
								pa1.HeadFlag = AffairType_Dovalidate;
								pa1.HeadUserID = 0;
								pa1.MesgFullType = MsgMoreUserInfo;
								pa1.PacketLen = sizeof(pa1);
								pa1.WgVersion =1;
								*(WORD*)sendbuf = sizeof(pa1);
								memcpy(sendbuf+2,&pa1,sizeof(pa1));
								send(g_sComm,sendbuf,sizeof(pa1)+2,0);//有更多的数据需要去取1。
							}
						}else{
							//不是第一个数据包。
							int datalen = 0;
							datalen = irecved - 2 - sizeof(ACKPACKETHEAD);
							memcpy(pUserData+dataRecvedLen,recvbuf+2+sizeof(ACKPACKETHEAD),datalen);
							dataRecvedLen += datalen;

							if (aph->DataSizeRemain == 0)
							{//没有更多数据，退出循环。
								break;
							}else{
								PACKETGENERALHEAD  pa1;
								pa1.CheckSum = 0;
								pa1.GameType = 0;
								pa1.GameVersion = 1;
								pa1.HeadFlag = AffairType_Dovalidate;
								pa1.HeadUserID = 0;
								pa1.MesgFullType = MsgMoreUserInfo;
								pa1.PacketLen = sizeof(pa1);
								pa1.WgVersion =1;
								*(WORD*)sendbuf = sizeof(pa1);
								memcpy(sendbuf+2,&pa1,sizeof(pa1));
								send(g_sComm,sendbuf,sizeof(pa1)+2,0);//有更多的数据需要去取2。
							}
						}
					}else{
						if (AckMsgAdminNoMoreUserData == aph->AckMesgFullType){
							break;
						}else{
							char btss[255];
							sprintf(btss,"未知消息%x",aph->AckMesgFullType);
							MessageBox(btss);
						}
					}
				}

				irecved = recv(g_sComm,recvbuf,sizeof(recvbuf),0);
				DebugMessage("recv bytes %d",irecved);
				if (irecved == 0){
					MessageBox("连接断开13");
					return;
				}else if (irecved == -1){
					MessageBox("连接断开14");
					return;
				}
			}

			if(dataRecvedLen>0){
				//[8字节 "TagBegin"]+[32字节 name] + [4字节 Crc32] + [f.len 文件内容]
				parseFileInfos(pUserData,dataRecvedLen);
				PostMessage(DATA_UPDATED);
			}
		}
	}
}

LRESULT CRemoteAdminDlg::OnDataUpdated( WPARAM wParam,LPARAM lParam )
{
	LoadLocalFiles();
	return 0;
}
void CRemoteAdminDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_UnbindID == "")
	{
		MessageBox("需要解绑的ID不能为空！");
		return;
	}
	if(g_mum.unbindNoCondition(std::string((LPCTSTR)m_UnbindID)) == 0)
		MessageBox("解绑成功！");
	else
		MessageBox("解绑失败！");
}

void CRemoteAdminDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_UnbindID == "")
	{
		MessageBox("需要解绑的ID不能为空！");
		return;
	}
	if(g_mum.unbindSub3Days(std::string((LPCTSTR)m_UnbindID)) == 0)
		MessageBox("解绑成功！");
	else
		MessageBox("解绑失败！");
}
