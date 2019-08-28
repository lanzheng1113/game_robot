// DlgGenNewCard.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteAdmin.h"
#include "DlgGenNewCard.h"
#include "../common/MyUserMgr.h"
#include "../common/MyUtil.h"
#include "DlgShowCreateResult.h"
extern MyUserMgr g_mum;
// DlgGenNewCard 对话框

IMPLEMENT_DYNAMIC(DlgGenNewCard, CDialog)

DlgGenNewCard::DlgGenNewCard(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGenNewCard::IDD, pParent)
	, m_strNewUserName(_T(""))
	, m_UseDays(0)
	, m_dwGenCount(0)
{

}

DlgGenNewCard::~DlgGenNewCard()
{
}

void DlgGenNewCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_SelectOld, m_RadioSelectOldUsrs);
	DDX_Control(pDX, IDC_COMBO_OldUsrs, m_ComboOldUsrs);
	DDX_Text(pDX, IDC_EDIT_newUser, m_strNewUserName);
	DDX_Text(pDX, IDC_EDIT_KardDays, m_UseDays);
	DDX_Text(pDX, IDC_EDIT_KardCount, m_dwGenCount);
}


BEGIN_MESSAGE_MAP(DlgGenNewCard, CDialog)
	ON_BN_CLICKED(IDOK, &DlgGenNewCard::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SelectOld, &DlgGenNewCard::OnBnClickedRadioSelectold)
	ON_BN_CLICKED(IDC_RADIO_NewUsr, &DlgGenNewCard::OnBnClickedRadioNewusr)
END_MESSAGE_MAP()


// DlgGenNewCard 消息处理程序

// DlgGenNewCard 消息处理程序

void DlgGenNewCard::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_dwGenCount == 0 || m_UseDays == 0){
		MessageBox("天数和生成数量都不能为0");
		return;
	}
	std::vector<CMyNineXUsr*> vret;
	CButton* so = (CButton*)GetDlgItem(IDC_RADIO_SelectOld);
	CString ssName;
	if (so->GetCheck() == BST_CHECKED)
	{
		CString cs;
		m_ComboOldUsrs.GetLBText(m_ComboOldUsrs.GetCurSel(),cs);
		ssName = cs;
	}else{
		ssName = m_strNewUserName;
	}

	if (ssName == ""){
		MessageBox("失败！用户名字为空。");
		return;
	}

	vret = g_mum.addNewUsr(m_dwGenCount,m_UseDays,std::string((LPCTSTR)ssName));

	if (vret.empty()){
		MessageBox("失败！");
		return;
	}else{
//		LogKaiKaResult(vret);
		DlgShowCreateResult d;
		d._vret = vret;
		d.DoModal();
		OnOK();
	}
}

void DlgGenNewCard::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::PreInitDialog();
}

BOOL DlgGenNewCard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton* so = (CButton*)GetDlgItem(IDC_RADIO_SelectOld);
	so->SetCheck(BST_CHECKED);
	GetDlgItem(IDC_EDIT_newUser)->EnableWindow(FALSE);
	for (std::set<std::string>::const_iterator it = m_nameset.begin(); it!=m_nameset.end(); ++it){
		m_ComboOldUsrs.AddString(it->c_str());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgGenNewCard::OnBnClickedRadioSelectold()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_newUser)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_OldUsrs)->EnableWindow(TRUE);
}

void DlgGenNewCard::OnBnClickedRadioNewusr()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_newUser)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_OldUsrs)->EnableWindow(FALSE);
}
