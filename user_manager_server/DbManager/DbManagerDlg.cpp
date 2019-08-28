
// DbManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DbManager.h"
#include "DbManagerDlg.h"
#include "MySQL.h"
#include <fstream>
#include <algorithm>
#include "util/StringEx.h"
#include "util/File.h"
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


// CDbManagerDlg �Ի���




CDbManagerDlg::CDbManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDbManagerDlg::IDD, pParent)
	, m_strSQL(_T(""))
	, m_strEditorSQL(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDbManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATASET, m_ListCtrlDataSet);
	DDX_Control(pDX, IDC_COMBO_History, m_ComboHistory);
	DDX_Text(pDX, IDC_EDIT_SQL, m_strEditorSQL);
}

BEGIN_MESSAGE_MAP(CDbManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_exec, &CDbManagerDlg::OnBnClickedButtonexec)
	ON_BN_CLICKED(IDC_BUTTON_execEdit, &CDbManagerDlg::OnBnClickedButtonexecedit)
END_MESSAGE_MAP()


// CDbManagerDlg ��Ϣ�������

BOOL CDbManagerDlg::OnInitDialog()
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
	if (!CMySQL::getInstance().connectDB())
	{
		AfxMessageBox("�����ݿ�ʧ���ˡ�");
		ExitProcess(0);
	}

	LONG lStyle = m_ListCtrlDataSet.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_ListCtrlDataSet.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);

	loadhistory();
	/*
	CString strIns;
	int nItems = m_pComboBox->GetCount();
	for (int i = 0; i < nItems; i++)
	{
	strIns.Format(_T("item string %c"), (char)('A'+i));
	m_pComboBox->InsertString(2*i, strIns);
	}
	*/
	
	for (std::list<std::string>::const_iterator it =m_vecHistory.begin(); it!=m_vecHistory.end(); it++){
		int nItems = m_ComboHistory.GetCount();
// 		if (nItems == 0){
// 			m_ComboHistory.AddString(m_vecHistory[i].c_str());
// 		}else
			m_ComboHistory.InsertString(0,it->c_str());
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDbManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDbManagerDlg::OnPaint()
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
HCURSOR CDbManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDbManagerDlg::OnBnClickedButtonexec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMbd;
	m_ComboHistory.GetWindowText(strMbd);
	if (strMbd == ""){
		AfxMessageBox("����Ϊ�ա�");
	}else{
		BOOL bxx = CMySQL::getInstance().Exec(strMbd,m_ListCtrlDataSet);
		if (bxx){
			std::list<std::string>::iterator it = find(m_vecHistory.begin(),m_vecHistory.end(),std::string((LPCTSTR)strMbd));
			if ( it == m_vecHistory.end()){
				m_vecHistory.push_back((LPCTSTR)strMbd);
				m_ComboHistory.InsertString(0,strMbd);
			}else{
				m_vecHistory.remove(*it);
				m_vecHistory.push_back((LPCTSTR)strMbd);
				int pos = -1;
				for (int i = 0; i!=m_ComboHistory.GetCount(); i++){
					CString rs;
					m_ComboHistory.GetLBText(i,rs);
					if(rs == strMbd){
						pos = i;
						break;
					}
				}

				if (pos != CB_ERR){
					DebugMessageA("pos = %d,str = %s",pos,(LPCTSTR)strMbd);
					m_ComboHistory.DeleteString(pos);
					m_ComboHistory.InsertString(0,strMbd);
				}
			}
		}
	}
}

BOOL CDbManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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

void CDbManagerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CMySQL::getInstance().Close();
	saveHistory();
	CDialog::OnClose();
}


void CDbManagerDlg::loadhistory()
{
	std::ifstream ifs;
	ifs.open("history.txt");
	if (ifs.is_open())
	{
		std::string line;
		int lines = 0;
		while (getline(ifs,line)){
//			DebugMessage("%d:%s",lines+1,line.c_str());
			String lineEx(line);	//�޳�LINE�еĿո�
			lineEx = lineEx.trim();
			lineEx.remove("\r");
			lineEx.remove("\n");
			lines ++;
			m_vecHistory.push_back(line);
		}
		ifs.close();
	}else{
		char xzxxe[255] = {0};
		sprintf(xzxxe,"���ļ�historyʧ����LastError=%d��",GetLastError());
		MessageBox(xzxxe);
	}
}

void CDbManagerDlg::saveHistory()
{
	if (!m_vecHistory.empty())
	{
		FileWriter fr("history.txt");
		for (std::list<std::string>::const_iterator it = m_vecHistory.begin(); it != m_vecHistory.end(); ++it)
		{
			std::string frstring = *it;
			frstring += "\r\n";
			fr.write(frstring);
		}
		fr.close();
	}
}

void CDbManagerDlg::OnBnClickedButtonexecedit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CMySQL::getInstance().Exec(m_strEditorSQL,m_ListCtrlDataSet);
}
