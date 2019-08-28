
// RemoteAdminDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define DATA_UPDATED (WM_USER+0X100)
#include <set>
#include <string>

// CRemoteAdminDlg 对话框
class CRemoteAdminDlg : public CDialog
{
// 构造
public:
	CRemoteAdminDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REMOTEADMIN_DIALOG };

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
public:
	CListCtrl m_ListCtrlUsers;
	afx_msg void OnBnClickedButtonLogin();
	void parseRecvedUserInfos( char* pUserData, int dataRecvedLen );
	BOOL CreateFileWithBuf( BYTE* readPos, LPCTSTR filename, int size_file );
	CString m_strPswd;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void LoadLocalUserInfo();
	CComboBox m_ComboName;
	std::set<std::string> m_nameset;
	afx_msg void OnCbnSelchangeComboUserselect();
	afx_msg void OnBnClickedButtonnewcard();
	void SyncUserInfoWithServer();
	afx_msg LRESULT OnDataUpdated(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CString m_UnbindID;
};
