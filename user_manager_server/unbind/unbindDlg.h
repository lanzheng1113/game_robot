
// unbindDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CunbindDlg �Ի���
class CunbindDlg : public CDialog
{
// ����
public:
	CunbindDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UNBIND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
