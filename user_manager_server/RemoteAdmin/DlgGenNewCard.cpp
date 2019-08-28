// DlgGenNewCard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteAdmin.h"
#include "DlgGenNewCard.h"
#include "../common/MyUserMgr.h"
#include "../common/MyUtil.h"
#include "DlgShowCreateResult.h"
extern MyUserMgr g_mum;
// DlgGenNewCard �Ի���

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


// DlgGenNewCard ��Ϣ�������

// DlgGenNewCard ��Ϣ�������

void DlgGenNewCard::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_dwGenCount == 0 || m_UseDays == 0){
		MessageBox("��������������������Ϊ0");
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
		MessageBox("ʧ�ܣ��û�����Ϊ�ա�");
		return;
	}

	vret = g_mum.addNewUsr(m_dwGenCount,m_UseDays,std::string((LPCTSTR)ssName));

	if (vret.empty()){
		MessageBox("ʧ�ܣ�");
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
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PreInitDialog();
}

BOOL DlgGenNewCard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CButton* so = (CButton*)GetDlgItem(IDC_RADIO_SelectOld);
	so->SetCheck(BST_CHECKED);
	GetDlgItem(IDC_EDIT_newUser)->EnableWindow(FALSE);
	for (std::set<std::string>::const_iterator it = m_nameset.begin(); it!=m_nameset.end(); ++it){
		m_ComboOldUsrs.AddString(it->c_str());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void DlgGenNewCard::OnBnClickedRadioSelectold()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_newUser)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_OldUsrs)->EnableWindow(TRUE);
}

void DlgGenNewCard::OnBnClickedRadioNewusr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_newUser)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_OldUsrs)->EnableWindow(FALSE);
}
