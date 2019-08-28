// DlgExitProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "MuSrv.h"
#include "DlgExitProgress.h"


// DlgExitProgress 对话框

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


// DlgExitProgress 消息处理程序
// CDlgExitProgress 消息处理程序

BOOL DlgExitProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Progress_Exit.SetRange(0,100);
	m_Progress_Exit.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL DlgExitProgress::PreTranslateMessage(MSG* pMsg)
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

void DlgExitProgress::OnBnClickedButtonForceExit()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitProcess(0);
}
