#pragma once
#include "Resource.h"

// DlgExitProgress �Ի���

class DlgExitProgress : public CDialog
{
	DECLARE_DYNAMIC(DlgExitProgress)

public:
	DlgExitProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgExitProgress();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strProgress;
	CProgressCtrl m_Progress_Exit;
	afx_msg void OnBnClickedButtonForceExit();
};
