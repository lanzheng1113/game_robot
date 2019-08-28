
// MuSrvDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MuSrv.h"
#include "MuSrvDlg.h"
#include "testServer.h"
#include "DlgAuth.h"
#include "OffsetMgr.h"
#include "../common/Notice.h"
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


// CMuSrvDlg 对话框




CMuSrvDlg::CMuSrvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMuSrvDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMuSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMuSrvDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMuSrvDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMuSrvDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_NOTICE, &CMuSrvDlg::OnBnClickedButtonReloadNotice)
END_MESSAGE_MAP()


// CMuSrvDlg 消息处理程序

BOOL CMuSrvDlg::OnInitDialog()
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
	OffsetMgr::getInstance().reloadOffsetFromInis();
	SetWindowText(m_title);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMuSrvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMuSrvDlg::OnPaint()
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
HCURSOR CMuSrvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMuSrvDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox("确定退出？","退出程序",MB_YESNO) == IDYES)
	{
		ShutDownServer();
		CDialog::OnClose();
	}
	//CDialog::OnClose();
}

BOOL CMuSrvDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
// 		if (GetFocus() == (CWnd*)GetDlgItem(IDC_EDIT_FILTER_ProxyName))
// 		{
// 			UpdateData(TRUE);
// 			OnRefreshUserList(m_strProxyName);
// 		}
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMuSrvDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	DlgAuth d;
	d.DoModal();
}

void CMuSrvDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OffsetMgr::getInstance().reloadOffsetFromInis();
	MessageBox("载入完成。");
}

void CMuSrvDlg::setTitle( CString mtitle )
{
	m_title = mtitle;
}

void CMuSrvDlg::OnBnClickedButtonReloadNotice()
{
	// TODO: 在此添加控件通知处理程序代码
	CNotice::getInstance().read();
}
