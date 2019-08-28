// DlgExitProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MuSrv.h"
#include "DlgExitProgress.h"


// DlgExitProgress �Ի���

IMPLEMENT_DYNAMIC(DlgExitProgress, CDialog)

DlgExitProgress::DlgExitProgress(CWnd* pParent /*=NULL*/)
	: CDialog(DlgExitProgress::IDD, pParent)
{

}

DlgExitProgress::~DlgExitProgress()
{
}

void DlgExitProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_Exit, m_strProgress);
	DDX_Control(pDX, IDC_PROGRESS_ExitProgram, m_Progress_Exit);
}


BEGIN_MESSAGE_MAP(DlgExitProgress, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Force_Exit, &DlgExitProgress::OnBnClickedButtonForceExit)
END_MESSAGE_MAP()


// DlgExitProgress ��Ϣ�������
// CDlgExitProgress ��Ϣ�������

BOOL DlgExitProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Progress_Exit.SetRange(0,100);
	m_Progress_Exit.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL DlgExitProgress::PreTranslateMessage(MSG* pMsg)
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

void DlgExitProgress::OnBnClickedButtonForceExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitProcess(0);
}
