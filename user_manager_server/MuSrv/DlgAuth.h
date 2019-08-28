#pragma once
#include "afxwin.h"
#include <set>
#include <string>
#include "afxcmn.h"

// DlgAuth �Ի���

class DlgAuth : public CDialog
{
	DECLARE_DYNAMIC(DlgAuth)

public:
	DlgAuth(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgAuth();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboSelectType;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	std::set<std::string> m_nameset;
	CComboBox m_ComboName;
	afx_msg void OnCbnSelchangeComboUsrname();
	CListCtrl m_ListCtrlUsrs;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_UnbindID;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	CString m_strFind;
	afx_msg void OnBnClickedButtonfastfind();
	afx_msg void OnBnClickedButtonOpt();
	CString m_strToDelAcc;
	afx_msg void OnBnClickedButtonDelacc();
};
