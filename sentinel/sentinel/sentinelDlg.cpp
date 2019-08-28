
// sentinelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "sentinel.h"
#include "sentinelDlg.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "TaskWatch.h"
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


// CsentinelDlg 对话框




CsentinelDlg::CsentinelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsentinelDlg::IDD, pParent)
	, m_strItem1(_T(""))
	, m_strItem2(_T(""))
	, m_strItem3(_T(""))
	, m_strItem4(_T(""))
	, m_strItem5(_T(""))
	, m_strItem6(_T(""))
	, m_taskWatch(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsentinelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Item1, m_strItem1);
	DDX_Text(pDX, IDC_EDIT_Item2, m_strItem2);
	DDX_Text(pDX, IDC_EDIT_Item3, m_strItem3);
	DDX_Text(pDX, IDC_EDIT_Item4, m_strItem4);
	DDX_Text(pDX, IDC_EDIT_Item5, m_strItem5);
	DDX_Text(pDX, IDC_EDIT_Item6, m_strItem6);
}

BEGIN_MESSAGE_MAP(CsentinelDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_Execute, &CsentinelDlg::OnBnClickedButtonExecute)
END_MESSAGE_MAP()


// CsentinelDlg 消息处理程序

BOOL CsentinelDlg::OnInitDialog()
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

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	m_taskWatch = new TaskWatch();
	if(!m_taskWatch){
		AfxMessageBox(L"创建守护对象失败(new TaskWatch failed!),程序初始化失败！");
		return FALSE;
	}
	int iCount = m_taskWatch->LoadTaskFromXML();
	if (iCount)
	{
		ShowTaskMap();
	}
	
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsentinelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsentinelDlg::OnPaint()
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
HCURSOR CsentinelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CsentinelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == GetDlgItem(IDC_EDIT_WND_NAME1))
			{
				OnPressReturnOnWndNameEdit1();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_WND_NAME2))
			{
				OnPressReturnOnWndNameEdit2();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_WND_NAME3))
			{
				OnPressReturnOnWndNameEdit3();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_WND_NAME4))
			{
				OnPressReturnOnWndNameEdit4();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_WND_NAME5))
			{
				OnPressReturnOnWndNameEdit5();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_WND_NAME6))
			{
				OnPressReturnOnWndNameEdit6();
			}
			return TRUE;
		}
		
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	if(pMsg->message == WM_LBUTTONDBLCLK){
		if (GetFocus() == GetDlgItem(IDC_EDIT_Item1))
		{
			OnLBDoubleClickTextEdit1();
			return TRUE;
		}
		else if (GetFocus() == GetDlgItem(IDC_EDIT_Item2))
		{
			OnLBDoubleClickTextEdit2();
			return TRUE;
		}
		else if (GetFocus() == GetDlgItem(IDC_EDIT_Item3))
		{
			OnLBDoubleClickTextEdit3();
			return TRUE;
		}
		else if (GetFocus() == GetDlgItem(IDC_EDIT_Item4))
		{
			OnLBDoubleClickTextEdit4();
			return TRUE;
		}
		else if (GetFocus() == GetDlgItem(IDC_EDIT_Item5))
		{
			OnLBDoubleClickTextEdit5();
			return TRUE;
		}
		else if (GetFocus() == GetDlgItem(IDC_EDIT_Item6))
		{
			OnLBDoubleClickTextEdit6();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CsentinelDlg::OnBnClickedButtonExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bOnWatch = m_taskWatch->GetOnWatch();
	if (!bOnWatch)
	{
		GetDlgItem(IDC_EDIT_Item1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Item2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Item3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Item4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Item5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Item6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WND_NAME1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WND_NAME2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WND_NAME3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WND_NAME4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WND_NAME5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WND_NAME6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Execute)->SetWindowText(L"停止");
		int iCount = m_taskWatch->LoadTaskFromXML();//reload from xml
		if (iCount){
			ShowTaskMap();
		}
		m_taskWatch->TestFun();
	}
	else
	{
		GetDlgItem(IDC_EDIT_Item1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Item2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Item3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Item4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Item5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Item6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_NAME1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_NAME2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_NAME3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_NAME4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_NAME5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WND_NAME6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Execute)->SetWindowText(L"执行");
	}
	m_taskWatch->SetOnWatch(!bOnWatch);
}

void CsentinelDlg::ShowTaskMap(){

	TASK_DESCRIBE_MAP tsk_map = m_taskWatch->GetMap();
	for(TaskDescribeMapConstIter it=tsk_map.begin();it!=tsk_map.end();++it){
		int taskLableItemId = 0;
		int taskFullPathItemId = 0;
		int taskWndNameItemId = 0;
		switch(it->first){
			case 1:
				taskLableItemId = IDC_STATIC_TASK1;
				taskFullPathItemId = IDC_EDIT_Item1;
				taskWndNameItemId = IDC_EDIT_WND_NAME1;
				break;
			case 2:
				taskLableItemId = IDC_STATIC_TASK2;
				taskFullPathItemId = IDC_EDIT_Item2;
				taskWndNameItemId = IDC_EDIT_WND_NAME2;
				break;
			case 3:
				taskLableItemId = IDC_STATIC_TASK3;
				taskFullPathItemId = IDC_EDIT_Item3;
				taskWndNameItemId = IDC_EDIT_WND_NAME3;
				break;
			case 4:
				taskLableItemId = IDC_STATIC_TASK4;
				taskFullPathItemId = IDC_EDIT_Item4;
				taskWndNameItemId = IDC_EDIT_WND_NAME4;
				break;
			case 5:
				taskLableItemId = IDC_STATIC_TASK5;
				taskFullPathItemId = IDC_EDIT_Item5;
				taskWndNameItemId = IDC_EDIT_WND_NAME5;
				break;
			case 6:
				taskLableItemId = IDC_STATIC_TASK6;
				taskFullPathItemId = IDC_EDIT_Item6;
				taskWndNameItemId = IDC_EDIT_WND_NAME6;
				break;
			default:
				break;
		}

		if(taskLableItemId == 0 || taskFullPathItemId == 0){
			continue;
		}
		else{
			wchar_t szTaskName[200] = {0};
			wchar_t szTaskExeFullPath[200] = {0};
			wchar_t szTaskWndName[200] = {0};
			
			MultiByteToWideChar(CP_ACP,0,it->second.strTaskName.c_str(),-1,szTaskName,sizeof(szTaskName)/sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP,0,it->second.strFullPath.c_str(),-1,szTaskExeFullPath,sizeof(szTaskName)/sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP,0,it->second.strWndName.c_str(),-1,szTaskWndName,sizeof(szTaskWndName)/sizeof(wchar_t));

			GetDlgItem(taskLableItemId)->SetWindowText(szTaskName);
			GetDlgItem(taskFullPathItemId)->SetWindowText(szTaskExeFullPath);
			GetDlgItem(taskWndNameItemId)->SetWindowText(szTaskWndName);
		}
	}
}

static wchar_t szFilter[] =	L"Execute Files (*.exe)|*.exe|All Files (*.*)|*.*||";

void CsentinelDlg::OnLBDoubleClickTextEdit1()
{
	CFileDialog dlg(TRUE,L"exe",NULL,4|2,szFilter);
	if(dlg.DoModal() == IDOK){
		char szFullPath[MAX_PATH] = {0};
		AfxMessageBox(dlg.GetPathName());
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dlg.GetPathName(),-1,szFullPath,sizeof(szFullPath),0,0);
		m_taskWatch->SetTaskFullPath(1,szFullPath);
		GetDlgItem(IDC_EDIT_Item1)->SetWindowText(dlg.GetPathName());
	}
}

void CsentinelDlg::OnLBDoubleClickTextEdit2()
{
	CFileDialog dlg(TRUE,L"exe",NULL,4|2,szFilter);
	if(dlg.DoModal() == IDOK){
		char szFullPath[MAX_PATH] = {0};
		AfxMessageBox(dlg.GetPathName());
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dlg.GetPathName(),-1,szFullPath,sizeof(szFullPath),0,0);
		m_taskWatch->SetTaskFullPath(2,szFullPath);
		GetDlgItem(IDC_EDIT_Item2)->SetWindowText(dlg.GetPathName());
	}
}

void CsentinelDlg::OnLBDoubleClickTextEdit3()
{
	CFileDialog dlg(TRUE,L"exe",NULL,4|2,szFilter);
	if(dlg.DoModal() == IDOK){
		char szFullPath[MAX_PATH] = {0};
		AfxMessageBox(dlg.GetPathName());
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dlg.GetPathName(),-1,szFullPath,sizeof(szFullPath),0,0);
		m_taskWatch->SetTaskFullPath(3,szFullPath);
		GetDlgItem(IDC_EDIT_Item3)->SetWindowText(dlg.GetPathName());
	}
}

void CsentinelDlg::OnLBDoubleClickTextEdit4()
{
	CFileDialog dlg(TRUE,L"exe",NULL,4|2,szFilter);
	if(dlg.DoModal() == IDOK){
		char szFullPath[MAX_PATH] = {0};
		AfxMessageBox(dlg.GetPathName());
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dlg.GetPathName(),-1,szFullPath,sizeof(szFullPath),0,0);
		m_taskWatch->SetTaskFullPath(4,szFullPath);
		GetDlgItem(IDC_EDIT_Item4)->SetWindowText(dlg.GetPathName());
	}
}

void CsentinelDlg::OnLBDoubleClickTextEdit5()
{
	CFileDialog dlg(TRUE,L"exe",NULL,4|2,szFilter);
	if(dlg.DoModal() == IDOK){
		char szFullPath[MAX_PATH] = {0};
		AfxMessageBox(dlg.GetPathName());
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dlg.GetPathName(),-1,szFullPath,sizeof(szFullPath),0,0);
		m_taskWatch->SetTaskFullPath(5,szFullPath);
		GetDlgItem(IDC_EDIT_Item5)->SetWindowText(dlg.GetPathName());
	}
}

void CsentinelDlg::OnLBDoubleClickTextEdit6()
{
	CFileDialog dlg(TRUE,L"exe",NULL,4|2,szFilter);
	if(dlg.DoModal() == IDOK){
		char szFullPath[MAX_PATH] = {0};
		AfxMessageBox(dlg.GetPathName());
		WideCharToMultiByte(CP_ACP,0,(LPCWSTR)dlg.GetPathName(),-1,szFullPath,sizeof(szFullPath),0,0);
		m_taskWatch->SetTaskFullPath(6,szFullPath);
		GetDlgItem(IDC_EDIT_Item6)->SetWindowText(dlg.GetPathName());
	}
}

void CsentinelDlg::OnPressReturnOnWndNameEdit1()
{
	char szWndName[200] = {0};
	wchar_t wszWndName[200] = {0};
	GetDlgItemText(IDC_EDIT_WND_NAME1,wszWndName,200);
	WideCharToMultiByte(CP_ACP,0,wszWndName,-1,szWndName,sizeof(szWndName),0,0);
	m_taskWatch->SetWindowName(1,szWndName);
	AfxMessageBox(wszWndName);
}

void CsentinelDlg::OnPressReturnOnWndNameEdit2()
{
	char szWndName[200] = {0};
	wchar_t wszWndName[200] = {0};
	GetDlgItemText(IDC_EDIT_WND_NAME2,wszWndName,200);
	WideCharToMultiByte(CP_ACP,0,wszWndName,-1,szWndName,sizeof(szWndName),0,0);
	m_taskWatch->SetWindowName(2,szWndName);
	AfxMessageBox(wszWndName);
}

void CsentinelDlg::OnPressReturnOnWndNameEdit3()
{
	char szWndName[200] = {0};
	wchar_t wszWndName[200] = {0};
	GetDlgItemText(IDC_EDIT_WND_NAME3,wszWndName,200);
	WideCharToMultiByte(CP_ACP,0,wszWndName,-1,szWndName,sizeof(szWndName),0,0);
	m_taskWatch->SetWindowName(3,szWndName);
	AfxMessageBox(wszWndName);
}

void CsentinelDlg::OnPressReturnOnWndNameEdit4()
{
	char szWndName[200] = {0};
	wchar_t wszWndName[200] = {0};
	GetDlgItemText(IDC_EDIT_WND_NAME4,wszWndName,200);
	WideCharToMultiByte(CP_ACP,0,wszWndName,-1,szWndName,sizeof(szWndName),0,0);
	m_taskWatch->SetWindowName(4,szWndName);
	AfxMessageBox(wszWndName);
}

void CsentinelDlg::OnPressReturnOnWndNameEdit5()
{
	char szWndName[200] = {0};
	wchar_t wszWndName[200] = {0};
	GetDlgItemText(IDC_EDIT_WND_NAME5,wszWndName,200);
	WideCharToMultiByte(CP_ACP,0,wszWndName,-1,szWndName,sizeof(szWndName),0,0);
	m_taskWatch->SetWindowName(5,szWndName);
	AfxMessageBox(wszWndName);
}

void CsentinelDlg::OnPressReturnOnWndNameEdit6()
{
	char szWndName[200] = {0};
	wchar_t wszWndName[200] = {0};
	GetDlgItemText(IDC_EDIT_WND_NAME6,wszWndName,200);
	WideCharToMultiByte(CP_ACP,0,wszWndName,-1,szWndName,sizeof(szWndName),0,0);
	m_taskWatch->SetWindowName(6,szWndName);
	AfxMessageBox(wszWndName);
}