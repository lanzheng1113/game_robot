#pragma once

#include <vector>
#include "../common/MyNineXUsr.h"
#include "afxwin.h"
// DlgShowCreateResult �Ի���

class DlgShowCreateResult : public CDialog
{
	DECLARE_DYNAMIC(DlgShowCreateResult)

public:
	DlgShowCreateResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgShowCreateResult();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOWCREATERESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	std::vector<CMyNineXUsr*> _vret;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strResult;
};
