
// recordreaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "recordreader.h"
#include "recordreaderDlg.h"
#include "util/Path.h"
#include "util/File.h"
#include "util/StringEx.h"
#include <fstream>
#include <set>
#include <map>
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


// CrecordreaderDlg 对话框




CrecordreaderDlg::CrecordreaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CrecordreaderDlg::IDD, pParent)
	, m_dwKaiKaSum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrecordreaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_k, m_ListBoxAll);
	DDX_Control(pDX, IDC_LIST_Detail, m_ListDetail);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Text(pDX, IDC_EDIT_sum, m_dwKaiKaSum);
	DDX_Control(pDX, IDC_COMBO_Sum, m_combo_kaikasub);
}

BEGIN_MESSAGE_MAP(CrecordreaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CrecordreaderDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_BUTTON3, &CrecordreaderDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_sum, &CrecordreaderDlg::OnBnClickedButtonsum)
END_MESSAGE_MAP()


// CrecordreaderDlg 消息处理程序

BOOL CrecordreaderDlg::OnInitDialog()
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
	setlocale( LC_ALL, "chs" );
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CrecordreaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CrecordreaderDlg::OnPaint()
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
HCURSOR CrecordreaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


recordsInfo ToRecords( std::vector<std::string> str_block ) 
{
	recordsInfo ri;
	ri.count = 0;
	ri.dateTime = "";
	ri.days = 0;
	ri.KaiKaList.clear();
	ri.name = "";

	for (int i=0; i!=str_block.size(); i++){
		String strx = str_block[i];
		if (strx.beginsWith("##DateTime=")){
			strx.remove("##DateTime=");
			strx.remove("##");
			ri.dateTime = strx;
			continue;
		}

		if (strx.beginsWith("##Name="))
		{
			strx.remove("##Name=");
			strx.remove("##");
			ri.name = strx;
			continue;
		}

		if (strx.beginsWith("##Days="))
		{
			strx.remove("##Days=");
			strx.remove("##");
			ri.days = atoi(strx.c_str());
			continue;
		}

		if (strx.beginsWith("##Count="))
		{
			strx.remove("##Count=");
			strx.remove("##");
			ri.count = atoi(strx.c_str());
			continue;
		}

		if (strx.beginsWith("#AuthId = "))
		{
			strx.remove("#AuthId = ");
			strx.remove("#");
			ri.KaiKaList.push_back(strx);
			continue;
		}
	}
	return ri;
}

void CrecordreaderDlg::OnBnClickedButtonOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	//第2个参数和第3个参数都可以认为是扩展名；最后参数就是非常重要的文件类型；当你打开对话框的时//候就会发现有2栏，一个是文件名，另一个就是文件类型
	CFileDialog FileDlg( TRUE , "(*.txt)|*.txt","*.txt" ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,"文本文件(*.txt)");
	FileDlg.m_ofn.lpstrInitialDir=Path::getApplicationDirPath().c_str();
	
	if(FileDlg.DoModal()==IDOK){
		CString strFileName = FileDlg.GetPathName();
		std::ifstream ifs;
		ifs.open((LPCTSTR)strFileName);
		m_recs.clear();
		if (ifs.is_open())
		{
			/*
			###开卡记录开始###
			##DateTime=2013-11-02 13:45:16##
			##Name=测试卡##
			##Days=5##
			##Count=3##
			#AuthId = 677672737#
			#AuthId = 148638077#
			#AuthId = 751056685#
			###开卡记录结束###
			*/
			std::string line;
			int lines = 0;
			std::vector<std::string> str_block;
			bool blockStarted = false;
			while (getline(ifs,line)){
				String lineEx(line);	//剔除LINE中的空格
				lineEx = lineEx.trim();
				if (lineEx.beginsWith("###开卡记录开始")){
					blockStarted = true;
					continue;
				}

				if (lineEx.beginsWith("###开卡记录结束")){
					blockStarted = false;
					recordsInfo ri = ToRecords(str_block);
					m_recs.push_back(ri);
					str_block.clear();
					continue;
				}

				if (blockStarted){
					str_block.push_back(lineEx);
				}
				lines ++;
			}
			ifs.close();
			OnRefresh();
		}
	}
}
std::map<std::string,int> g_cts;
void CrecordreaderDlg::OnRefresh()
{
	UpdateData(TRUE);
	std::set<std::string> ns;
	bool onlyshow25dayup = false;
	onlyshow25dayup = m_Check1.GetCheck() == BST_CHECKED;
	m_ListBoxAll.ResetContent();
	m_ListDetail.ResetContent();
	m_combo_kaikasub.ResetContent();
	g_cts.clear();
	for (int i=0; i!=m_recs.size(); i++){
		if (onlyshow25dayup){
			if (m_recs[i].days < 25){
				continue;
			}
		}
		std::string bz2="[";
		bz2 += m_recs[i].name;
		ns.insert(m_recs[i].name);
		bz2 += "][";
		bz2 += m_recs[i].dateTime;
		bz2 += "][";
		bz2 += String::fromLong(m_recs[i].days);
		bz2 += "天][数量:";
		bz2 += String::fromLong(m_recs[i].count);
		g_cts[m_recs[i].name] += m_recs[i].count;
		bz2 += "]";
		m_ListBoxAll.AddString(bz2.c_str());
	}

	for (std::set<std::string>::const_iterator it = ns.begin(); it != ns.end(); ++it){
		m_combo_kaikasub.AddString(it->c_str());
	}
}

void CrecordreaderDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnRefresh();
}

void CrecordreaderDlg::OnBnClickedButtonsum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strname;
	m_combo_kaikasub.GetLBText(m_combo_kaikasub.GetCurSel(),strname);
	if (strname != ""){
		m_dwKaiKaSum = g_cts[(LPCTSTR)strname];
		UpdateData(FALSE);
	}
}
