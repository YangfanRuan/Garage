#include "lpr_process.h"

lpr::lpr() {
	m_PlateImg = cv::Mat(cv::Size(40 * HIGH_WITH_CAR, 40), CV_8U, 1);
	m_CharOne = cv::Mat(cv::Size(20, 40), CV_8U, 1);
	m_CharOne.copyTo(m_CharTwo);
	m_CharOne.copyTo(m_CharThree);
	m_CharOne.copyTo(m_CharFour);
	m_CharOne.copyTo(m_CharFive);
	m_CharOne.copyTo(m_CharSix);
	m_CharOne.copyTo(m_CharSeven);
}

void lpr::LoadImg(const std::string& SrcPath) {
	m_src = cv::imread("../res/PlateNumber.jpg");
	if (m_src.empty()) {
		std::cout << "failed to load src..." << std::endl;
		exit(1);
	}
	//...show
	cv::imshow("src", m_src);
}

void lpr::Threshold() {
	cv::cvtColor(m_src, m_CannyImg, cv::COLOR_BGR2GRAY);          // 转化为灰度图  openCV函数 Y = 0.299*R + 0.587*G + 0.114*B                                                     //  转为灰度图	  Y=0.299*R + 0.587*G + 0.114*B
	cv::GaussianBlur(m_CannyImg, m_CannyImg, cv::Size(3, 3), 0);  // 平滑高斯滤波  滤波后的图片保存在 m_CannyImg
	int thresMax = 0, thresMin = 255, i = 0, j = 0;
	for (j = 0; j < m_src.rows; j++)
		for (i = 0; i < m_src.cols; i++) {
			if (m_src.at<uchar>(j, i) > thresMax)
				thresMax = m_src.at<uchar>(j, i);
			else if (m_src.at<uchar>(j, i) < thresMin)
				thresMin = m_src.at<uchar>(j, i);
		}
	cv::Canny(m_src, m_CannyImg, AdaptiveThreshold((thresMax + thresMin) * 0.5, m_src), thresMax * 0.7);
	//...show
	cv::imshow("CannyImg", m_CannyImg);
}

void lpr::Threshold(cv::Mat& GrayImg, cv::Mat& BinImg) {
	int thresMax = 0, thresMin = 255, i = 0, j = 0;
	for (j = 0; j < GrayImg.rows; j++)
		for (i = 0; i < GrayImg.cols; i++) {
			if (GrayImg.at<uchar>(j, i) > thresMax)
				thresMax = GrayImg.at<uchar>(j, i);
			else if (GrayImg.at<uchar>(j, i) < thresMin)
				thresMin = GrayImg.at<uchar>(j, i);
		}
	cv::Canny(GrayImg, BinImg, AdaptiveThreshold((thresMax + thresMin) * 0.5, GrayImg), thresMax * 0.7);
}

int lpr::AdaptiveThreshold(int t, cv::Mat& Image) {
	int t1 = 0, t2 = 0, tnew = 0, i = 0, j = 0;
	int Allt1 = 0, Allt2 = 0, accountt1 = 0, accountt2 = 0;
	for (j = 0; j < Image.rows; j++) {
		for (i = 0; i < Image.cols; i++) {
			if (Image.at<uchar>(j, i) < t) {
				Allt1 += Image.at<uchar>(j, i);
				accountt1++;
			} else {
				Allt2 += Image.at<uchar>(j, i);
				accountt2++;
			}
		}
	}
	t1 = Allt1 / accountt1;
	t2 = Allt2 / accountt2;
	tnew = 0.5 * (t1 + t2);
	if (tnew == t)
		return tnew;
	else
		AdaptiveThreshold(tnew, Image);
}

/*
* 在给定图像中找到可能包含车牌的区域。
*/
void lpr::PlateAreaSearch() {
	if (m_CannyImg.empty()) {
		exit(-1);
	}
	int i = 0, j = 0, k = 0, m = 0;
	bool flag = 0;
	int plate_n = 0, plate_s = 0, plate_e = 0, plate_w = 0;
	int* num_h = new int[MAX(m_CannyImg.cols, m_CannyImg.rows)];
	if (num_h == NULL) {
		std::cout << "memory exhausted!" << std::endl;
		exit(-1);
	}
	for (i = 0; i < m_CannyImg.cols; i++) {
		num_h[i] = 0;
	}
	for (j = 0; j < m_CannyImg.rows; j++) {// 水平方向投影计算
		for (i = 0; i < m_CannyImg.cols - 1; i++) {
			m_CannyImg.at<uchar>(j, i) = m_CannyImg.at<uchar>(j, i + 1) - m_CannyImg.at<uchar>(j, i);
			num_h[j] += m_CannyImg.at<uchar>(j, i) / 250;
		}
	}

	int temp_1 = 0;
	int temp_max = 0;
	int temp_i = 0;
	for (j = 0; j < m_CannyImg.rows - 20; j++) {
		temp_1 = 0;
		for (i = 0; i < 20; i++)
			temp_1 += num_h[i + j];
		if (temp_1 >= temp_max) { // 寻找车牌边缘位置
			temp_max = temp_1;
			temp_i = j;
		}
	}
	k = temp_i;
	while (((num_h[k + 1] > POINT_X) || (num_h[k + 2] > POINT_X) || (num_h[k] > POINT_X)) && k)
		k--;
	plate_n = k + 1;
	k = temp_i + 10;
	while (((num_h[k - 1] > POINT_X) || (num_h[k - 2] > POINT_X) || (num_h[k] > POINT_X)) && (k < m_CannyImg.rows))
		k++;
	plate_s = k;

	if (!(plate_n && plate_s && (plate_n < plate_s) && ((plate_s - plate_n) * HIGH_WITH_CAR < m_CannyImg.cols * (1 - WITH_X)))) {
		std::cout << "水平分割失败" << std::endl;
		exit(-1);
	} else {
		int max_count = 0;
		int plate_length = (m_CannyImg.cols - (plate_s - plate_n) * HIGH_WITH_CAR);
		plate_w = m_CannyImg.cols * WITH_X - 1;

		for (i = 0; i < m_CannyImg.cols; i++)
			for (j = 0; j < m_CannyImg.rows - 1; j++) {
				m_CannyImg.at<uchar>(j, i) = m_CannyImg.at<uchar>(j + 1, i) - m_CannyImg.at<uchar>(j, i);
			}

		for (k = 0; k < plate_length; k++) {
			for (i = 0; i < (int)((plate_s - plate_n) * HIGH_WITH_CAR); i++)
				for (j = plate_n; j < plate_s; j++) { // 两水平线之间
					num_h[k] = num_h[k] + m_CannyImg.at<uchar>(j, i + k) / 250;
				}
			if (num_h[k] > max_count) {
				max_count = num_h[k];
				plate_w = k;
			}
		}
		cv::Rect ROI_rect;
		ROI_rect.x = plate_w;
		ROI_rect.y = plate_n;
		ROI_rect.width = (plate_s - plate_n) * HIGH_WITH_CAR;
		ROI_rect.height = plate_s - plate_n;
		if ((ROI_rect.width + ROI_rect.x) > m_CannyImg.cols) {
			ROI_rect.width = m_CannyImg.cols - ROI_rect.x;
			std::cout << "垂直方向分割失败!" << std::endl;
			exit(-1);
		} else {
			cv::Mat pImg8uROI(cv::Size(ROI_rect.width, ROI_rect.height), m_src.depth(), m_src.channels());
			cv::Mat pImg8u11(cv::Size(40 * HIGH_WITH_CAR, 40), pImg8uROI.depth(), pImg8uROI.channels());
			SetRoi(m_src, ROI_rect, pImg8uROI);
			cv::resize(pImg8uROI, pImg8u11, cv::Size(40 * HIGH_WITH_CAR, 40), 0, 0, cv::INTER_LINEAR);
			cv::cvtColor(pImg8u11, m_PlateImg, cv::COLOR_BGR2GRAY);
			Threshold(m_PlateImg, m_PlateImg);
		}
	}
	//...show
	cv::imshow("PlateImg", m_PlateImg);
	delete[] num_h;
	num_h = NULL;
}

void lpr::SetRoi(cv::Mat src, cv::Rect& ROI_rect, cv::Mat& pImgROI) {
	src = src(cv::Rect(ROI_rect.x, ROI_rect.y, ROI_rect.width, ROI_rect.height));
	pImgROI = src.clone();
	src = src.colRange(0, src.cols).rowRange(0, src.rows);
}


void lpr::SegmentPlate() {
	if (m_PlateImg.empty()) {
		exit(-1);
	}
	int* num_h = new int[MAX(m_PlateImg.cols, m_PlateImg.rows)];
	if (num_h == NULL) {
		std::cout << "字符分割memory exhausted" << std::endl;
		exit(-1);
	} 
	int i = 0, j = 0, k = 0;
	int letter[14] = { 0, 20, 23, 43, 55, 75, 78, 98, 101, 121, 124, 127, 147, 167 };
	bool flag1 = 0;

	for (i = 0; i < 40 * HIGH_WITH_CAR; i++) {
		num_h[i] = 0;
		for (j = 0; j < 17; j++) {
			num_h[i] += m_PlateImg.at<uchar>(j, i) / 45;
		}
		for (j = 24; j < 40; j++) {
			num_h[i] += m_PlateImg.at<uchar>(j, i) / 45;
		}
	}
	int max_count = 0;
	int flag = 0;
	for (i = 30; i < 40 * HIGH_WITH_CAR; i++) {
		if (num_h[i] < POINT_Y) {
			max_count++;
			if (max_count == 11) {
				letter[3] = i - 11;
				while ((num_h[i] < POINT_Y) || (num_h[i - 1] < POINT_Y))
					i++;
				letter[4] = i - 1;
				break;
			}
		} else {
			max_count = 0;
		}
	}
	for (i = 0; i < 40 * HIGH_WITH_CAR; i++) {
		for (j = 17; j <= 24; j++) {
			num_h[i] += m_PlateImg.at<uchar>(j, i) / 45;
		}
	}
	for (j = letter[3]; j > 0; j--) {
		if ((num_h[j] < POINT_Y) && (num_h[j - 1] < POINT_Y)) {
			letter[2] = j;
			letter[1] = (j >= 23) ? j - 3 : letter[1];
			letter[0] = (j >= 23) ? j - 23 : letter[0];
			break;
		}
	}
	j = 2;
	flag = 0;
	flag1 = 0;
	for (i = letter[4]; i < 40 * HIGH_WITH_CAR; i++) {
		if ((num_h[i] > POINT_Y) && (num_h[i - 1] > POINT_Y) && !flag) {
			flag = 1;
			flag1 = 0;
			letter[2 * j] = i - 1;
			if (j == 6) {
				letter[2 * j + 1] = ((letter[2 * j] + 20) > 40 * HIGH_WITH_CAR - 1) ? 40 * HIGH_WITH_CAR - 1 : letter[2 * j] + 20;
				break;
			}
		} else if ((num_h[i] < POINT_Y) && (num_h[i - 1] < POINT_Y) && !flag1 && flag) {
			flag = 0;
			flag1 = 1;
			letter[2 * j + 1] = i - 1;
			j++;
		}
	}
	for (i = 1; i < 191; i++) {
		for (j = 1; j < 39; j++) {
			if (m_PlateImg.at<uchar>(j, i) && m_PlateImg.at<uchar>(j, i + 1) && m_PlateImg.at<uchar>(j + 1, i)) //  01
				m_PlateImg.at<uchar>(j, i) = 0;                                                                 //   1
			if (m_PlateImg.at<uchar>(j, i) && m_PlateImg.at<uchar>(j, i - 1) && m_PlateImg.at<uchar>(j + 1, i)) //  10
				m_PlateImg.at<uchar>(j, i) = 0;                                                                 //    1
			if (m_PlateImg.at<uchar>(j, i) && m_PlateImg.at<uchar>(j, i - 1) && m_PlateImg.at<uchar>(j - 1, i)) //   1
				m_PlateImg.at<uchar>(j, i) = 0;                                                                 // 10
			if (m_PlateImg.at<uchar>(j, i) && m_PlateImg.at<uchar>(j, i + 1) && m_PlateImg.at<uchar>(j - 1, i)) // 1
				m_PlateImg.at<uchar>(j, i) = 0;                                                                 // 01
		}
	}



	cv::Rect ROI_rect1;
	ROI_rect1.x = 0.5 * (letter[1] + letter[0]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;

	SetRoi(m_PlateImg, ROI_rect1, m_CharOne);

	ROI_rect1.x = 0.5 * (letter[3] + letter[2]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	SetRoi(m_PlateImg, ROI_rect1, m_CharTwo);

	ROI_rect1.x = 0.5 * (letter[5] + letter[4]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	SetRoi(m_PlateImg, ROI_rect1, m_CharThree);

	ROI_rect1.x = 0.5 * (letter[7] + letter[6]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	SetRoi(m_PlateImg, ROI_rect1, m_CharFour);

	ROI_rect1.x = 0.5 * (letter[9] + letter[8]) - 10;
	ROI_rect1.y = 0;
	SetRoi(m_PlateImg, ROI_rect1, m_CharFive);

	ROI_rect1.x = 0.5 * (letter[11] + letter[10]) - 10;
	ROI_rect1.y = 0;
	SetRoi(m_PlateImg, ROI_rect1, m_CharSix);

	ROI_rect1.x = 0.5 * (letter[13] + letter[12]) - 10;
	ROI_rect1.y = 0;
	ROI_rect1.width = 20;
	ROI_rect1.height = 40;
	SetRoi(m_PlateImg, ROI_rect1, m_CharSeven);

	delete[] num_h;
	num_h = NULL;
}                                                     

void lpr::SingleCodeRecognize(cv::Mat& CharImg, int num, int char_num) {
	if (CharImg.empty()) {
		exit(-1);
	}

	int i = 0, j = 0, k = 0, t = 0;
	int char_start = 0, char_end = 0;
	int num_t[CHARACTER] = { 0 };

	switch (num) {
	case 0:
		char_start = 0;
		char_end = 9;
		break;
	case 1:
		char_start = 10;
		char_end = 35;
		break;
	case 2:
		char_start = 0;
		char_end = 35;
		break;
	case 3:
		char_start = 36;
		char_end = TEMPLETENUM - 1;
		break;
	default:
		break;
	}

	for (k = 0; k < 8; k++) {
		for (j = int(k / 2) * 10; j < int(k / 2 + 1) * 10; j++) {
			for (i = (k % 2) * 10; i < (k % 2 + 1) * 10; i++) {
				num_t[k] += CharImg.at<uchar>(j, i) / 255;
			}
		}
		num_t[8] += num_t[k];
	}

	for (i = 0; i < 20; i++)
		num_t[9] += CharImg.at<uchar>(10, i) / 255;
	for (i = 0; i < 20; i++)
		num_t[10] += CharImg.at<uchar>(20, i) / 255;
	for (i = 0; i < 20; i++)
		num_t[11] += CharImg.at<uchar>(30, i) / 255;

	for (j = 0; j < 40; j++)
		num_t[12] += CharImg.at<uchar>(j, 7) / 255;
	for (j = 0; j < 40; j++)
		num_t[13] += CharImg.at<uchar>(j, 10) / 255;
	for (j = 0; j < 40; j++)
		num_t[14] += CharImg.at<uchar>(j, 13) / 255;

	int num_tt[CHARACTER] = { 0 };
	int matchnum = 0;
	int matchnum_max = 0;
	int matchcode = 0;

	j = 0;

	for (k = char_start; k <= char_end; k++) {
		matchnum = 0;

		for (i = 0; i < 8; i++) {
			if (abs(num_t[i] - Num_Templete[k][i]) <= 2)
				matchnum++;
		}

		if (Num_Templete[k][i] - abs(num_t[i]) <= 8)
			matchnum += 2;
		for (i = 9; i < CHARACTER; i++) {
			if (Num_Templete[k][i] >= 5) {
				if (abs(num_t[i] - Num_Templete[k][i]) <= 1)
					matchnum += 2;
			} else if (num_t[i] == Num_Templete[k][i]) {
				matchnum += 2;
			}
		}
		if (matchnum > matchnum_max) {
			matchnum_max = matchnum;
			matchcode = k;
		}
	}
	G_PlateChar[char_num] = PlateCode[matchcode];
}

void lpr::CodeRecognize() {
	SingleCodeRecognize(m_CharOne, 3, 0);
	SingleCodeRecognize(m_CharTwo, 1, 1);
	SingleCodeRecognize(m_CharThree, 2, 2);
	SingleCodeRecognize(m_CharFour, 2, 3);
	SingleCodeRecognize(m_CharFive, 0, 4);
	SingleCodeRecognize(m_CharSix, 0, 5);
	SingleCodeRecognize(m_CharSeven, 0, 6);
	//...show
	for (int i = 0; i < 7; i++) {
		std::cout << G_PlateChar[i];
	}
	std::cout<<std::endl;
}