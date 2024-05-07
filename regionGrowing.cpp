#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>

using namespace std;
using namespace cv;

uchar max_region = 100;
Point pshift[8] = {
	Point(1,1),
	Point(1,0),
	Point(1,-1),
	Point(0,1),
	Point(0,-1),
	Point(-1,1),
	Point(-1,0),
	Point(-1,-1)
};

void grow(Mat src, Mat dst, Mat& region, Point seed, int th) {

	stack<Point> pstack;
	pstack.push(seed);

	while (!pstack.empty()) {

		Point center = pstack.top();
		pstack.pop();

		for (int i = 0; i < 8; i++) {
			Point punto = center + pshift[i];
			if (punto.x<0 || punto.x>src.cols - 1 || punto.y<0 || punto.y>src.rows - 1) continue;
			else {
				int delta = (int)pow(src.at<Vec3b>(punto)[0], 2) + pow(src.at<Vec3b>(punto)[1], 2) + pow(src.at<Vec3b>(punto)[2], 2);
				if (delta < th && dst.at<uchar>(punto) == 0 && region.at<uchar>(punto) == 0)
					region.at<uchar>(punto) = 1;
				pstack.push(punto);
			}
		}

	}
};

void myRegionGrowing(Mat src, Mat& dst, int th) {

	int min_region_area = (int)src.rows * src.cols * 0.01;

	Mat region = Mat::zeros(src.size(), CV_8U);
	dst = Mat::zeros(src.size(), CV_8U);

	uchar starting_label = 100;
	uchar label = starting_label;

	for (int i = 0; i < src.cols; i++) {
		for (int j = 0; j < src.rows; j++) {
			if (dst.at<uchar>(i, j) == 0) {
				grow(src, dst, region, Point(i, j), th);
				int region_area = (int)sum(region).val[0];
				if (region_area > min_region_area) {
					dst += region * label;
					if (++label > starting_label + max_region) exit(-1);
				}
				else dst += region * 255;
				region -= region;
			}
		}
	}
};

int main(int argc, char** argv) {

	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/lenna.jpg", IMREAD_GRAYSCALE), dst;

	int th = 100;

	myRegionGrowing(src, dst, th);

	imshow("input", src);
	imshow("output", dst);
	waitKey();

	return 0;
}
