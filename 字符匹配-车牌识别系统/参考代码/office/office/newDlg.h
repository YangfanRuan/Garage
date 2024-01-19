#pragma once
#include "afxdialogex.h"


// newDlg 对话框

class newDlg : public CDialogEx
{
	DECLARE_DYNAMIC(newDlg)

public:
	newDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~newDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OFFICE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
