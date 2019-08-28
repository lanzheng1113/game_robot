
// chsInterpreterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chsInterpreter.h"
#include "chsInterpreterDlg.h"
#include "itpcore.h"
#include <string>
#include <vector>
#include <fstream>
#include "util/StringEx.h"
#include "util/StringList.h"
#include "debugmessage.h"

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


// CchsInterpreterDlg 对话框




CchsInterpreterDlg::CchsInterpreterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CchsInterpreterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchsInterpreterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CchsInterpreterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CchsInterpreterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CchsInterpreterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CchsInterpreterDlg 消息处理程序

BOOL CchsInterpreterDlg::OnInitDialog()
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

void CchsInterpreterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CchsInterpreterDlg::OnPaint()
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
HCURSOR CchsInterpreterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CchsInterpreterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

int resetzidian()
{
	int icount = 0;
	std::ifstream ifs;
	std::vector<std::string> cmds_;
	std::string nameofneclfile = "D:\\tst.txt";
	
	ifs.open(nameofneclfile.c_str());
	if (ifs.is_open())
	{
		std::string oneline;
		while (getline(ifs,oneline))
		{
			String strEx(oneline);
			strEx = strEx.trim();
			strEx.remove("\r");
			strEx.remove("\n");
			strEx.remove("\t");
			strEx.remove(" ");
			if (strEx.length() == 0)
			{
				continue;
			}else{
				cmds_.push_back(strEx);
			}
		}
		ifs.close();

		std::ofstream ofs;
		ofs.open("d:\\tst1.txt");
		if (ofs.is_open())
		{
			for (int i=0; i!=cmds_.size(); i++)
			{
				ofs << cmds_[i].c_str() << "\n";
			}
			ofs.close();
		}else{
			MessageBox(NULL,"打开写入的文件失败了。",NULL,MB_OK);
		}
	}else{
		DebugMessage("open %s failed with error:%d",nameofneclfile.c_str(),GetLastError());
		MessageBox(NULL,"打开读取的文件失败了。",NULL,MB_OK);
		return -1;
	}
	return icount;
}

itpcore core;

void CchsInterpreterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//resetzidian();
//  	std::string cmd = "如果 [判断人物 战斗状态 >= 0x10 0] 那么 跳到标记 [ 宝宝打炎刃偷袭者 ]";
//  	std::string condtion,exec;
//  	core.splite_cmd_if_condition_and_exec(cmd,condtion,exec);

	core.setSourceFile("./test.txt");
	core.load();
	core.exec();
}
