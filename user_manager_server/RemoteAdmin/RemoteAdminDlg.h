
// RemoteAdminDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#define DATA_UPDATED (WM_USER+0X100)
#include <set>
#include <string>

// CRemoteAdminDlg �Ի���
class CRemoteAdminDlg : public CDialog
{
// ����
public:
	CRemoteAdminDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTEADMIN_DIALOG };

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
