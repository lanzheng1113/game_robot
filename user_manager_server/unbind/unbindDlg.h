
// unbindDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CunbindDlg 对话框
class CunbindDlg : public CDialog
{
// 构造
public:
	CunbindDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UNBIND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strCardID;
	afx_msg void OnBnClickedButtonUnbind();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_static;
	CEdit m_editPswd;
	CButton m_buttonLogin;
	afx_msg void OnBnClickedButtonlogin();
	int unbindNoCondition( const CString& strId );
	CString m_strPswd;
	CString m_strAcc;
	afx_msg void OnBnClickedCheckRemeberpswd();
	BOOL m_bIsRemeberPswd;
};
