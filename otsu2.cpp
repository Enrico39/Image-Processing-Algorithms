
//otsu2
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

#define H_SIZE 256

using namespace cv;
using namespace std;

void otsu2(Mat src, Mat& dst) {

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
		globAVG += normHistogram[i] * i;
	}

	double P1k = 0, p1k = 0, m1k = 0;
	double maxvarianza = 0;
	int optTh1 = 0, optTh2 = 0;
	for (int i = 0; i < H_SIZE; i++) {
		P1k += normHistogram[i];
		p1k += i * normHistogram[i];
		m1k = p1k / P1k;
		double P2k = 0, p2k = 0, m2k = 0;
		for (int j = i + 1; j < H_SIZE; j++) {
			P2k += normHistogram[j];
			p2k += j * normHistogram[j];
			m2k = p2k / P2k;
			double P3k = 1 - P2k - P1k;
			double p3k = globAVG - p1k - p2k;
			double m3k = p3k / P3k;
			double varianza = P1k * pow(m1k - globAVG, 2) + P2k * pow(m2k - globAVG, 2) + P3k * pow(m3k - globAVG, 2);
			if (varianza > maxvarianza) {
				maxvarianza = varianza;
				optTh1 = i;
				optTh2 = j;
			}
		}
	}

	dst = Mat::zeros(src.rows, src.cols, CV_8U);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			uchar val = src.at<uchar>(i, j);
			if (val > optTh2)
				dst.at<uchar>(i, j) = 255;
			else if (val > optTh1 && val < optTh2)
				dst.at<uchar>(i, j) = 128;
		}
	}

};

int main(int argc, char** argv) {

	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/lenna.jpg", IMREAD_GRAYSCALE), dst;

	otsu2(src, dst);

	imshow("input", src);
	imshow("output", dst);
	waitKey(0);

	return 0;
}
