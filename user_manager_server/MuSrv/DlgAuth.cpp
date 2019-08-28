// DlgAuth.cpp : 实现文件
//

#include "stdafx.h"
#include "MuSrv.h"
#include "DlgAuth.h"
#include "../common/MyUserMgr.h"
#include "DlgGenNewCard.h"
// DlgAuth 对话框
extern MyUserMgr g_mum;

IMPLEMENT_DYNAMIC(DlgAuth, CDialog)

DlgAuth::DlgAuth(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAuth::IDD, pParent)
	, m_UnbindID(_T(""))
	, m_strFind(_T(""))
	, m_strToDelAcc(_T(""))
{

}

DlgAuth::~DlgAuth()
{
}

void DlgAuth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ComboSelectType);
	DDX_Control(pDX, IDC_COMBO_UsrName, m_ComboName);
	DDX_Control(pDX, IDC_LIST_KARD, m_ListCtrlUsrs);
	DDX_Text(pDX, IDC_EDIT_UNBINDID, m_UnbindID);
	DDX_Text(pDX, IDC_EDIT2, m_strFind);
	DDX_Text(pDX, IDC_EDIT_AccToDel, m_strToDelAcc);
}


BEGIN_MESSAGE_MAP(DlgAuth, CDialog)
	ON_BN_CLICKED(IDOK, &DlgAuth::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_UsrName, &DlgAuth::OnCbnSelchangeComboUsrname)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgAuth::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &DlgAuth::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &DlgAuth::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &DlgAuth::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTONFASTFIND, &DlgAuth::OnBnClickedButtonfastfind)
	ON_BN_CLICKED(IDC_BUTTON_OPT, &DlgAuth::OnBnClickedButtonOpt)
	ON_BN_CLICKED(IDC_BUTTON_DelAcc, &DlgAuth::OnBnClickedButtonDelacc)
END_MESSAGE_MAP()


// DlgAuth 消息处理程序

void DlgAuth::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL DlgAuth::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboSelectType.AddString("31天");
	m_ComboSelectType.AddString("7天");
	m_ComboSelectType.AddString("3天");
	m_ComboSelectType.SetCurSel(0);
	
	std::vector<CMyNineXUsr*> _usrsCpy = g_mum.getCopy();

	for (int i=0; i!=_usrsCpy.size(); i++){
		m_nameset.insert(_usrsCpy[i]->getName());	
	}
	
	for (std::set<std::string>::const_iterator it = m_nameset.begin(); it!=m_nameset.end();++it){
		m_ComboName.AddString((*it).c_str());
	}
	
	m_ListCtrlUsrs.InsertColumn(0,"计数",0,40);
	m_ListCtrlUsrs.InsertColumn(1,"授权码",0,80);
	m_ListCtrlUsrs.InsertColumn(2,"卡类型",0,50);
	m_ListCtrlUsrs.InsertColumn(3,"硬件ID",0,120);
	m_ListCtrlUsrs.InsertColumn(4,"到期时间",0,100);
	m_ListCtrlUsrs.InsertColumn(5,"最后解绑时间",0,150);
	LONG lStyle = m_ListCtrlUsrs.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_ListCtrlUsrs.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgAuth::OnCbnSelchangeComboUsrname()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cs;
	m_ComboName.GetLBText(m_ComboName.GetCurSel(),cs);
	std::string name = (LPCTSTR)cs;
	std::vector<CMyNineXUsr*> _usrs_with_name;
	std::vector<CMyNineXUsr*> _usrs_Cpy = g_mum.getCopy();
	for (int i=0; i!=_usrs_Cpy.size(); i++)
	{
		if (_usrs_Cpy[i]->getName() == name){
			_usrs_with_name.push_back(_usrs_Cpy[i]);
		}
	}

	m_ListCtrlUsrs.DeleteAllItems();

	for (int i=0; i!=_usrs_with_name.size(); i++)
	{
		CString sText;
		sText.Format("%d",i+1);
		m_ListCtrlUsrs.InsertItem(i,sText);
		sText = _usrs_with_name[i]->getAuthId().c_str();
		m_ListCtrlUsrs.SetItemText(i,1,sText);
		sText = _usrs_with_name[i]->getCardDaysType().c_str();
		m_ListCtrlUsrs.SetItemText(i,2,sText);
		sText = _usrs_with_name[i]->getHarddisk().c_str();
		m_ListCtrlUsrs.SetItemText(i,3,sText);
		sText = _usrs_with_name[i]->getDate_of_end().c_str();
		m_ListCtrlUsrs.SetItemText(i,4,sText);
		sText = _usrs_with_name[i]->getDate_of_last_unbind().c_str();
		m_ListCtrlUsrs.SetItemText(i,5,sText);
	}
}

void DlgAuth::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int OriSel = m_ComboSelectType.GetCurSel();
	DlgGenNewCard d;
	d.m_nameset = m_nameset;
	d.DoModal();
	
	m_ComboName.ResetContent();
	UpdateData(FALSE);
	std::vector<CMyNineXUsr*> xCopy = g_mum.getCopy();
	for (int i=0; i!=xCopy.size(); i++){
		m_nameset.insert(xCopy[i]->getName());	
	}

	for (std::set<std::string>::const_iterator it = m_nameset.begin(); it!=m_nameset.end();++it){
		m_ComboName.AddString((*it).c_str());
	}
}

void DlgAuth::OnBnClickedButton3() //解绑
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_UnbindID == "")
	{
		MessageBox("需要解绑的ID不能为空！");
		return;
	}
	if(g_mum.unbindNoCondition(std::string((LPCTSTR)m_UnbindID)) == 0)
		MessageBox("解绑成功！");
	else
		MessageBox("解绑失败！");
}

void DlgAuth::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_UnbindID == "")
	{
		MessageBox("需要解绑的ID不能为空！");
		return;
	}
	if(g_mum.unbindSub1Days(std::string((LPCTSTR)m_UnbindID),"admin") == 0)
		MessageBox("解绑成功！");
	else
		MessageBox("解绑失败！");
}

void DlgAuth::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_mum.deleteOutDateKards();
}

void DlgAuth::OnBnClickedButtonfastfind()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strFind == ""){
		return;
	}

	int icount = m_ListCtrlUsrs.GetItemCount();
	for (int i = 0; i!= icount ;i++)
	{
		CString strFinded = m_ListCtrlUsrs.GetItemText(i,1);
		if (strFinded == m_strFind)
		{
			m_ListCtrlUsrs.EnsureVisible(i,FALSE);
			m_ListCtrlUsrs.SetItemState(i,LVIS_SELECTED|LVIS_ACTIVATING|LVIS_DROPHILITED , LVIS_SELECTED|LVIS_ACTIVATING|LVIS_DROPHILITED);
			break;
		}
	}
	return;
}

void DlgAuth::OnBnClickedButtonOpt()
{
	// TODO: 在此添加控件通知处理程序代码
	g_mum.addfivedaysall();
}

void DlgAuth::OnBnClickedButtonDelacc()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strToDelAcc != "")
	{
		g_mum.deleteAcc(m_strToDelAcc);
	}
}
