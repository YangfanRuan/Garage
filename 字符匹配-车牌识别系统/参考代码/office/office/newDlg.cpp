// newDlg.cpp: 实现文件
//

#include "pch.h"
#include "office.h"
#include "afxdialogex.h"
#include "newDlg.h"


// newDlg 对话框

IMPLEMENT_DYNAMIC(newDlg, CDialogEx)

newDlg::newDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OFFICE_DIALOG, pParent)
{

}

newDlg::~newDlg()
{
}

void newDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(newDlg, CDialogEx)
END_MESSAGE_MAP()


// newDlg 消息处理程序
