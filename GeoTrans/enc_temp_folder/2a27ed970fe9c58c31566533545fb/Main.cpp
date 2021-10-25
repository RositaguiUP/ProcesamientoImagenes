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

Mat decrease(Mat, float, float);

int main() {
	string path = samples::findFile("images/cat.jpg");
	Mat originalImage = imread(path, IMREAD_GRAYSCALE);

	Mat reducImage = decrease(originalImage, 0.5, 0.5);
	Mat ampliImage = decrease(originalImage, 1.5, 1.5);

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

Mat decrease(Mat original, float cx, float cy) {
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
