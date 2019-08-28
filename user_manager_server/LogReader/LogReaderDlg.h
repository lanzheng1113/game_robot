
// LogReaderDlg.h : ͷ�ļ�
//

#pragma once


// CLogReaderDlg �Ի���
class CLogReaderDlg : public CDialog
{
// ����
public:
	CLogReaderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LOGREADER_DIALOG };

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
	CString m_strInfos;
	CString m_strToFind;
	afx_msg void OnBnClickedButtonLoadfile();
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnBnClickedButtonSelectOld();
	afx_msg void OnBnClickedButtonExchange();
};
