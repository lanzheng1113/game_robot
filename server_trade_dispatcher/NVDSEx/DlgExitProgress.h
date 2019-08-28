#pragma once
#include "afxcmn.h"

#include "Resource.h"
// CDlgExitProgress 对话框

#define EXIT_MSG_CLOSING_DISPATCHER		(WM_USER + 200)
#define EXIT_MSG_CLOSING_DATABASE		(WM_USER + 201)
#define EXIT_MSG_CLOSING_USERS_MGR		(WM_USER + 202)
#define EXIT_MSG_CLOSING_TCP_SERVER		(WM_USER + 203)
#define EXIT_COMPLETE					(WM_USER + 204)
#define EXIT_MSG_WAIT_PROCESS_ORDER		(WM_USER + 205)
#define EXIT_MSG_FORCE_EXIT				(WM_USER + 206)
#define EXIT_MSG_CLOSING_ORDER_DISPATCHER	(WM_USER + 207)
#define EXIT_MS_WAIT_PROCESS_CONFIRM	(WM_USER + 208)
#define EXIT_MSG_CLOSING_CONFIRM_DISPATCHER	(WM_USER+209)
class CDlgExitProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgExitProgress)

public:
	CDlgExitProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExitProgress();

// 对话框数据
	enum { IDD = IDD_DIALOG_Exit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strProgress;
	CProgressCtrl m_Progress_Exit;
	afx_msg void OnBnClickedButtonForceExit();
	afx_msg LRESULT OnMsgClosingDispatcher(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgClosingDataBase(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgClosingUserMgr(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgClosingTCP_Server(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgCloseComplete(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageCloseOrderWait(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgWaitExitClientConfirm(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMegForceClostMacDispatcher(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgCloseOrderDispatcher(WPARAM wParam,LPARAM lParam);
	BOOL m_isReady;
	afx_msg void OnClose();
};
