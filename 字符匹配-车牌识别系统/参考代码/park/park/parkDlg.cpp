
// parkDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "park.h"
#include "parkDlg.h"
#include "afxdialogex.h"

#include "Dbghelp.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/highgui/highgui_c.h>
 
 
using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CparkDlg 对话框



CparkDlg::CparkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CparkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_ListFile);
	DDX_Control(pDX, IDC_STATIC_LISTENSTATUS, m_staticListenStatus);
	DDX_Control(pDX, IDC_IPADDRESS1, m_srvIP);
}

BEGIN_MESSAGE_MAP(CparkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_STARTLISTEN, OnStartListen)
	ON_BN_CLICKED(IDC_OPENCAM, &CparkDlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_GETPIC, &CparkDlg::OnBnClickedGetpic)
	ON_BN_CLICKED(IDC_BUTTON_CLOSECAM, &CparkDlg::OnBnClickedButtonClosecam)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ADDOTHER, &CparkDlg::OnBnClickedButtonAddother)
	ON_MESSAGE(WM_SENDFILELIST, OnSendFileList) //消息映射
	ON_BN_CLICKED(IDC_STARTSRV, &CparkDlg::OnBnClickedStartsrv)
END_MESSAGE_MAP()


// CparkDlg 消息处理程序

BOOL CparkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_srvIP.SetAddress(127, 0, 0, 1);//设置服务器IP
	//初始化文件列表
	int iWidth = 50;
	m_ListFile.InsertColumn(0, "文件名", LVCFMT_LEFT, 4 * iWidth, -1);
	m_ListFile.InsertColumn(1, "文件大小", LVCFMT_LEFT, 2 * iWidth, -1);
	m_ListFile.InsertColumn(2, "文件路径", LVCFMT_LEFT, 6 * iWidth, -1);
	//插入列：左对齐，无子列
	m_ListFile.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CparkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CparkDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CparkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CparkDlg::OnStartListen(WPARAM wParam, LPARAM lParam)
{
	m_staticListenStatus.SetWindowText("服务器正在监听...");
	return 0;
}

Mat srcImage;
Rect roirect;
Point startPoint;
Point endPoint;

int nCloseCam = 0, nGetPic=0;
void CparkDlg::addfile(CString strPathName)
{
	CFile file;
	BOOL bResult = file.Open(strPathName, CFile::modeRead | CFile::shareDenyNone, NULL);
	//打开指定文件
	if (!bResult)//打开不成功
		return; //结束本次循环，继续下次循环
	CString strFileName = file.GetFileName();//得到文件名
	long lFileLength = file.GetLength();
	file.Close();

	CString strFileSize;//显示文件大小
	strFileSize.Format("%0.2fk", (float)lFileLength / 1024);
	//精确到小数点后两位，单位字节

	int iItem = m_ListFile.GetItemCount();//获取文件列表元素个数
	LV_ITEM lvi;//结构体ListView Item，指定或接收属性

	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;
	lvi.lParam = lFileLength;
	lvi.pszText = strFileName.GetBuffer(0);//返回CString中指向字符串的指针
	m_ListFile.InsertItem(&lvi);//插入列表
	m_ListFile.SetItemText(iItem, 1, strFileSize.GetBuffer(0));
	m_ListFile.SetItemText(iItem, 2, strPathName.GetBuffer(0));
}
void CparkDlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	VideoCapture capture(0);
	Mat frame;
	capture >> frame;
	while (capture.isOpened())
	{
		capture >> frame;
		if (nCloseCam) 	break;
		if (nGetPic)
		{
			nGetPic = 0;
			CTime time = CTime::GetCurrentTime();
			CString strPath = time.Format("d:\\test\\车牌%Y-%m-%d %H-%M-%S.jpg");  //Format("%I:%M:%S %p, %A, %B %d, %Y")
			capture.read(srcImage); //获取图像	
			//imshow("srcImage", srcImage); //如果需要可以查看下截取到的图片
			imwrite((String)strPath, srcImage);
			cvWaitKey(500);//延时
			addfile(strPath);
			AfxMessageBox("截取成功");
		}
		imshow("video", frame);
		if (cvWaitKey(20) == 27) //每隔20微秒录制一帧，按ESC键退出
			break;
	}
	capture.release();
	destroyWindow("video");
	nCloseCam = 0;
	 
}
 
void CparkDlg::OnBnClickedGetpic()
{
	// TODO: 在此添加控件通知处理程序代码
	MakeSureDirectoryPathExists("d:\\test\\");
	nGetPic = 1;
}


void CparkDlg::OnBnClickedButtonClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	nCloseCam = 1;
}


void CparkDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	nCloseCam = 1;
	CDialogEx::OnClose();
}


void CparkDlg::OnBnClickedButtonAddother()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, "jpg文件(*.jpg)|*.jpg||", this);
	//创建一个打开对话框，只读|可多选，所有类型，获得当前指针
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();//返回文件列表中第一个文件的位置

		while (pos != NULL)//文件位置存在
		{
			CString strPathName = dlg.GetNextPathName(pos);//获取下一个文件路径
			CFile file;
			BOOL bResult = file.Open(strPathName, CFile::modeRead | CFile::shareDenyNone, NULL);
			//打开指定文件
			if (!bResult)//打开不成功
				continue; //结束本次循环，继续下次循环
			CString strFileName = file.GetFileName();//得到文件名
			long lFileLength = file.GetLength();
			file.Close();

			CString strFileSize;//显示文件大小
			strFileSize.Format("%0.2fk", (float)lFileLength / 1024);
			//精确到小数点后两位，单位字节

			int iItem = m_ListFile.GetItemCount();//获取文件列表元素个数
			LV_ITEM lvi;//结构体ListView Item，指定或接收属性

			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			lvi.iItem = iItem;
			lvi.iSubItem = 0;
			lvi.lParam = lFileLength;
			lvi.pszText = strFileName.GetBuffer(0);//返回CString中指向字符串的指针
			m_ListFile.InsertItem(&lvi);//插入列表
			m_ListFile.SetItemText(iItem, 1, strFileSize.GetBuffer(0));
			m_ListFile.SetItemText(iItem, 2, strPathName.GetBuffer(0));
		}
	}
}



LRESULT CparkDlg::OnSendFileList(WPARAM wParam, LPARAM lParam)
{
	SOCKET sock = *(SOCKET*)wParam;//创建socket
	int iTotal = m_ListFile.GetItemCount();
	MSGFILELIST msgFileList;//结构体，文件列表缓冲区
	for (int i = 0; i < iTotal; i++)
	{
		msgFileList.lFileLength = m_ListFile.GetItemData(i);//文件长度
		strcpy(msgFileList.sServerPath, m_ListFile.GetItemText(i, 2));
		//文件路径
		int iSendCnt = send(sock, (char*)&msgFileList, sizeof(msgFileList), 0);
		//发送至socket
	}
	return 0;
}

void CparkDlg::OnBnClickedStartsrv()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pServer = new CTCPServer(this);
	if (m_pServer != NULL)
	{
		m_pServer->SetListenPort(7000);//设置端口

		BYTE i1, i2, i3, i4;
		m_srvIP.GetAddress(i1, i2, i3, i4);
		char sIP[15];
		sprintf(sIP, "%d.%d.%d.%d", i1, i2, i3, i4);//显示IP地址
		m_pServer->SetSrvIP(sIP);
		m_pServer->ListenRequest();//开始监听
	}
}
