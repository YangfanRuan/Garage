
// officeDlg.h: 头文件
//

#pragma once
#include <opencv2/imgproc/types_c.h>
#include "CTCPClient.h"
// CofficeDlg 对话框
class CofficeDlg : public CDialogEx
{
// 构造
public:
	CTCPClient* m_pClient;
	int CofficeDlg::CodeRecognize(IplImage *imgTest, int num, int char_num);
	int CofficeDlg::SegmentPlate();
	int CofficeDlg::PlateAreaSearch(IplImage *pImg_Image);
	void CofficeDlg::Threshold(IplImage *Image, IplImage *Image_O);
	int CofficeDlg::AdaptiveThreshold(int t, IplImage *Image);
	void CofficeDlg::DrawPicToHDC(IplImage *img, UINT ID);
	IplImage *src;                                // 原始图片
	IplImage *pImgCanny;                      //二值化的图
	IplImage *pImgResize;                     //归一化的车牌区域灰度图
	IplImage *pImgCharOne;                // 字符图片
	IplImage *pImgCharTwo;
	IplImage *pImgCharThree;
	IplImage *pImgCharFour;
	IplImage *pImgCharFive;
	IplImage *pImgCharSix;
	IplImage *pImgCharSeven;
	CofficeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OFFICE_DIALOG };
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
	afx_msg LRESULT OnFileList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileStatus(WPARAM wParam, LPARAM lParam); //文件状态
	afx_msg LRESULT OnConnectError(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenpic();
	afx_msg void OnBnClickedPicbin();
	afx_msg void OnBnClickedLocpic();
	afx_msg void OnBnClickedSpitchar();
 
	afx_msg void OnBnClickedIdchar();
	CIPAddressCtrl m_IPCtrl;
	afx_msg void OnBnClickedConnectSrv();
	CListCtrl m_ListFile;
	afx_msg void OnNMDblclkListFilelist(NMHDR *pNMHDR, LRESULT *pResult);
};
