#pragma once

#include <vector>
#include "../common/MyNineXUsr.h"
#include "afxwin.h"
// DlgShowCreateResult 对话框

class DlgShowCreateResult : public CDialog
{
	DECLARE_DYNAMIC(DlgShowCreateResult)

public:
	DlgShowCreateResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgShowCreateResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWCREATERESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	std::vector<CMyNineXUsr*> _vret;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strResult;
};
