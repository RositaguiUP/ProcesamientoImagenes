#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Mat bordes(Mat original);

int main() {
	string path = samples::findFile("images/monalisa.jpg");
	Mat originalImage = imread(path, IMREAD_GRAYSCALE);

	Mat bordesImage = bordes(originalImage);

	imshow("Original", originalImage);
	imshow("Bordes", bordesImage);

	int k = waitKey(0); // Wait for a keystroke in the window
	return 0;
}

Mat bordes(Mat original) {
	Mat newImage(original.rows, original.cols, CV_8UC1);
	Mat gx(original.rows, original.cols, CV_8UC1);
	Mat gy(original.rows, original.cols, CV_8UC1);

	int sumx;
	int sumy;
	int maskx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int masky[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

	for (int j = 1; j < original.rows - 1; j++) {
		for (int i = 1; i < original.cols - 1; i++) {
			sumx = 0;
			sumy = 0;
			for (int l = -1; l <= 1; l++) {
				for (int k = -1; k <= 1; k++) {
					sumx += original.at<uchar>(j + l, i + k) * maskx[l + 1][k + 1];
					sumy += original.at<uchar>(j + l, i + k) * masky[l + 1][k + 1];
				}
			}
			gx.at<uchar>(j, i) = (uchar)abs(sumx);
			gy.at<uchar>(j, i) = (uchar)abs(sumy);
			newImage.at<uchar>(j, i) = (uchar)sqrt(pow(sumx, 2) + pow(sumy, 2));
		}
	}

	imshow("gx", gx);
	imshow("gy", gy);

	return newImage;
}