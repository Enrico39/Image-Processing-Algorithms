
#include <iostream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat isteresi(Mat src, int lth, int hth) {

	Mat dst = Mat::zeros(src.rows, src.cols, CV_8U);

	for (int i = 1; i < src.rows; i++) {
		for (int j = 1; j < src.cols; j++) {
			if (src.at<uchar>(i, j) > hth) {
				dst.at<uchar>(i, j) = 255;
				for (int x = -1; x <= 1; x++) {
					for (int y = -1; y <= 1; y++) {
						if (src.at<uchar>(i + x, j + y) > lth) {
							dst.at<uchar>(i + x, j + y) = 255;
						}
					}
				}
			}
		}
	}

	return dst;
};

Mat noMaxSup(Mat magnitudo, Mat orientation) {

	Mat dst = Mat::zeros(magnitudo.rows, magnitudo.cols, CV_8U);

	for (int i = 1; i < magnitudo.rows; i++) {
		for (int j = 1; j < magnitudo.cols; j++) {
			float mag = magnitudo.at<uchar>(i, j);
			float angolo = orientation.at<float>(i, j);
			angolo = angolo > 180 ? angolo - 360 : angolo;

			if ((angolo > -22.5 && angolo <= 22.5) || (angolo > -157.5 && angolo <= 157.5))
				if (mag > magnitudo.at<uchar>(i, j + 1) && mag > magnitudo.at<uchar>(i, j - 1))
					dst.at<uchar>(i, j) = magnitudo.at<uchar>(i, j);

				else if ((angolo > 67.5 && angolo <= 112.5) || (angolo > -112.5 && angolo <= 67.5))
					if (mag > magnitudo.at<uchar>(i + 1, j) && mag > magnitudo.at<uchar>(i - 1, j))
						dst.at<uchar>(i, j) = magnitudo.at<uchar>(i, j);

					else if ((angolo > 22.5 && angolo <= 67.5) || (angolo > -157.5 && angolo <= -112.5))
						if (mag > magnitudo.at<uchar>(i + 1, j + 1) && mag > magnitudo.at<uchar>(i - 1, j - 1))
							dst.at<uchar>(i, j) = magnitudo.at<uchar>(i, j);

						else if ((angolo > 112.5 && angolo <= 157.5) || (angolo > -67.5 && angolo <= -22.5))
							if (mag > magnitudo.at<uchar>(i - 1, j + 1) && mag > magnitudo.at<uchar>(i + 1, j - 1))
								dst.at<uchar>(i, j) = magnitudo.at<uchar>(i, j);
		}
	}

	return dst;
};

void myCanny(Mat src, Mat& dst, int lth, int hth) {

	Mat gauss;
	GaussianBlur(src, gauss, Size(7, 7), 0, 0);

	Mat sobx, soby, magnitudo, orientation;
	Sobel(gauss, sobx, CV_32F, 1, 0, 3);
	Sobel(gauss, soby, CV_32F, 0, 1, 3);

	magnitude(sobx, soby, magnitudo);
	normalize(magnitudo, magnitudo, 0, 255, NORM_MINMAX, CV_8U);

	phase(sobx, soby, orientation, true);

	Mat nms = noMaxSup(magnitudo, orientation);

	dst = isteresi(nms, lth, hth);
};

 
int main(int argc, char** argv) {

	//Mat src=imread(argv[1],IMREAD_GRAYSCALE),dst;
	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/lenna.jpg", IMREAD_GRAYSCALE), dst;

	//int lth=atoi(argv[2]),hth=atoi(argv[3]);
	int lth = 10, hth = 30;

	myCanny(src, dst, lth, hth);

	imshow("input", src);
	imshow("output", dst);
	waitKey(0);

	return 0;
}