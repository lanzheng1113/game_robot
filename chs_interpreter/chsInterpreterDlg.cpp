
// chsInterpreterDlg.cpp : ʵ���ļ�
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


// CchsInterpreterDlg �Ի���




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


// CchsInterpreterDlg ��Ϣ�������

BOOL CchsInterpreterDlg::OnInitDialog()
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CchsInterpreterDlg::OnPaint()
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
HCURSOR CchsInterpreterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CchsInterpreterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox(NULL,"��д����ļ�ʧ���ˡ�",NULL,MB_OK);
		}
	}else{
		DebugMessage("open %s failed with error:%d",nameofneclfile.c_str(),GetLastError());
		MessageBox(NULL,"�򿪶�ȡ���ļ�ʧ���ˡ�",NULL,MB_OK);
		return -1;
	}
	return icount;
}

itpcore core;

void CchsInterpreterDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//resetzidian();
//  	std::string cmd = "��� [�ж����� ս��״̬ >= 0x10 0] ��ô ������� [ ����������͵Ϯ�� ]";
//  	std::string condtion,exec;
//  	core.splite_cmd_if_condition_and_exec(cmd,condtion,exec);

	core.setSourceFile("./test.txt");
	core.load();
	core.exec();
}
