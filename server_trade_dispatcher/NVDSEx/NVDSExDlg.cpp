
// NVDSExDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CNVDSExDlg 对话框




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


// CNVDSExDlg 消息处理程序

BOOL CNVDSExDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	LONG lStyle = m_ListActiveUsers.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_ListActiveUsers.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);
	
	m_ListActiveUsers.InsertColumn(0,L"序号",0,50);
	m_ListActiveUsers.InsertColumn(1,L"用户ID",0,80);
	m_ListActiveUsers.InsertColumn(2,L"IP",0,120);
	m_ListActiveUsers.InsertColumn(3,L"账号拥有者",0,120);
	m_ListActiveUsers.InsertColumn(4,L"连接时间",0,130);

	m_ListShowRecord.InsertColumn(0,L"序号",0,50);
	m_ListShowRecord.InsertColumn(1,L"订单号",0,120);
	m_ListShowRecord.InsertColumn(2,L"处理时间",0,120);
	m_ListShowRecord.InsertColumn(3,L"结果",0,120);

	m_ListShowRecord.SetItemCount(1000000);

	string strTitle = g_ServerParameterMap["WindowTitle"];
	if (strTitle != string(""))
	{
		TCHAR szWndTitle[200] = {0};
		MultiByteToWideChar(CP_ACP,0,strTitle.c_str(),strTitle.size(),szWndTitle,sizeof(szWndTitle)/sizeof(TCHAR));
		//AfxMessageBox(szWndTitle);
		SetWindowText(szWndTitle);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNVDSExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNVDSExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNVDSExDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MessageBox(L"确定退出？",L"退出程序",MB_YESNO) == IDYES)
	{
		ShutDownServer();
		CDialog::OnClose();
	}
}

BOOL CNVDSExDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加控件通知处理程序代码

	OnRefreshUserList(NULL);
	*pResult = 0;
}

void CNVDSExDlg::OnNMCustomdrawListConnections(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
// 		if(m_ListDbData.GetItemText(nItem,7) == L"禁用")  //设置为红色
// 		{
// 			clrNewTextColor = RGB( 255, 0, 0 );
// 		}
// 		else  //设置为黑色
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
		//设置背景色
		if( nItem%2 ==0 )
		{
			clrNewBkColor = RGB( 240, 240, 240 ); //偶数行背景色为灰色
		}
		else
		{
			clrNewBkColor = RGB( 255, 255, 255 ); //奇数行背景色为白色
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
	//通过传递的参数来得到CSortList对象指针，从而得到排序方式	
	CListCtrl* pV=(CListCtrl*)lParamSort;	
	//通过ItemData来确定数据	
	CString  strItem1 = pV->GetItemText(lParam1,listSortCol);
	CString  strItem2 = pV->GetItemText(lParam2,listSortCol);	
	int iCompRes;	
	switch(listSortCol)		
	{		
	case 0:
	case 1:
		{
			//以第一列为根据排序	
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
	//根据当前的排序方式进行调整
	if(listAsc)	
		return iCompRes;
	else	
		return iCompRes*(-1);
}


void CNVDSExDlg::OnLvnColumnclickListConnections(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if( pNMLV->iSubItem == listSortCol )
		listAsc = !listAsc;
	else	
	{	
		listAsc = true;
		listSortCol= pNMLV->iSubItem;
	}
	//调用排序函数
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
		item_texts._strOrderId = orderId + "[审核]";
		item_texts._strResult = (result_seller == 513) ? "成功" : (string("失败ID=")+String::fromLong(result_seller));
	}else{
		item_texts._strOrderId = orderId;
		if (result_seller == result_seller && result_buyer == 513)
		{
			item_texts._strResult = "成功";
		}else{
			std::stringstream ss;
			ss << "失败:" <<" 卖方结果:" << result_seller << " 买方结果:" << result_buyer;
			item_texts._strResult = ss.str();
		}
	}

	_items.push_back(item_texts);
}

void CNVDSExDlg::OnLvnGetdispinfoListRecordShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	unsigned long iItemIndx= pItem->iItem;
	if (iItemIndx >= _items.size())
	{
		*pResult = 0;
		return;
	}
	
	if (pItem->mask & LVIF_TEXT) //字符串缓冲区有效
	{
		switch(pItem->iSubItem){
		case 0: //填充数据项的名字
			lstrcpy(pItem->pszText,(LPCTSTR)StringToCString(_items[iItemIndx]._strIndexs));
			break;
		case 1: //填充子项1
			lstrcpy(pItem->pszText,(LPCTSTR)StringToCString(_items[iItemIndx]._strOrderId));
			break;
		case 2: //填充子项2
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
