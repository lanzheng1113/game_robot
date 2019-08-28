
// recordreaderDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include <vector>
#include "afxwin.h"
struct recordsInfo 
{
	std::string name;
	std::string dateTime;
	int days;
	int count;
	std::vector<std::string> KaiKaList;
};

// CrecordreaderDlg �Ի���
class CrecordreaderDlg : public CDialog
{
// ����
public:
	CrecordreaderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RECORDREADER_DIALOG };

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
	void OnRefresh();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenfile();
	std::vector<recordsInfo> m_recs;
	CListBox m_ListBoxAll;
	CListBox m_ListDetail;
	CButton m_Check1;
	afx_msg void OnBnClickedButton3();
	DWORD m_dwKaiKaSum;
	CComboBox m_combo_kaikasub;
	afx_msg void OnBnClickedButtonsum();
};
