// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "NVDSEx.h"
#include "DlgLogin.h"


// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_strAcc(_T(""))
	, m_strPassword(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DataBaseAcc, m_strAcc);
	DDX_Text(pDX, IDC_EDIT_DataBasePassword, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序

void CDlgLogin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
