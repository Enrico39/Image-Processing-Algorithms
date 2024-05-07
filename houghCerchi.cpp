//Hough cerchi


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void hough(Mat src, Mat& dst, int th) {

	int rmin = 40, rmax = 200;

	Mat canny, gauss;
	GaussianBlur(src, gauss, Size(7, 7), 0, 0);
	Canny(gauss, canny, 150, 200, 3);

	int sizes[] = { canny.rows,canny.cols,rmax - rmin + 1 };
	Mat H = Mat(3, sizes, CV_8U, Scalar(0));

	double b, a, r;
	for (int x = 0; x < canny.rows; x++) {
		for (int y = 0; y < canny.cols; y++) {
			if (canny.at<uchar>(x, y) == 255) {
				for (double theta = 0; theta < 360; theta++) {
					for (r = rmin; r <= rmax; r++) {
						a = y - r * cos(theta * CV_PI / 180);
						b = x - r * sin(theta * CV_PI / 180);
						if (a > 0 && a < canny.cols && b>0 && b < canny.rows)
							H.at<uchar>(b, a, r - rmin)++;
					}
				}
			}
		}
	}

	dst = src.clone();

	for (r = rmin; r < rmax; r++) {
		for (b = 0; b < canny.rows; b++) {
			for (a = 0; a < canny.cols; a++) {
				if (H.at<uchar>(b, a, r - rmin) > th) {
					circle(dst, Point(a, b), r, Scalar(0), 2);
					circle(dst, Point(a, b), 3, Scalar(200), 1);
				}
			}
		}
	}
};


int main(int argc, char** argv) {

	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/coins.png", IMREAD_GRAYSCALE), dst;
	if (src.empty()) {
		cout << "Impossibile caricare l'immagine." << endl;
		return -1;
	}
	int th = 140;
 	hough(src, dst, th);

	imshow("input", src);
	imshow("output", dst);
 
	waitKey();
 
	return 0;
}
