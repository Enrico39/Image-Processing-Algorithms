
//Split and Merge

#include <iostream>
#include <opencv2/opencv.hpp>

#define MIN_DEV 10.6
#define MAX_AREA 64

using namespace std;
using namespace cv;

struct Region {
	vector<Region> adj;
	bool valid;
	Scalar label;
	Rect area;
};

bool predicate(Mat src) {
	Scalar dev;
	meanStdDev(src, Scalar(), dev);
	return(dev[0] < MIN_DEV || src.rows * src.cols < MAX_AREA);
};

Region split(Mat src, Rect area) {

	Region R;
	R.valid = true;
	R.area = area;
	if (predicate(src)) {
		Scalar mean;
		meanStdDev(src, mean, Scalar());
		R.label = mean;
	}
	else {
		int w = src.cols / 2;
		int h = src.rows / 2;
		Region r1 = split(src(Rect(0, 0, w, h)), Rect(area.x, area.y, w, h));
		Region r2 = split(src(Rect(w, 0, w, h)), Rect(area.x + w, area.y, w, h));
		Region r3 = split(src(Rect(0, h, w, h)), Rect(area.x, area.y + h, w, h));
		Region r4 = split(src(Rect(w, h, w, h)), Rect(area.x + w, area.y + h, w, h));

		R.adj.push_back(r1);
		R.adj.push_back(r2);
		R.adj.push_back(r3);
		R.adj.push_back(r4);
	}

	return R;
};

void mergeRegion(Mat src, Region& r1, Region& r2) {
	Rect r12 = r1.area | r2.area;
	if (predicate(src(r12))) {
		r1.area = r12;
		r1.label = (r1.label + r2.label) / 2;
		r2.valid = false;
	}
};

void merge(Mat src, Region& R) {
	if (R.adj.size() > 0) {
		mergeRegion(src, R.adj.at(0), R.adj.at(1));
		mergeRegion(src, R.adj.at(0), R.adj.at(2));
		mergeRegion(src, R.adj.at(1), R.adj.at(3));
		mergeRegion(src, R.adj.at(2), R.adj.at(3));
		for (int i = 0; i < R.adj.size(); i++)
			merge(src, R.adj.at(i));
	}
};

void display(Mat& out, Region R) {
	if (R.adj.size() == 0 && R.valid)
		rectangle(out, R.area, R.label, FILLED);

	for (int i = 0; i < R.adj.size(); i++) {
		display(out, R.adj.at(i));
	}
};

int main(int argc, char** argv) {
	Mat src = imread("C:/Users/enric/Desktop/Laboratorio_ELIM/images/lenna.jpg", IMREAD_ANYCOLOR);
	
	Region R = split(src, Rect(0, 0, src.cols, src.rows));
	merge(src, R);

	Mat out = Mat::zeros(src.size(), src.type());
	display(out, R);

	imshow("input", src);
	imshow("output", out);
	waitKey();

	return 0;
} 
