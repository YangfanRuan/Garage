
// parkDlg.h: 头文件
//

#pragma once
#include "CTCPServer.h"

// CparkDlg 对话框
class CparkDlg : public CDialogEx
{
// 构造
public:
	void CparkDlg::addfile(CString strPathName);
	CTCPServer* m_pServer;   //对象指针
	CparkDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARK_DIALOG };
#endif

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
	LRESULT CparkDlg::OnStartListen(WPARAM wParam, LPARAM lParam);
	LRESULT CparkDlg::OnSendFileList(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListFile;
	CStatic m_staticListenStatus;
	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedGetpic();
	afx_msg void OnBnClickedButtonClosecam();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonAddother();
	CIPAddressCtrl m_srvIP;
	afx_msg void OnBnClickedStartsrv();
};
