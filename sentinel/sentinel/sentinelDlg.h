
// sentinelDlg.h : ͷ�ļ�
//

#pragma once

class TaskWatch;
// CsentinelDlg �Ի���
class CsentinelDlg : public CDialog
{
// ����
public:
	CsentinelDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SENTINEL_DIALOG };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strItem1;
	CString m_strItem2;
	CString m_strItem3;
	CString m_strItem4;
	CString m_strItem5;
	CString m_strItem6;
private:
	TaskWatch* m_taskWatch;
	void ShowTaskMap();
public:
	afx_msg void OnBnClickedButtonExecute();
	void OnLBDoubleClickTextEdit1();
	void OnLBDoubleClickTextEdit2();
	void OnLBDoubleClickTextEdit3();
	void OnLBDoubleClickTextEdit4();
	void OnLBDoubleClickTextEdit5();
	void OnLBDoubleClickTextEdit6();

	void OnPressReturnOnWndNameEdit1();
	void OnPressReturnOnWndNameEdit2();
	void OnPressReturnOnWndNameEdit3();
	void OnPressReturnOnWndNameEdit4();
	void OnPressReturnOnWndNameEdit5();
	void OnPressReturnOnWndNameEdit6();
};
