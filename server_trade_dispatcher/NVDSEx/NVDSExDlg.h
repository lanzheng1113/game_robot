
// NVDSExDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "bran.h"
#include "resource.h"
// CNVDSExDlg �Ի���
class CNVDSExDlg : public CDialog
{
// ����
public:
	CNVDSExDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NVDSEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
