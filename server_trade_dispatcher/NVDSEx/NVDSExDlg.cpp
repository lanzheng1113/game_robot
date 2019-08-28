
// NVDSExDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NVDSEx.h"
#include "NVDSExDlg.h"
#include "testServer.h"
#include "../common/StringTool.h"
#include "util/StringEx.h"
#include "ActiveConnectionInfo.h"

#include <string>
#include <map>
using std::string;
using std::map;
extern map<string,string> g_ServerParameterMap;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CNVDSExDlg �Ի���




CNVDSExDlg::CNVDSExDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNVDSExDlg::IDD, pParent)
	, m_strProxyName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNVDSExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONNECTIONS, m_ListActiveUsers);
	DDX_Control(pDX, IDC_LIST_RECORD_SHOW, m_ListShowRecord);
}

BEGIN_MESSAGE_MAP(CNVDSExDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONNECTIONS, &CNVDSExDlg::OnNMDblclkListConnections)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_CONNECTIONS, &CNVDSExDlg::OnNMCustomdrawListConnections)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CONNECTIONS, &CNVDSExDlg::OnLvnColumnclickListConnections)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RECORD_SHOW, &CNVDSExDlg::OnLvnGetdispinfoListRecordShow)
END_MESSAGE_MAP()


// CNVDSExDlg ��Ϣ�������

BOOL CNVDSExDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	LONG lStyle = m_ListActiveUsers.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_ListActiveUsers.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);
	
	m_ListActiveUsers.InsertColumn(0,L"���",0,50);
	m_ListActiveUsers.InsertColumn(1,L"�û�ID",0,80);
	m_ListActiveUsers.InsertColumn(2,L"IP",0,120);
	m_ListActiveUsers.InsertColumn(3,L"�˺�ӵ����",0,120);
	m_ListActiveUsers.InsertColumn(4,L"����ʱ��",0,130);

	m_ListShowRecord.InsertColumn(0,L"���",0,50);
	m_ListShowRecord.InsertColumn(1,L"������",0,120);
	m_ListShowRecord.InsertColumn(2,L"����ʱ��",0,120);
	m_ListShowRecord.InsertColumn(3,L"���",0,120);

	m_ListShowRecord.SetItemCount(1000000);

	string strTitle = g_ServerParameterMap["WindowTitle"];
	if (strTitle != string(""))
	{
		TCHAR szWndTitle[200] = {0};
		MultiByteToWideChar(CP_ACP,0,strTitle.c_str(),strTitle.size(),szWndTitle,sizeof(szWndTitle)/sizeof(TCHAR));
		//AfxMessageBox(szWndTitle);
		SetWindowText(szWndTitle);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CNVDSExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNVDSExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNVDSExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNVDSExDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(L"ȷ���˳���",L"�˳�����",MB_YESNO) == IDYES)
	{
		ShutDownServer();
		CDialog::OnClose();
	}
}

BOOL CNVDSExDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNVDSExDlg::OnNMDblclkListConnections(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnRefreshUserList(NULL);
	*pResult = 0;
}

void CNVDSExDlg::OnNMCustomdrawListConnections(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor, clrNewBkColor;
 		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		clrNewTextColor = RGB( 0, 0, 0 );
// 		if(m_ListDbData.GetItemText(nItem,7) == L"����")  //����Ϊ��ɫ
// 		{
// 			clrNewTextColor = RGB( 255, 0, 0 );
// 		}
// 		else  //����Ϊ��ɫ
// 		{
// 			COleDateTime oledtDeadLine;
// 			bool bParse = oledtDeadLine.ParseDateTime(m_ListDbData.GetItemText(nItem,3));
// 			if (!bParse){
// 				clrNewTextColor = RGB( 0, 0, 0 );
// 			}
// 			else{
// 				COleDateTime oledtNow = COleDateTime::GetCurrentTime();
// 				if (oledtDeadLine>oledtNow)
// 				{
// 					COleDateTimeSpan span(2,0,0,0);
// 					if (oledtNow + span > oledtDeadLine)
// 					{
// 						clrNewTextColor = RGB(0,0,255);
// 					}
// 					else{
// 						clrNewTextColor = RGB( 0, 0, 0 );
// 					}
// 				}
// 				else{
// 					clrNewTextColor = RGB( 0, 0, 0 );
// 				}
// 			}
// 		}
		//���ñ���ɫ
		if( nItem%2 ==0 )
		{
			clrNewBkColor = RGB( 240, 240, 240 ); //ż���б���ɫΪ��ɫ
		}
		else
		{
			clrNewBkColor = RGB( 255, 255, 255 ); //�����б���ɫΪ��ɫ
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}
}

void CNVDSExDlg::OnRefreshUserList( LPCTSTR ProxyName )
{
	m_ListActiveUsers.DeleteAllItems();
}

static bool listAsc = false;
static int listSortCol = 0;
static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	//ͨ�����ݵĲ������õ�CSortList����ָ�룬�Ӷ��õ�����ʽ	
	CListCtrl* pV=(CListCtrl*)lParamSort;	
	//ͨ��ItemData��ȷ������	
	CString  strItem1 = pV->GetItemText(lParam1,listSortCol);
	CString  strItem2 = pV->GetItemText(lParam2,listSortCol);	
	int iCompRes;	
	switch(listSortCol)		
	{		
	case 0:
	case 1:
		{
			//�Ե�һ��Ϊ��������	
			if(StrToInt((LPCTSTR)strItem1)>StrToInt((LPCTSTR)strItem2))
				iCompRes=1;	
			else
				iCompRes=-1;	
			break;	
		}	
	default:
		iCompRes=strItem1.Compare(strItem2);
		break;

	}
	//���ݵ�ǰ������ʽ���е���
	if(listAsc)	
		return iCompRes;
	else	
		return iCompRes*(-1);
}


void CNVDSExDlg::OnLvnColumnclickListConnections(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( pNMLV->iSubItem == listSortCol )
		listAsc = !listAsc;
	else	
	{	
		listAsc = true;
		listSortCol= pNMLV->iSubItem;
	}
	//����������
	m_ListActiveUsers.SortItems(ListCompare, (DWORD_PTR)&m_ListActiveUsers);   
	int getItem = m_ListActiveUsers.GetItemCount();
	for(int i=0;i<getItem;i++)     
		m_ListActiveUsers.SetItemData(i,i);
	*pResult = 0;
}

void CNVDSExDlg::add_item( const string& orderId,const string& tradetime,short result_seller,short result_buyer)
{
	static int i = 0;
	ItemTexts item_texts;
	item_texts._strIndexs = String::fromLong(i++);
	item_texts._strProcessTime = tradetime;

	if (result_buyer == -2)
	{
		item_texts._strOrderId = orderId + "[���]";
		item_texts._strResult = (result_seller == 513) ? "�ɹ�" : (string("ʧ��ID=")+String::fromLong(result_seller));
	}else{
		item_texts._strOrderId = orderId;
		if (result_seller == result_seller && result_buyer == 513)
		{
			item_texts._strResult = "�ɹ�";
		}else{
			std::stringstream ss;
			ss << "ʧ��:" <<" �������:" << result_seller << " �򷽽��:" << result_buyer;
			item_texts._strResult = ss.str();
		}
	}

	_items.push_back(item_texts);
}

void CNVDSExDlg::OnLvnGetdispinfoListRecordShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	unsigned long iItemIndx= pItem->iItem;
	if (iItemIndx >= _items.size())
	{
		*pResult = 0;
		return;
	}
	
	if (pItem->mask & LVIF_TEXT) //�ַ�����������Ч
	{
		switch(pItem->iSubItem){
		case 0: //��������������
			lstrcpy(pItem->pszText,(LPCTSTR)StringToCString(_items[iItemIndx]._strIndexs));
			break;
		case 1: //�������1
			lstrcpy(pItem->pszText,(LPCTSTR)StringToCString(_items[iItemIndx]._strOrderId));
			break;
		case 2: //�������2
			lstrcpy(pItem->pszText,(LPCTSTR)StringToCString(_items[iItemIndx]._strProcessTime));
			break;
		case 3:
			lstrcpy(pItem->pszText,(LPCTSTR)StringToCString(_items[iItemIndx]._strResult));
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}
