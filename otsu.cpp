#include <iostream>
#include <opencv2/opencv.hpp>

#define H_SIZE 256

using namespace cv;
using namespace std;

void otsu(Mat src, Mat& dst) {

	double histogram[H_SIZE] = { 0 }, normHistogram[H_SIZE] = { 0 };
	double MN = src.rows * src.cols;
	double globAVG = 0;

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			histogram[src.at<uchar>(i, j)]++;
		}
	}

	for (int i = 0; i < H_SIZE; i++) {
		normHistogram[i] = histogram[i] / MN;
		globAVG += i * normHistogram[i];
	}

	double p1k=0, P1k=0, m1k=0, p2k=0, P2k=0, m2k=0;
	double varianza = 0, maxVarianza = 0;
	int optTh = 0;

	for (int i = 0; i < H_SIZE; i++) {
		P1k += normHistogram[i];
		p1k += i * normHistogram[i];
		m1k = p1k / P1k;
		P2k = 1 - P1k;
		p2k = globAVG - p1k;
		m2k = p2k / P2k;
		varianza = P1k * P2k * pow(m1k - m2k, 2);
		if (varianza > maxVarianza) {
			maxVarianza = varianza;
			optTh = i;
		}
	}

	dst = Mat::zeros(src.rows, src.cols, CV_8U);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src.at<uchar>(i, j) > optTh)
				dst.at<uchar>(i, j) = 255;
		}
	}

};

int main(int argc, char** argv) {

	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/lenna.jpg", IMREAD_GRAYSCALE), dst;

	otsu(src, dst);

	imshow("input", src);
	imshow("output", dst);
	waitKey(0);

	return 0;
}