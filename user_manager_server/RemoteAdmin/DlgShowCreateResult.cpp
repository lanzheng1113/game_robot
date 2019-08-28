// DlgShowCreateResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteAdmin.h"
#include "DlgShowCreateResult.h"


// DlgShowCreateResult �Ի���

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


// DlgShowCreateResult ��Ϣ�������

BOOL DlgShowCreateResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for (int i=0; i!=_vret.size(); i++){
		//m_ListBoxShowResult.InsertString(i,_vret[i]->getAuthId().c_str());
		m_strResult += CString(_vret[i]->getAuthId().c_str());
		m_strResult += "\r\n";
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void DlgShowCreateResult::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
