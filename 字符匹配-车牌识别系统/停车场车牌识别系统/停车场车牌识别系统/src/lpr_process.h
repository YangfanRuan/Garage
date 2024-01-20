#ifndef LPR_PROCESS_H
#define LPR_PROCESS_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "lpr_font.h"

class lpr {
  public:
	  lpr();
	  void LoadImg(const std::string& SrcPath);
	  void Threshold();
	  void PlateAreaSearch();
	  void SegmentPlate();
	  void CodeRecognize();
  private:
	  cv::Mat m_src; // 原始图像
	  cv::Mat m_CannyImg; // 提取边缘后的二值图像
	  cv::Mat m_PlateImg; // 归一化后的车牌区域图像
	  cv::Mat m_CharOne, m_CharTwo, m_CharThree, m_CharFour, m_CharFive, m_CharSix, m_CharSeven;// 车牌字符
	  int AdaptiveThreshold(int t, cv::Mat& Image);
	  void Threshold(cv::Mat& GrayImg, cv::Mat& BinImg);
	  void SetRoi(cv::Mat src, cv::Rect& ROI_rect, cv::Mat& pImgROI);
	  void SingleCodeRecognize(cv::Mat& CharImg, int num, int char_num);
};

#endif // LPR_PROCESS_H
