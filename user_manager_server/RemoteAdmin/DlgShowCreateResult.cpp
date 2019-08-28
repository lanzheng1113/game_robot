// DlgShowCreateResult.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteAdmin.h"
#include "DlgShowCreateResult.h"


// DlgShowCreateResult 对话框

IMPLEMENT_DYNAMIC(DlgShowCreateResult, CDialog)

DlgShowCreateResult::DlgShowCreateResult(CWnd* pParent /*=NULL*/)
	: CDialog(DlgShowCreateResult::IDD, pParent)
	, m_strResult(_T(""))
{

}

DlgShowCreateResult::~DlgShowCreateResult()
{
}

void DlgShowCreateResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOWRESULT, m_strResult);
}


BEGIN_MESSAGE_MAP(DlgShowCreateResult, CDialog)
	ON_BN_CLICKED(IDOK, &DlgShowCreateResult::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgShowCreateResult 消息处理程序

BOOL DlgShowCreateResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for (int i=0; i!=_vret.size(); i++){
		//m_ListBoxShowResult.InsertString(i,_vret[i]->getAuthId().c_str());
		m_strResult += CString(_vret[i]->getAuthId().c_str());
		m_strResult += "\r\n";
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgShowCreateResult::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
