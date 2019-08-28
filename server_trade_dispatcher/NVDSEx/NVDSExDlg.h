
// NVDSExDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "bran.h"
#include "resource.h"
// CNVDSExDlg 对话框
class CNVDSExDlg : public CDialog
{
// 构造
public:
	CNVDSExDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NVDSEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void OnRefreshUserList(LPCTSTR ProxyName);
	std::vector<ItemTexts> _items;
public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_ListActiveUsers;
	afx_msg void OnNMDblclkListConnections(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListConnections(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strProxyName;
	afx_msg void OnLvnColumnclickListConnections(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ListShowRecord;
	afx_msg void OnLvnGetdispinfoListRecordShow(NMHDR *pNMHDR, LRESULT *pResult);
	void add_item(const string& item,const string& tradetime,short result_seller,short result_buyer);
};
