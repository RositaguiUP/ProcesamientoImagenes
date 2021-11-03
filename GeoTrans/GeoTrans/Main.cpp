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
int interpolationP(Mat, float, float);
Mat translate(Mat, int, int);
Mat rotate(Mat, float);
Mat skewy(Mat, float);
Mat skewx(Mat, float);

int main() {
	string path = samples::findFile("images/cat.jpg");
	Mat originalImage = imread(path, IMREAD_GRAYSCALE);

	// Mat reducImage = scale(originalImage, 0.5, 0.5);
	//Mat ampliImage = scale(originalImage, 1.5, 1.5);

	//Mat reducInterImage = interpol(originalImage, 0.5, 0.5);
	//Mat ampliInterImage = interpol(originalImage, 1.5, 1.5);

	//Mat translImage = translate(originalImage, 200, 200);
	//Mat rotateImage = rotate(originalImage, 90);
	Mat skewXImage = skewx(originalImage, 0.8);

	imshow("Original", originalImage);
	imshow("Skew (Sesgo)", skewXImage);

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
				q11 = original.at<uchar>(y1, v);
				q12 = original.at<uchar>(y2, v);

				p = floor((q12 - q11 / (y2 - y1)) * (u - y1) + q11);
			}
			else if (x1 != x2 && y1 == y2) {
				q11 = original.at<uchar>(u, x1);
				q12 = original.at<uchar>(u, x2);

				p = floor((q12 - q11 / (x2 - x1)) * (v - x1) + q11);
			}
			else {
				p = original.at<uchar>(u, v);
			}

			newImage.at<uchar>(j, i) =(uchar)p;
		}
	}

	return newImage;
}

int interpolationP(Mat original, float u, float v) {
	float q11, q12, q21, q22, r1, r2;
	int x1, x2, y1, y2, p;

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
		q11 = original.at<uchar>(y1, v);
		q12 = original.at<uchar>(y2, v);

		p = floor((q12 - q11 / (y2 - y1)) * (u - y1) + q11);
	}
	else if (x1 != x2 && y1 == y2) {
		q11 = original.at<uchar>(u, x1);
		q12 = original.at<uchar>(u, x2);

		p = floor((q12 - q11 / (x2 - x1)) * (v - x1) + q11);
	}
	else {
		p = original.at<uchar>(u, v);
	}

	return p;
}

Mat translate(Mat original, int tx, int ty) {
	int u, v;

	Mat newImage(original.rows, original.cols, CV_8UC1);

	for (int j = 0; j < newImage.rows; j++) {
		for (int i = 0; i < newImage.cols; i++) {
			u = j - ty;
			v = i - tx;
			if (u > 0 && u < newImage.rows && v > 0 && v < newImage.cols)
				newImage.at<uchar>(j, i) = original.at<uchar>(u, v);
		}
	}

	return newImage;
}

Mat rotate(Mat original, float teta) {
	float u, v, u0, v0, x0, y0, p;
	float radians = (teta * 3.14159) / 180;
	Mat newImage(original.rows, original.cols, CV_8UC1);

	for (int j = 0; j < newImage.rows; j++) {
		for (int i = 0; i < newImage.cols; i++) {
			x0 = i - original.cols / 2;
			y0 = (original.rows / 2) - j;

			v0 = (x0 * cos(radians)) + (y0 * sin(radians));
			u0 = -(x0 * sin(radians)) + (y0 * cos(radians));

			v = v0 + original.cols / 2;
			u = (original.rows / 2) - u0;
			if (u > 0 && u < original.rows -1 && v > 0 && v < original.cols-1) {
				p = interpolationP(original, u, v);
				newImage.at<uchar>(j, i) = (uchar)p;
			}
			else {
				newImage.at<uchar>(j, i) = (uchar)0;
			}
		}
	}

	return newImage;
}

Mat skewy(Mat original, float sy) {
	int x0, y0, p;
	float u0, v0, u, v;
	Mat newImage(original.rows, original.cols, CV_8UC1);

	for (int j = 0; j < newImage.rows; j++) {
		for (int i = 0; i < newImage.cols; i++) {
			x0 = i - original.cols / 2;
			y0 = original.rows / 2 - j;

			v0 = x0;
			u0 = y0 - (sy * v0);

			v = v0 + original.cols / 2;
			u = original.rows / 2 - u0;
			if (u > 0 && u < original.rows - 1 && v > 0 && v < original.cols - 1) {
				p = interpolationP(original, u, v);
				newImage.at<uchar>(j, i) = (uchar)p;
			}
			else {
				newImage.at<uchar>(j, i) = (uchar)0;
			}
		}
	}

	return newImage;
}

Mat skewx(Mat original, float sx) {
	int x0, y0, p;
	float u0, v0, u, v;
	Mat newImage(original.rows, original.cols, CV_8UC1);

	for (int j = 0; j < newImage.rows; j++) {
		for (int i = 0; i < newImage.cols; i++) {
			x0 = i - original.cols / 2;
			y0 = original.rows / 2 - j;

			u0 = y0;
			v0 = x0 - (sx * u0);

			v = v0 + original.cols / 2;
			u = original.rows / 2 - u0;
			if (u > 0 && u < original.rows - 1 && v > 0 && v < original.cols - 1) {
				p = interpolationP(original, u, v);
				newImage.at<uchar>(j, i) = (uchar)p;
			}
			else {
				newImage.at<uchar>(j, i) = (uchar)0;
			}
		}
	}

	return newImage;
}