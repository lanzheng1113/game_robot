// DlgExitProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NVDSEx.h"
#include "DlgExitProgress.h"


// CDlgExitProgress �Ի���

IMPLEMENT_DYNAMIC(CDlgExitProgress, CDialog)

CDlgExitProgress::CDlgExitProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExitProgress::IDD, pParent)
	, m_strProgress(_T(""))
{
	m_isReady = FALSE;
}

CDlgExitProgress::~CDlgExitProgress()
{
}

void CDlgExitProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_Exit, m_strProgress);
	DDX_Control(pDX, IDC_PROGRESS_ExitProgram, m_Progress_Exit);
}

BEGIN_MESSAGE_MAP(CDlgExitProgress, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Force_Exit, &CDlgExitProgress::OnBnClickedButtonForceExit)
	ON_MESSAGE(EXIT_MSG_CLOSING_DISPATCHER,&CDlgExitProgress::OnMsgClosingDispatcher)
	ON_MESSAGE(EXIT_MSG_CLOSING_DATABASE,&CDlgExitProgress::OnMsgClosingDataBase)
	ON_MESSAGE(EXIT_MSG_CLOSING_USERS_MGR,&CDlgExitProgress::OnMsgClosingUserMgr)
	ON_MESSAGE(EXIT_MSG_CLOSING_TCP_SERVER,&CDlgExitProgress::OnMsgClosingTCP_Server)
	ON_MESSAGE(EXIT_MSG_WAIT_PROCESS_ORDER,&CDlgExitProgress::OnMessageCloseOrderWait)
	ON_MESSAGE(EXIT_MSG_FORCE_EXIT,&CDlgExitProgress::OnMegForceClostMacDispatcher)
	ON_MESSAGE(EXIT_COMPLETE,&CDlgExitProgress::OnMsgCloseComplete)
	ON_MESSAGE(EXIT_MSG_CLOSING_ORDER_DISPATCHER,&CDlgExitProgress::OnMsgCloseOrderDispatcher)
	ON_MESSAGE(EXIT_MS_WAIT_PROCESS_CONFIRM,&CDlgExitProgress::OnMsgWaitExitClientConfirm)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgExitProgress ��Ϣ�������

BOOL CDlgExitProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Progress_Exit.SetRange(0,100);
	m_Progress_Exit.SetPos(0);
	m_isReady = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgExitProgress::PreTranslateMessage(MSG* pMsg)
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

void CDlgExitProgress::OnBnClickedButtonForceExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitProcess(0);
}

LRESULT CDlgExitProgress::OnMsgClosingDispatcher( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"���ڱ������ݲ��ر��ɷ���";
	m_Progress_Exit.SetPos(10);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgClosingDataBase( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"�ر����ݿ����Ӻ���־д��ģ�顭��";
	m_Progress_Exit.SetPos(30);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgClosingUserMgr( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"�ر��û�����������";
	m_Progress_Exit.SetPos(40);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgClosingTCP_Server( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"�ر�TCP������ģ�顭��";
	m_Progress_Exit.SetPos(60);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgCloseComplete( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"�ɹ��˳�������";
	m_Progress_Exit.SetPos(100);
	UpdateData(FALSE);
	OnOK();
	return 0;
}

LRESULT CDlgExitProgress::OnMessageCloseOrderWait( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"�������пͻ��˷���֪ͨ���ȴ��ͻ���ȡ������������ǰʣ��%d����%d���δ����ǿ�йرգ�",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMegForceClostMacDispatcher( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"ʣ��%d��δ�رգ�ǿ�жϿ������пͻ��˵����ӡ����Ե�%d��",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgCloseOrderDispatcher( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"����ִ���������ɷ�������",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgWaitExitClientConfirm( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"�ȴ��ͻ��������˶�����ʣ��%d����%d���ǿ�йر����ӣ�",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}


/*
pDlg->m_strProgress = L"�ر��ɷ���";
pDlg->UpdateData(FALSE);
CObjectFactory::getInstance().GetMacDispatcher()->exit();
CObjectFactory::getInstance().GetOrderDispatcher()->exit();
//
pDlg->m_strProgress = L"�ر����ݿ����Ӻ���־д��ģ�顭��";
pDlg->UpdateData(FALSE);
afkGetDbObject()->Close();
pDlg->m_Progress_Exit.SetPos(20);

pDlg->m_strProgress = L"�ر��û�����������";
pDlg->UpdateData(FALSE);
afkGetTradeAffairManager()->Close();

pDlg->m_Progress_Exit.SetPos(40);

pDlg->m_strProgress = L"�ر�TCP������ģ�顭��";
pDlg->UpdateData(FALSE);
server.ShutDown();
pDlg->m_Progress_Exit.SetPos(80);
pDlg->UpdateData(FALSE);

delete pDlg;
Sleep(100);
*/
void CDlgExitProgress::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ExitProcess(1);
	CDialog::OnClose();
}
