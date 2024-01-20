#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
	cv::Mat A;
	A = cv::imread("../res/house.tif");
	cv::Mat B(A);
	cv::Mat C = A;
	cv::Mat D = A.clone();
	cv::Mat E;
	A.copyTo(E);

	cv::Mat F{};
	cv::Mat G(A.rows, A.cols, A.type());
	cv::Mat H(A.size(), A.type());
	cv::Mat I(A.rows, A.cols, A.type(), cv::Scalar(255,255,255));
	cv::Mat J(cv::Size(A.rows, A.cols), A.type(), cv::Scalar(255, 255, 255));
	cv::Mat K(A);
	cv::Mat L(A.rows, A.cols, A.type(), A.data);
	cv::Mat M(cv::Size(A.rows, A.cols), A.type(), A.data);
	cv::Mat N(A, cv::Range(0,10), cv::Range(0, 10)); // rowRange, colRange
	cv::Mat O(A, cv::Rect(10, 10, 20, 20)); // start_x, start_y, cols, rows
	//cv::Mat P(2, {A.rows, A.cols}, A.type(), cv::Scalar::all(255));

	cv::Mat Q;
	Q.create(A.rows, A.cols, A.type());

	cv::Mat R = cv::Mat::zeros(A.rows, A.cols, A.type());
	cv::Mat S = cv::Mat::ones(A.rows, A.cols, A.type());
	cv::Mat T = cv::Mat::eye(A.rows, A.cols, A.type());

	cv::waitKey(0);
}

/*
* A.size() = cv::Size(A.rows, A.cols)
* cv::Scalar(255, 255, 255) = cv::Scalar::all(255)
* X.types() = CV_(位的深度)(S/U/F)C(通道数)
* 注意Mat的成员是否带括号,cols+rows+data+dims没有括号,其余有
* G-O可以先构造后赋值,如:
*		cv::Mat G;
*		G = cv::Mat(A.rows, A.cols, A.type());
*/
