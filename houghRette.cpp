//HOUGH RETTE

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void hough(Mat src, Mat& dst, int th) {

	int d = hypot(src.rows, src.cols);
	Mat H = Mat::zeros(d * 2, 181, CV_8U);

	Mat gauss, canny;
	GaussianBlur(src, gauss, Size(7, 7), 0, 0);
	Canny(gauss, canny, 150, 200, 3);

	double rho, theta;
	for (int x = 0; x < src.rows; x++) {
		for (int y = 0; y < src.cols; y++) {
			if (canny.at<uchar>(x, y) == 255) {
				for (theta = 0; theta <= 180; theta++) {
					double t = (theta - 90) * CV_PI / 180;
					rho = x * sin(t) + y * cos(t);
					H.at<uchar>(rho, theta)++;
				}
			}
		}
	}

	dst = src.clone();

	double t, sint, cost;
	int x, y;
	for (int rho = 0; rho < H.rows; rho++) {
		for (int theta = 0; theta < H.cols; theta++) {
			if (H.at<uchar>(rho, theta) > th) {
				double t = (theta - 90) * CV_PI / 180;
				sint = sin(t);
				cost = cos(t);
				x = rho * cost;
				y = rho * sint;
				Point pt1(cvRound(x + d * (-sint)), cvRound(y + d * cost));
				Point pt2(cvRound(x - d * (-sint)), cvRound(y - d * cost));
				line(dst, pt1, pt2, Scalar(255), 1.5);
			}
		}
	}


};

int main(int argc, char** argv) {

	//Mat src = imread(argv[1], IMREAD_GRAYSCALE), dst;

	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/auto.jpg", IMREAD_GRAYSCALE), dst;
	if (src.empty()) {
		cout << "Impossibile caricare l'immagine." << endl;
		return -1;
	}
	int th =40;

	hough(src, dst, th);

	imshow("input", src);
	imshow("output", dst);
	waitKey();

	return 0;
}