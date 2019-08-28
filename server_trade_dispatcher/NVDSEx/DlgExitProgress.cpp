// DlgExitProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "NVDSEx.h"
#include "DlgExitProgress.h"


// CDlgExitProgress 对话框

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


// CDlgExitProgress 消息处理程序

BOOL CDlgExitProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Progress_Exit.SetRange(0,100);
	m_Progress_Exit.SetPos(0);
	m_isReady = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgExitProgress::PreTranslateMessage(MSG* pMsg)
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

void CDlgExitProgress::OnBnClickedButtonForceExit()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitProcess(0);
}

LRESULT CDlgExitProgress::OnMsgClosingDispatcher( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"正在保存数据并关闭派发器";
	m_Progress_Exit.SetPos(10);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgClosingDataBase( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"关闭数据库连接和日志写入模块……";
	m_Progress_Exit.SetPos(30);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgClosingUserMgr( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"关闭用户管理器……";
	m_Progress_Exit.SetPos(40);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgClosingTCP_Server( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"关闭TCP服务器模块……";
	m_Progress_Exit.SetPos(60);
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgCloseComplete( WPARAM wParam,LPARAM lParam )
{
	m_strProgress = L"成功退出服务器";
	m_Progress_Exit.SetPos(100);
	UpdateData(FALSE);
	OnOK();
	return 0;
}

LRESULT CDlgExitProgress::OnMessageCloseOrderWait( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"已向所有客户端发送通知，等待客户端取消订单……当前剩余%d个。%d秒后未处理将强行关闭！",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMegForceClostMacDispatcher( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"剩余%d个未关闭，强行断开与所有客户端的连接……稍等%d秒",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgCloseOrderDispatcher( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"正在执行清理订单派发器……",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}

LRESULT CDlgExitProgress::OnMsgWaitExitClientConfirm( WPARAM wParam,LPARAM lParam )
{
	CString str;
	str.Format(L"等待客户端完成审核订单，剩余%d个。%d秒后强行关闭连接！",wParam,lParam);
	m_strProgress = str;
	UpdateData(FALSE);
	return 0;
}


/*
pDlg->m_strProgress = L"关闭派发器";
pDlg->UpdateData(FALSE);
CObjectFactory::getInstance().GetMacDispatcher()->exit();
CObjectFactory::getInstance().GetOrderDispatcher()->exit();
//
pDlg->m_strProgress = L"关闭数据库连接和日志写入模块……";
pDlg->UpdateData(FALSE);
afkGetDbObject()->Close();
pDlg->m_Progress_Exit.SetPos(20);

pDlg->m_strProgress = L"关闭用户管理器……";
pDlg->UpdateData(FALSE);
afkGetTradeAffairManager()->Close();

pDlg->m_Progress_Exit.SetPos(40);

pDlg->m_strProgress = L"关闭TCP服务器模块……";
pDlg->UpdateData(FALSE);
server.ShutDown();
pDlg->m_Progress_Exit.SetPos(80);
pDlg->UpdateData(FALSE);

delete pDlg;
Sleep(100);
*/
void CDlgExitProgress::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ExitProcess(1);
	CDialog::OnClose();
}
