#pragma once
#include "Resource.h"

// DlgExitProgress 对话框

class DlgExitProgress : public CDialog
{
	DECLARE_DYNAMIC(DlgExitProgress)

public:
	DlgExitProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgExitProgress();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strProgress;
	CProgressCtrl m_Progress_Exit;
	afx_msg void OnBnClickedButtonForceExit();
};
