#include <iostream>
#include <opencv2/opencv.hpp>
#include "lpr_process.h"

std::string SrcPath{ "../res/PlateNumber.jpg" };// 修改此处,其余不动

int main() {
    lpr Lpr{};

    // 打开图片
    Lpr.LoadImg(SrcPath);
    // 图像二值化
    Lpr.Threshold();
    // 定位车牌
    Lpr.PlateAreaSearch();
    // 分割字符
    Lpr.SegmentPlate(); 
    // 识别字符 
    Lpr.CodeRecognize();

    cv::waitKey(0);
}
