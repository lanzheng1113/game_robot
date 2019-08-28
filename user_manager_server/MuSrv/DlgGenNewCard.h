#pragma once
#include "afxwin.h"
#include <set>
#include <string>

// DlgGenNewCard 对话框

class DlgGenNewCard : public CDialog
{
	DECLARE_DYNAMIC(DlgGenNewCard)

public:
	DlgGenNewCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgGenNewCard();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
protected:
	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	CButton m_RadioSelectOldUsrs;
	CComboBox m_ComboOldUsrs;
	CString m_strNewUserName;
	DWORD m_UseDays;
	DWORD m_dwGenCount;
	std::set<std::string> m_nameset;
	afx_msg void OnBnClickedRadioSelectold();
	afx_msg void OnBnClickedRadioNewusr();
};
