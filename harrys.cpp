
//HARRYS

 
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;
using namespace std;

void myHarris(Mat src, Mat& dst, float k, int th) {

	Mat dx, dy, d2x, d2y, dxy;
	Sobel(src, dx, CV_32F, 1, 0, 3);
	Sobel(src, dy, CV_32F, 0, 1, 3);
	multiply(dx, dy, dxy);
	pow(dx, 2, d2x);
	pow(dy, 2, d2y);

	GaussianBlur(d2x, d2x, Size(7, 7), 2.0, 0);
	GaussianBlur(d2y, d2y, Size(7, 7), 0, 2.0);
	GaussianBlur(dxy, dxy, Size(7, 7), 2.0, 2.0);

	Mat det1, det2, trace, R;
	multiply(d2x, d2y, det1);
	multiply(dxy, dxy, det2);
	pow(d2x + d2y, 2, trace);

	R = det1 - det2 - k * trace;
	normalize(R, R, 0, 255, NORM_MINMAX, CV_32F);

	dst = src.clone();

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if ((int)R.at<float>(i, j) > th)
				circle(dst, Point(j, i), 5, Scalar(255), 1.5);
		}
	}
};
 

int main(int argc, char** argv) {

	//Mat src = imread(argv[1], IMREAD_GRAYSCALE), dst;
	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/blox.jpg", IMREAD_GRAYSCALE), dst;
	if (src.empty()) {
		cout << "Impossibile caricare l'immagine." << endl;
		return -1;
	}
	int th = 60;

	myHarris(src, dst, 0.04,th);
	imshow("input", src);
	imshow("out", dst);
	waitKey();

	return 0;
}
 
 