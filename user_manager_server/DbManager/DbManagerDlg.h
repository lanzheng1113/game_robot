
// DbManagerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <list>
#include <string>

#include "afxwin.h"
// CDbManagerDlg 对话框
class CDbManagerDlg : public CDialog
{
// 构造
public:
	CDbManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DBMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrlDataSet;
	CString m_strSQL;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonexec();
	void saveHistory();
private:
	void loadhistory();
	std::list<std::string> m_vecHistory;
public:
	CComboBox m_ComboHistory;
	afx_msg void OnBnClickedButtonexecedit();
	CString m_strEditorSQL;
};
