
// officeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "office.h"
#include "officeDlg.h"
#include "afxdialogex.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2/imgproc/imgproc_c.h"
#include "CvvImage.h"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEBUGTEST
#define POINT_X  18                                           // 水平方向不重叠点                        
#define POINT_Y  2                                            // 垂直方向不重叠点                            
#define WITH_X 0.1                                            // 水平方向车牌在图区域     
#define WITH_Y 0.2                                            // 垂直方向车牌在图区域     
#define HIGH_WITH_CAR   4.8   //(440/85)       // 小车的宽高比  440/140    
#define CHARACTER 15
#define TEMPLETENUM  43


 
// 车牌字符模板特征值
const int Num_Templete[TEMPLETENUM][CHARACTER] =
{
  {16,19,10,12,10,10,15,18,110,3,2,2,3,3,3},     //0
  {9,11,10,10,10,10,9,10,79,2,2,2,0,2,12},       //1
  {18,19,3,18,10,10,23,22,123,4,2,2,7,6,8},      //2
  {19,21,11,14,4,20,18,22,129,2,2,4,6,6,7},      //3
  {2,18,11,22,20,21,11,18,123,2,4,2,6,7,5},      //4
  {23,19,20,12,9,20,18,22,143,2,4,4,6,6,6},      //5
  {6,13,17,8,15,20,18,20,117,2,2,4,5,7,6},       //6
  {21,21,0,20,8,12,9,11,102,2,2,2,2,8,15},       //7
  {17,18,18,19,14,20,17,20,143,4,2,4,6,6,6},     //8
  {16,18,15,21,7,19,13,7,116,3,2,2,6,6,5},       //9
  {10,10,16,16,20,20,18,19,129,2,4,2,8,3,6},     //A
  {24,20,20,19,22,22,24,20,171,4,8,4,6,6,6},     //B
  {18,19,20,4,20,8,17,21,127,3,2,4,4,4,4},       //C
  {23,19,11,20,12,20,22,21,148,3,3,3,4,4,4},     //D
  {23,19,21,9,22,8,23,23,148,2,2,2,6,6,6},       //E
  {25,17,20,9,22,8,19,0,120,2,2,2,4,4,4},        //F
  {17,18,22,14,12,24,18,21,146,4,7,4,4,6,6},     //G
  {14,20,18,22,17,22,16,20,149,4,1,4,2,2,2},     //H
  {0,17,0,20,3,20,18,22,100,2,2,4,2,2,2},        //J
  {19,20,26,10,20,20,20,22,157,4,4,4,3,5,11},    //K
  {20,0,20,0,20,0,25,20,105,2,2,2,2,2,2},        //L
  {20,10,27,17,20,10,22,14,140,1,3,3,4,1,5},     //M
  {21,12,25,17,26,12,18,18,149,3,5,3,5,5,6},     //N 
  {23,19,18,20,21,8,22,0,131,3,3,2,4,4,4},       //P
  {18,19,20,10,26,15,18,21,147,3,3,4,5,7,5},     //Q
  {26,19,21,18,21,17,20,21,163,4,3,4,4,6,5},     //R
  {18,18,18,10,8,17,17,22,128,4,3,4,6,6,6},      //S
  {22,18,10,10,10,10,10,10,100,2,2,2,33,2,2},    //T
  {18,12,20,10,20,10,19,21,130,3,3,3,2,2,2},     //U
  {20,19,20,20,15,14,9,10,127,4,4,2,9,1,8},      //V
  {21,25,26,28,16,16,21,19,172,6,2,4,13,0,7},    //W
  {21,21,13,13,12,11,22,21,134,4,2,4,8,0,10},    //X
  {21,20,10,11,10,10,10,11,103,3,2,2,5,2,6},     //Y
  {21,23,5,15,15,5,24,20,128,2,2,2,8,8,7},       //Z
  {13,14,10,10,10,10,13,13,93,2,2,2,29,2,29},    //I
  {20,20,13,20,19,12,17,20,141,3,3,4,4,4,4},     //O          //36
  {14,15,17,17,16,10,25,24,138,0,2,4,12,8,9},    //云        //37
  {17,20,17,12,33,28,23,20,170,3,4,7,13,6,4},    //苏
  {21,21,23,24,24,25,31,27,196,0,9,6,8,6,7},     //京
  {19,27,20,34,19,36,24,37,216,4,4,7,13,28,3},   //湘
  {17,14,23,27,36,40,26,27,210,4,13,4,16,14,14}, //鲁
  {24,24,32,38,34,32,17,22,223,9,6,10,11,12,9}, // 粤
  {22,20,33,37,25,24,24,25,210,13,3,6,12,8,7}     //蒙
};

//车牌字符
char *PlateCode[TEMPLETENUM] =
{
"0", "1", "2", "3", "4" ,
"5","6", "7", "8", "9",
"A", "B", "C", "D","E",
"F", "G","H", "J", "K",
"L", "M", "N","P", "Q",
"R", "S", "T", "U", "V",
"W","X", "Y", "Z", "I", "O",
 "云", "苏","京", "湘", "鲁","粤","蒙"
};
char *G_PlateChar[7] = { "没", "有","找", "到", "车", "牌" ,"!" }; // 车牌号



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


// CofficeDlg 对话框



CofficeDlg::CofficeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OFFICE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CofficeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPCtrl);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_ListFile);
}

BEGIN_MESSAGE_MAP(CofficeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPENPIC, &CofficeDlg::OnBnClickedOpenpic)
	ON_BN_CLICKED(IDC_PICBIN, &CofficeDlg::OnBnClickedPicbin)
	ON_BN_CLICKED(IDC_LOCPIC, &CofficeDlg::OnBnClickedLocpic)
	ON_BN_CLICKED(IDC_SPITCHAR, &CofficeDlg::OnBnClickedSpitchar)
	ON_BN_CLICKED(IDC_IDCHAR, &CofficeDlg::OnBnClickedIdchar)
	ON_BN_CLICKED(IDC_CONNECT_SRV, &CofficeDlg::OnBnClickedConnectSrv)
	ON_MESSAGE(WM_RECVFILELIST, OnFileList)
	ON_MESSAGE(WM_FILESTATUS, OnFileStatus)
	ON_MESSAGE(WM_CONNECTERROR, OnConnectError)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILELIST, &CofficeDlg::OnNMDblclkListFilelist)
END_MESSAGE_MAP()


// CofficeDlg 消息处理程序

BOOL CofficeDlg::OnInitDialog()
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
	m_IPCtrl.SetAddress(127, 0, 0, 1);//设置服务器IP
	int iWidth = 50;
	m_ListFile.InsertColumn(0, "文件名", LVCFMT_LEFT, 3 * iWidth, -1);
	m_ListFile.InsertColumn(1, "文件尺寸", LVCFMT_LEFT, 2 * iWidth, -1);
	m_ListFile.InsertColumn(2, "下载状态", LVCFMT_LEFT, 2 * iWidth, -1);
	m_ListFile.InsertColumn(3, "下载速率", LVCFMT_LEFT, 2 * iWidth, -1);//速度列
	m_ListFile.InsertColumn(4, "下载时间", LVCFMT_LEFT, 2 * iWidth, -1);
	m_ListFile.InsertColumn(5, "服务器路径", LVCFMT_LEFT, 3 * iWidth, -1);//服务器路径
	m_ListFile.InsertColumn(6, "Start Tic", LVCFMT_LEFT, 2 * iWidth, -1);//开始记号
	m_ListFile.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_pClient = new CTCPClient(this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CofficeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CofficeDlg::OnPaint()
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
HCURSOR CofficeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CofficeDlg::OnBnClickedOpenpic()
{
	// TODO: 在此添加控件通知处理程序代码
	src = NULL;
	CString filePath;
	CFileDialog dlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||", NULL);
	char title[] = { "Open Image" };
	dlg.m_ofn.lpstrTitle = title;
	if (dlg.DoModal() == IDOK) {
		filePath = dlg.GetPathName();
		//LoadBmpFile(filePath);
		// src=cvLoadImage(filePath,1);
		Mat gmat;
		gmat = imread((String)filePath, IMREAD_COLOR);
		imshow("src", gmat);
		IplImage* transIplimage = cvCloneImage(&cvIplImage(gmat));
		src = cvCloneImage(transIplimage);
		pImgCanny = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);     //  2值化后图片大小初始化
		cvCvtColor(src, pImgCanny, CV_RGB2GRAY);  //转化为灰度图  openCV函数 Y = 0.299*R + 0.587*G + 0.114*B                                                     //  转为灰度图	  Y=0.299*R + 0.587*G + 0.114*B
		cvSmooth(pImgCanny, pImgCanny, CV_GAUSSIAN, 3, 0, 0);   //平滑高斯滤波  滤波后的图片保存在 pImgCanny    
	}   
}
 
int CofficeDlg::AdaptiveThreshold(int t, IplImage *Image)   
{
	int t1 = 0, t2 = 0, tnew = 0, i = 0, j = 0;
	int Allt1 = 0, Allt2 = 0, accountt1 = 0, accountt2 = 0; 
	for (j = 0; j < Image->height; j++) 
	{
		for (i = 0; i < Image->width; i++)
		{
			if (CV_IMAGE_ELEM(Image, uchar, j, i) < t)
			{
				Allt1 += CV_IMAGE_ELEM(Image, uchar, j, i);
				accountt1++;
			}
			else
			{
				Allt2 += CV_IMAGE_ELEM(Image, uchar, j, i);
				accountt2++;
			}
		}
	}
	t1 = Allt1 / accountt1;
	t2 = Allt2 / accountt2;
	tnew = 0.5*(t1 + t2);
	if (tnew == t)  
		return tnew;
	else
		AdaptiveThreshold(tnew, Image);  
}

void CofficeDlg::Threshold(IplImage *Image, IplImage *Image_O)
{
	int thresMax = 0, thresMin = 255, i = 0, j = 0, t = 0;
	for (j = 0; j < Image->height; j++)
		for (i = 0; i < Image->width; i++)
		{
			if (CV_IMAGE_ELEM(Image, uchar, j, i) > thresMax)  
				thresMax = CV_IMAGE_ELEM(Image, uchar, j, i);   
			else if (CV_IMAGE_ELEM(Image, uchar, j, i) < thresMin)   
				thresMin = CV_IMAGE_ELEM(Image, uchar, j, i);  
		}
	cvCanny(Image, Image_O, AdaptiveThreshold((thresMax + thresMin)*0.5, Image), thresMax*0.7, 3);
}


//把图像显示在MFC图片控件里
void CofficeDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;

	cimg.CopyOf(img, 3);
	cimg.DrawToHDC(hDC, &rect);

	ReleaseDC(pDC);
}



void CofficeDlg::OnBnClickedPicbin()
{
	// TODO: 在此添加控件通知处理程序代码
	Threshold(pImgCanny, pImgCanny);
	DrawPicToHDC(pImgCanny, IDC_BIIMAGE);
}


 
int CofficeDlg::PlateAreaSearch(IplImage *pImg_Image)
{
	if (pImg_Image == NULL) { return 0; }  

	IplImage* imgTest = 0;
	int i = 0, j = 0, k = 0, m = 0;
	bool flag = 0;
	int plate_n = 0, plate_s = 0, plate_e = 0, plate_w = 0;   

	int *num_h = new int[max(pImg_Image->width, pImg_Image->height)];
	if (num_h == NULL)
	{
		MessageBox("memory exhausted!");
		return 0;	
	}   

	for (i = 0; i < pImg_Image->width; i++) { num_h[i] = 0; }  

	imgTest = cvCreateImage(cvSize(pImg_Image->width, pImg_Image->height), IPL_DEPTH_8U, 1);
	cvCopy(pImg_Image, imgTest);

 
	for (j = 0; j < imgTest->height; j++)
	{
		for (i = 0; i < imgTest->width - 1; i++)
		{
			CV_IMAGE_ELEM(imgTest, uchar, j, i) = CV_IMAGE_ELEM(imgTest, uchar, j, i + 1) - CV_IMAGE_ELEM(imgTest, uchar, j, i);
			num_h[j] += CV_IMAGE_ELEM(imgTest, uchar, j, i) / 250;
		}
	}


	int temp_1 = 0;
	int temp_max = 0;
	int temp_i = 0;  
	for (j = 0; j < imgTest->height - 20; j++)
	{
		temp_1 = 0;
		for (i = 0; i < 20; i++) 
			temp_1 += num_h[i + j];
		if (temp_1 >= temp_max)
		{
			temp_max = temp_1;
			temp_i = j; 
		}
	}
	k = temp_i; 
	while (((num_h[k + 1] > POINT_X) || (num_h[k + 2] > POINT_X) || (num_h[k] > POINT_X)) && k) k--; 
	plate_n = k + 1; 
	k = temp_i + 10;
	while (((num_h[k - 1] > POINT_X) || (num_h[k - 2] > POINT_X) || (num_h[k] > POINT_X)) && (k < imgTest->height)) k++;  
	plate_s = k; 

	 
	if (!(plate_n && plate_s   
		&& (plate_n < plate_s) && ((plate_s - plate_n)*HIGH_WITH_CAR < imgTest->width*(1 - WITH_X))))
	{
		//flag=1;
	//	cout<<"水平分割失败"<<endl;
		MessageBox("水平分割失败!");
		return 0;
	}
	else 
	{
		int  max_count = 0;
		int  plate_length = (imgTest->width - (plate_s - plate_n)*HIGH_WITH_CAR); 
		plate_w = imgTest->width*WITH_X - 1; 

	 
		for (i = 0; i < imgTest->width; i++)
			for (j = 0; j < imgTest->height - 1; j++) 
			{
				CV_IMAGE_ELEM(imgTest, uchar, j, i) = CV_IMAGE_ELEM(imgTest, uchar, j + 1, i) - CV_IMAGE_ELEM(imgTest, uchar, j, i);
			}
 
		for (k = 0; k < plate_length; k++)
		{
			for (i = 0; i < (int)((plate_s - plate_n)*HIGH_WITH_CAR); i++)
				for (j = plate_n; j < plate_s; j++)//两水平线之间 
				{
					num_h[k] = num_h[k] + CV_IMAGE_ELEM(imgTest, uchar, j, (i + k)) / 250;
				}
			if (num_h[k] > max_count)
			{
				max_count = num_h[k];
				plate_w = k;
			}  // end if 

		}


		CvRect ROI_rect;                  
		ROI_rect.x = plate_w;
		ROI_rect.y = plate_n;
		ROI_rect.width = (plate_s - plate_n)*HIGH_WITH_CAR;
		ROI_rect.height = plate_s - plate_n;

		if ((ROI_rect.width + ROI_rect.x) > pImg_Image->width)
		{
			ROI_rect.width = pImg_Image->width - ROI_rect.x;
	    
			MessageBox("垂直方向分割失败!");
			return 0;
		}
		else
		{

			IplImage *pImg8uROI = NULL;         
			pImg8uROI = cvCreateImage(cvSize(ROI_rect.width, ROI_rect.height), src->depth, src->nChannels);

			IplImage *pImg8u11 = NULL;        
			pImg8u11 = cvCreateImage(cvSize(40 * HIGH_WITH_CAR, 40), pImg8uROI->depth, pImg8uROI->nChannels);

			cvSetImageROI(src, ROI_rect); 
			cvCopy(src, pImg8uROI, NULL); 
			cvResetImageROI(src);   

			pImgResize = cvCreateImage(cvSize(40 * HIGH_WITH_CAR, 40), IPL_DEPTH_8U, 1);
			cvResize(pImg8uROI, pImg8u11, CV_INTER_LINEAR); 

			cvCvtColor(pImg8u11, pImgResize, CV_RGB2GRAY);  
			Threshold(pImgResize, pImgResize);             

			cvReleaseImage(&pImg8uROI);
			cvReleaseImage(&pImg8u11);
			cvReleaseImage(&imgTest);
		}   // end if 
	}    // end if 

 
	delete[]num_h;
	num_h = NULL;
	return 1;
}
void CofficeDlg::OnBnClickedLocpic()
{
	// TODO: 在此添加控件通知处理程序代码
	PlateAreaSearch(pImgCanny);    //  车牌定位
	DrawPicToHDC(pImgResize, IDC_BICAR);  //把定位好的车牌显示出来
}

 
int CofficeDlg::SegmentPlate()
{
	if (pImgResize == NULL) { return 0; }  

	int *num_h = new int[max(pImgResize->width, pImgResize->height)];	 
	if (num_h == NULL)   
	{
		MessageBox("字符分割memory exhausted");
		return 0;
	}  // end if 
	int i = 0, j = 0, k = 0; 
	int  letter[14] = { 0,20,23,43,55,75,78,98,101,121,124,127,147,167 };  
	bool flag1 = 0; 
 
	for (i = 0; i < 40 * HIGH_WITH_CAR; i++)
	{
		num_h[i] = 0;  
		for (j = 0; j < 17; j++)   
		{
			num_h[i] += CV_IMAGE_ELEM(pImgResize, uchar, j, i) / 45;
		}
		for (j = 24; j < 40; j++)  
		{
			num_h[i] += CV_IMAGE_ELEM(pImgResize, uchar, j, i) / 45;
		}
	}
 
	int	max_count = 0;
	int   flag = 0;
	for (i = 30; i < 40 * HIGH_WITH_CAR; i++)
	{
		if (num_h[i] < POINT_Y) 
		{
			max_count++;
			if (max_count == 11)
			{
				letter[3] = i - 11;  
				while ((num_h[i] < POINT_Y) || (num_h[i - 1] < POINT_Y)) i++;
				letter[4] = i - 1; 
				break; 
			}
		}
		else
		{
			max_count = 0;
		}
	}
 
	for (i = 0; i < 40 * HIGH_WITH_CAR; i++) 
	{
		for (j = 17; j <= 24; j++)  
		{
			num_h[i] += CV_IMAGE_ELEM(pImgResize, uchar, j, i) / 45;
		}
	}

	for (j = letter[3]; j > 0; j--) 
	{
		if ((num_h[j] < POINT_Y) && (num_h[j - 1] < POINT_Y)) 
		{                                          
			letter[2] = j;      
			letter[1] = (j >= 23) ? j - 3 : letter[1];   
			letter[0] = (j >= 23) ? j - 23 : letter[0];  
			break;       
		}
	}

	j = 2;  flag = 0; flag1 = 0; 
	for (i = letter[4]; i < 40 * HIGH_WITH_CAR; i++)   
	{
		if ((num_h[i] > POINT_Y) && (num_h[i - 1] > POINT_Y) && !flag)
		{
			flag = 1;
			flag1 = 0;
			letter[2 * j] = i - 1; 
			if (j == 6)   
			{
				letter[2 * j + 1] = ((letter[2 * j] + 20) > 40 * HIGH_WITH_CAR - 1) ? 40 * HIGH_WITH_CAR - 1 : letter[2 * j] + 20;
				break; 
			}
		}
		else if ((num_h[i] < POINT_Y) && (num_h[i - 1] < POINT_Y) && !flag1 && flag) 
		{
			flag = 0;
			flag1 = 1;
			letter[2 * j + 1] = i - 1;
			j++;  
		}                      
	}
                    
	for (i = 0; i < 40 * HIGH_WITH_CAR - 1; i++)
	{	                              

		for (j = 0; j < 39; j++)  
		{

			if (CV_IMAGE_ELEM(pImgResize, uchar, j, i) && CV_IMAGE_ELEM(pImgResize, uchar, j, i + 1) && CV_IMAGE_ELEM(pImgResize, uchar, j + 1, i)) //  01
				CV_IMAGE_ELEM(pImgResize, uchar, j, i) = 0;                                                                                                                                                                                                                             //   1

			if (CV_IMAGE_ELEM(pImgResize, uchar, j, i) && CV_IMAGE_ELEM(pImgResize, uchar, j, i - 1) && CV_IMAGE_ELEM(pImgResize, uchar, j + 1, i))     //  10
				CV_IMAGE_ELEM(pImgResize, uchar, j, i) = 0;                                                                                                                                                                                                                               //    1          			

			if (CV_IMAGE_ELEM(pImgResize, uchar, j, i) && CV_IMAGE_ELEM(pImgResize, uchar, j, i - 1) && CV_IMAGE_ELEM(pImgResize, uchar, j - 1, i))         //   1      
				CV_IMAGE_ELEM(pImgResize, uchar, j, i) = 0;                                                                                    // 10
			if (CV_IMAGE_ELEM(pImgResize, uchar, j, i) && CV_IMAGE_ELEM(pImgResize, uchar, j, i + 1) && CV_IMAGE_ELEM(pImgResize, uchar, j - 1, i))           // 1      
				CV_IMAGE_ELEM(pImgResize, uchar, j, i) = 0;                                                                                 // 01
		}
	}

 
	pImgCharOne = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
	pImgCharTwo = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
	pImgCharThree = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
	pImgCharFour = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
	pImgCharFive = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
	pImgCharSix = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
	pImgCharSeven = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);

	CvRect ROI_rect1;
	ROI_rect1.x = 0.5*(letter[1] + letter[0]) - 10; 
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharOne, NULL);  
	cvResetImageROI(pImgResize);

	ROI_rect1.x = 0.5*(letter[3] + letter[2]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharTwo, NULL);  
	cvResetImageROI(pImgResize);

	ROI_rect1.x = 0.5*(letter[5] + letter[4]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharThree, NULL);  
	cvResetImageROI(pImgResize);

	ROI_rect1.x = 0.5*(letter[7] + letter[6]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharFour, NULL);  
	cvResetImageROI(pImgResize);

	ROI_rect1.x = 0.5*(letter[9] + letter[8]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharFive, NULL);  
	cvResetImageROI(pImgResize);

	ROI_rect1.x = 0.5*(letter[11] + letter[10]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharSix, NULL);  
	cvResetImageROI(pImgResize);

	ROI_rect1.x = 0.5*(letter[13] + letter[12]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	cvSetImageROI(pImgResize, ROI_rect1);
	cvCopy(pImgResize, pImgCharSeven, NULL);  
	cvResetImageROI(pImgResize);
 
	delete[]num_h;
	num_h = NULL;

}


void CofficeDlg::OnBnClickedSpitchar()
{
	// TODO: 在此添加控件通知处理程序代码
	SegmentPlate();              //   车牌字符分割
	DrawPicToHDC(pImgCharOne, IDC_ONE);
	DrawPicToHDC(pImgCharTwo, IDC_TWO);
	DrawPicToHDC(pImgCharThree, IDC_THREE);
	DrawPicToHDC(pImgCharFour, IDC_FOUR);
	DrawPicToHDC(pImgCharFive, IDC_FIVE);
	DrawPicToHDC(pImgCharSix, IDC_SIX);
	DrawPicToHDC(pImgCharSeven, IDC_SEVEN);
}

 
int CofficeDlg::CodeRecognize(IplImage *imgTest, int num, int char_num)
{
	if (imgTest == NULL) { return 0; }

	int i = 0, j = 0, k = 0, t = 0; 
	int  char_start = 0, char_end = 0; 
	int num_t[CHARACTER] = { 0 };


	switch (num) 
	{
	case 0:  char_start = 0;        
		char_end = 9;
		break;
	case 1:  char_start = 10;         
		char_end = 35;
		break;
	case 2:  char_start = 0;       
		char_end = 35;
		break;
	case 3:  char_start = 36;       
		char_end = TEMPLETENUM - 1;
		break;
	default: break;
	}
 
	for (k = 0; k < 8; k++)
	{
		for (j = int(k / 2) * 10; j<int(k / 2 + 1) * 10; j++)
		{
			for (i = (k % 2) * 10; i < (k % 2 + 1) * 10; i++)
			{
				num_t[k] += CV_IMAGE_ELEM(imgTest, uchar, j, i) / 255;
			}
		}
		num_t[8] += num_t[k];   
	}

	for (i = 0; i < 20; i++)   
		num_t[9] += CV_IMAGE_ELEM(imgTest, uchar, 10, i) / 255;
	for (i = 0; i < 20; i++)
		num_t[10] += CV_IMAGE_ELEM(imgTest, uchar, 20, i) / 255;
	for (i = 0; i < 20; i++)
		num_t[11] += CV_IMAGE_ELEM(imgTest, uchar, 30, i) / 255;

	for (j = 0; j < 40; j++)
		num_t[12] += CV_IMAGE_ELEM(imgTest, uchar, j, 7) / 255;
	for (j = 0; j < 40; j++)
		num_t[13] += CV_IMAGE_ELEM(imgTest, uchar, j, 10) / 255;
	for (j = 0; j < 40; j++)
		num_t[14] += CV_IMAGE_ELEM(imgTest, uchar, j, 13) / 255;

	int num_tt[CHARACTER] = { 0 };
	int matchnum = 0;  
	int matchnum_max = 0;
	int matchcode = 0;         
 

	j = 0;

	for (k = char_start; k <= char_end; k++)
	{
		matchnum = 0;

		for (i = 0; i < 8; i++)  
		{
			if (abs(num_t[i] - Num_Templete[k][i]) <= 2) 
				matchnum++; 
		}

		if (Num_Templete[k][i] - abs(num_t[i]) <= 8) 
			matchnum += 2;
		for (i = 9; i < CHARACTER; i++)   
		{
			if (Num_Templete[k][i] >= 5)   
			{
				if (abs(num_t[i] - Num_Templete[k][i]) <= 1)
					matchnum += 2;
			}
			else if (num_t[i] == Num_Templete[k][i])
			{
				matchnum += 2;
			}
		}
		if (matchnum > matchnum_max)
		{
			matchnum_max = matchnum;  
			matchcode = k;  
		}
	}
	G_PlateChar[char_num] = PlateCode[matchcode];  
}



void CofficeDlg::OnBnClickedIdchar()
{
	// TODO: 在此添加控件通知处理程序代码
		//---------------------- 车牌识别 -----------------------------//
	CodeRecognize(pImgCharOne, 3, 0);
	CodeRecognize(pImgCharTwo, 1, 1);
	CodeRecognize(pImgCharThree, 2, 2);
	CodeRecognize(pImgCharFour, 2, 3);
	CodeRecognize(pImgCharFive, 0, 4);
	CodeRecognize(pImgCharSix, 0, 5);
	CodeRecognize(pImgCharSeven, 0, 6);

	CString outFile = "";
	int i;
	for (i = 0; i < 7; i++) //把结果放到 outFile CString 里
	{
		outFile += G_PlateChar[i];
	}
	GetDlgItem(IDC_RESULT)->SetWindowText(outFile);
}




void CofficeDlg::OnBnClickedConnectSrv()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	BYTE i1, i2, i3, i4;
	m_IPCtrl.GetAddress(i1, i2, i3, i4);

	char sIP[15];
	sprintf(sIP, "%d.%d.%d.%d", i1, i2, i3, i4);//显示IP地址

	m_ListFile.DeleteAllItems();//文件列表，清除所有元素

	m_pClient->SetServerIP(sIP);
	m_pClient->SetServerPort(7000);

	MSGREQUEST msgRequest;
	msgRequest.iCommand = FILELIST;//请求文件列表
	msgRequest.lFileOffset = 0;
	msgRequest.lFileLength = 0;
	strcpy(msgRequest.sClientPath, "\0");
	strcpy(msgRequest.sServerPath, "\0");
	m_pClient->RequestFile(msgRequest);//调用请求线程
}



LRESULT CofficeDlg::OnFileList(WPARAM wParam, LPARAM lParam)
{
	MSGFILELIST* pmsgList = (MSGFILELIST*)wParam;
	int iItem = m_ListFile.GetItemCount();
	CString strServerPath, strFileName, strFileSize;
	strServerPath.Format("%s", pmsgList->sServerPath);
	int iPos = strServerPath.ReverseFind('\\');
	//Searches this CString object for the last match of a substring.
	//设置文件名到缓冲区
	strFileName = strServerPath.Right(strServerPath.GetLength() - iPos - 1);

	strFileSize.Format("%0.2fk", (float)pmsgList->lFileLength / 1024);

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;
	lvi.lParam = pmsgList->lFileLength;
	//显示
	lvi.pszText = strFileName.GetBuffer(0);
	m_ListFile.InsertItem(&lvi);
	m_ListFile.SetItemText(iItem, 1, strFileSize.GetBuffer(0));
	m_ListFile.SetItemText(iItem, 2, "0.00%");
	m_ListFile.SetItemText(iItem, 3, "0k/s");
	m_ListFile.SetItemText(iItem, 4, "0s");
	m_ListFile.SetItemText(iItem, 5, strServerPath.GetBuffer(0));
	m_ListFile.SetItemText(iItem, 6, "0");
	return 0;
}


LRESULT CofficeDlg::OnFileStatus(WPARAM wParam, LPARAM lParam)
{
	MSGFILESTATUS* pmsg = (MSGFILESTATUS*)wParam;
	CString strPath;
	strPath.Format("%s", pmsg->sServerPath);
	int iTotal = m_ListFile.GetItemCount();
	int iSel = 0;
	for(; iSel<iTotal; iSel++)
	{
		CString strListPath;
		strListPath = m_ListFile.GetItemText(iSel, 5);
		if(strPath.Compare(strListPath) == 0)//路径相同
			break;
	}
	if(iSel == iTotal)
		return 1;

	long lFileLength = m_ListFile.GetItemData(iSel);

	CString strStatus;
	strStatus.Format("%0.2f%%", (float)pmsg->lFileOffset/lFileLength*100);
	//显示状态 %,精确到小数点后两位
	m_ListFile.SetItemText(iSel, 2, strStatus.GetBuffer(0));

	//显示时间
	DWORD dwNowTick, dwStartTick;//毫秒
	dwNowTick = GetTickCount();
	sscanf(m_ListFile.GetItemText(iSel, 6), "%u", &dwStartTick);

	long iEllapsed = (dwNowTick-dwStartTick);//间隔：秒
	strStatus.Format("%dms", iEllapsed);
	m_ListFile.SetItemText(iSel, 4, strStatus.GetBuffer(0));
	
	//显示速度
	if(iEllapsed > 0)
	{
		strStatus.Format("%dk/s", (pmsg->lFileOffset/1024/iEllapsed)*1000);
		m_ListFile.SetItemText(iSel, 3, strStatus.GetBuffer(0));
	}
	return 0;
}

LRESULT CofficeDlg::OnConnectError(WPARAM wParam, LPARAM lParam)
{
	MessageBox("连接服务器失败!");
	return 0;
}


void CofficeDlg::OnNMDblclkListFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW* pListView = (NMLISTVIEW*)pNMHDR;//结构体，ListView的通知信息


	int iSel = pListView->iItem;

	if (iSel == -1)
		return;

	CString strFileName = m_ListFile.GetItemText(iSel, 0);
	CFileDialog dlg(FALSE, NULL, strFileName.GetBuffer(0), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*||", this);
	//打开一个保存对话框，隐藏readonly||覆盖确认
	if (dlg.DoModal() != IDOK)
		return;

	CString strClientPath = dlg.GetPathName();
	//得到客户端保存路径
	CFile file;
	BOOL bOpen = file.Open(strClientPath, CFile::modeCreate | CFile::modeWrite, NULL);

	if (!bOpen)
	{
		MessageBox("文件创建或打开失败！请确认路径再重试！");
		return;
	}
	file.Close();

	MSGREQUEST msgRequest;//通信语言结构体
	msgRequest.iCommand = FILEDATA;
	msgRequest.lFileLength = m_ListFile.GetItemData(iSel);
	msgRequest.lFileOffset = 0;
	strcpy(msgRequest.sClientPath, strClientPath.GetBuffer(0));
	strcpy(msgRequest.sServerPath, m_ListFile.GetItemText(iSel, 5));
	//子项目，第6列

	DWORD dwStartTick = GetTickCount();
	char sStartTick[20];
	sprintf(sStartTick, "%u", dwStartTick);
	//格式：Unsigned decimal integer
	m_ListFile.SetItemText(iSel, 6, sStartTick);

	m_pClient->RequestFile(msgRequest);//调用请求线程

	*pResult = 0;
}
