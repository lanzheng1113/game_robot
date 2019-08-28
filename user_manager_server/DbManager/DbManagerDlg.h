
// DbManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <list>
#include <string>

#include "afxwin.h"
// CDbManagerDlg �Ի���
class CDbManagerDlg : public CDialog
{
// ����
public:
	CDbManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DBMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
