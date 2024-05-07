
//K-means

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

double myDistance(Scalar p1, Scalar p2) {
	return pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2);
};

void myKmeans(Mat src, Mat& dst, int nClusters) {

	vector<Scalar> centers_colors;
	vector<vector<Point>> clusters;

	for (int k = 0; k < nClusters; k++) {
		Point center;
		center.x = (src.cols / nClusters) * k;
		center.y = (src.rows / nClusters) * k;

		Scalar center_colors(src.at<Vec3b>(center)[0], src.at<Vec3b>(center)[1], src.at<Vec3b>(center)[2]);

		centers_colors.push_back(center_colors);
		vector<Point> t;
		clusters.push_back(t);
	}

	double difference = INFINITY;
	double oldCenterSum = 0;
	while (difference > 0.1) {

		for (int k = 0; k < nClusters; k++) {
			clusters[k].clear();
		}

		for (int x = 0; x < src.rows; x++) {
			for (int y = 0; y < src.cols; y++) {
				double minDistance = INFINITY;
				int clusterIndex = 0;
				Point pixel(y, x);
				for (int k = 0; k < nClusters; k++) {
					Scalar center = centers_colors[k];
					Scalar point(src.at<Vec3b>(x, y)[0], src.at<Vec3b>(x, y)[1], src.at<Vec3b>(x, y)[2]);

					double distance = myDistance(center, point);
					if (distance < minDistance) {
						minDistance = distance;
						clusterIndex = k;
					}
				}
				clusters[clusterIndex].push_back(pixel);
			}
		}

		double newCenterSum = 0;
		for (int k = 0; k < nClusters; k++) {
			vector<Point> clusterPoints = clusters[k];
			double blue = 0, green = 0, red = 0;
			for (int i = 0; i < clusterPoints.size(); i++) {
				Point pixel = clusterPoints[i];
				blue += src.at<Vec3b>(pixel)[0];
				green += src.at<Vec3b>(pixel)[1];
				red += src.at<Vec3b>(pixel)[2];
			}
			blue /= clusterPoints.size();
			green /= clusterPoints.size();
			red /= clusterPoints.size();

			Scalar center = centers_colors[k];
			Scalar newCenter(blue, green, red);

			newCenterSum += myDistance(center, newCenter);
			centers_colors[k] = newCenter;
		}
		newCenterSum /= nClusters;
		difference = abs(oldCenterSum - newCenterSum);
		oldCenterSum = newCenterSum;

	}

	for (int k = 0; k < nClusters; k++) {
		vector<Point> clusterPoints = clusters[k];
		Scalar center = centers_colors[k];
		for (int i = 0; i < clusterPoints.size(); i++) {
			Point pixel = clusterPoints[i];
			dst.at<Vec3b>(pixel)[0] = center[0];
			dst.at<Vec3b>(pixel)[1] = center[1];
			dst.at<Vec3b>(pixel)[2] = center[2];
		}
	}


};

int main(int argc, char** argv) {

	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/lenna.jpg");
	Mat dst(src.size(), src.type());

	int nClusters = 4;

	myKmeans(src, dst, nClusters);

	imshow("input", src);
	imshow("output", dst);
	waitKey(0);

	return 0;
}