// DlgLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NVDSEx.h"
#include "DlgLogin.h"


// CDlgLogin �Ի���

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


// CDlgLogin ��Ϣ�������

void CDlgLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
