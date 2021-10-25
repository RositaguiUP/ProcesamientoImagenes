#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <map>

#include <cmath>

using namespace cv;
using namespace std;

Mat scale(Mat, float, float);
Mat interpol(Mat, float, float);

int main() {
	string path = samples::findFile("images/cat.jpg");
	Mat originalImage = imread(path, IMREAD_GRAYSCALE);

	Mat reducImage = scale(originalImage, 0.5, 0.5);
	Mat ampliImage = scale(originalImage, 1.5, 1.5);

	imshow("Original", originalImage);
	imshow("Reducción", reducImage);
	imshow("Ampliación", ampliImage);

	int k = waitKey(0); // Wait for a keystroke in the window
	/* if (k == 's')
	{
		imwrite("Original.png", originalImage);
	} */
	return 0;
}

Mat scale(Mat original, float cx, float cy) {
	int cyn = floor(original.rows * cy);
	int cxm = floor(original.cols * cx);
	int u, v;

	Mat newImage(cyn, cxm, CV_8UC1);

	for (int j = 0; j < cyn; j++) {
		for (int i = 0; i < cxm; i++) {
			u = floor(j / cy);
			v = floor(i / cx);
			newImage.at<uchar>(j, i) = original.at<uchar>(u, v);
		}
	}
	
	return newImage;
}

Mat interpol(Mat original, float cx, float cy) {
	int cyn = floor(original.rows * cy);
	int cxm = floor(original.cols * cx);
	float u, v, q11, q12, q21, q22, r1, r2;
	int x1, x2, y1, y2, p;

	Mat newImage(cyn, cxm, CV_8UC1);

	for (int j = 1; j < cyn - 1; j++) {
		for (int i = 1; i < cxm - 1; i++) {
			u = j / cy;
			v = i / cx;
			x1 = floor(v);
			x2 = ceil(v);
			y1 = floor(u);
			y2 = ceil(u);

			if (x1 != x2 && y1 != y2) {
				q11 = original.at<uchar>(y1, x1);
				q12 = original.at<uchar>(y2, x1);
				q21 = original.at<uchar>(y1, x2);
				q22 = original.at<uchar>(y2, x2);

				r1 = ((q21 - q11) / (x2 - x1)) * (v - x1) + q11;
				r2 = ((q22 - q12) / (x2 - x1)) * (v - x1) + q12;

				p = floor(((r2 - r1) / (y2 - y1)) * (u - y1) + r1);
			}
			else if (x1 == x2 && y1 != y2) {
				p = floor((x1 / (y2 - y1)) * (u - y1) + r1);
			}
			else if (x1 != x2 && y1 == y2) {
				q21 = original.at<uchar>(y1, x2);
				q22 = original.at<uchar>(y2, x2);

				r1 = ((q21 - q11) / (x2 - x1)) * (v - x1) + q11;

				p = floor((r1 / (y2 - y1)) * (u - y1) + r1);
			}
			 

			newImage.at<uchar>(j, i) = original.at<uchar>(u, v);
		}
	}

	return newImage;
}
