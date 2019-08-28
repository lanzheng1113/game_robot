
// recordreaderDlg.cpp : ʵ���ļ�
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


// CrecordreaderDlg �Ի���




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


// CrecordreaderDlg ��Ϣ�������

BOOL CrecordreaderDlg::OnInitDialog()
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
	setlocale( LC_ALL, "chs" );
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CrecordreaderDlg::OnPaint()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��2�������͵�3��������������Ϊ����չ�������������Ƿǳ���Ҫ���ļ����ͣ�����򿪶Ի����ʱ//��ͻᷢ����2����һ�����ļ�������һ�������ļ�����
	CFileDialog FileDlg( TRUE , "(*.txt)|*.txt","*.txt" ,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,"�ı��ļ�(*.txt)");
	FileDlg.m_ofn.lpstrInitialDir=Path::getApplicationDirPath().c_str();
	
	if(FileDlg.DoModal()==IDOK){
		CString strFileName = FileDlg.GetPathName();
		std::ifstream ifs;
		ifs.open((LPCTSTR)strFileName);
		m_recs.clear();
		if (ifs.is_open())
		{
			/*
			###������¼��ʼ###
			##DateTime=2013-11-02 13:45:16##
			##Name=���Կ�##
			##Days=5##
			##Count=3##
			#AuthId = 677672737#
			#AuthId = 148638077#
			#AuthId = 751056685#
			###������¼����###
			*/
			std::string line;
			int lines = 0;
			std::vector<std::string> str_block;
			bool blockStarted = false;
			while (getline(ifs,line)){
				String lineEx(line);	//�޳�LINE�еĿո�
				lineEx = lineEx.trim();
				if (lineEx.beginsWith("###������¼��ʼ")){
					blockStarted = true;
					continue;
				}

				if (lineEx.beginsWith("###������¼����")){
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
		bz2 += "��][����:";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnRefresh();
}

void CrecordreaderDlg::OnBnClickedButtonsum()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strname;
	m_combo_kaikasub.GetLBText(m_combo_kaikasub.GetCurSel(),strname);
	if (strname != ""){
		m_dwKaiKaSum = g_cts[(LPCTSTR)strname];
		UpdateData(FALSE);
	}
}
