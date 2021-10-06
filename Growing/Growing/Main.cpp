#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <queue>
#include <vector>

using namespace cv;
using namespace std;

int main() {
	string path1 = samples::findFile("images/coraje.jpg");
	Mat image1 = imread(path1, IMREAD_GRAYSCALE);
	Mat twoColorsImage(image1.rows, image1.cols, CV_8UC1);
	Mat growingImage(image1.rows, image1.cols, CV_8UC1, Scalar(255));

	for (int j = 0; j < image1.rows; j++) {
		for (int i = 0; i < image1.cols; i++) {
			if (image1.at<uchar>(j, i) > 210) {
				twoColorsImage.at<uchar>(j, i) = (uchar)(0);
			}
			else {
				twoColorsImage.at<uchar>(j, i) = (uchar)(255);
			}
		}
	}

	vector<vector<int>>visited(twoColorsImage.rows, vector<int>(twoColorsImage.cols, 0));
	queue<Point> vecinos;

	int cont = 1;
	int color = 50;
	for (int j = 0; j < image1.rows; j++) {
		for (int i = 0; i < image1.cols; i++) {
			if (twoColorsImage.at<uchar>(j, i) == 255 && visited[j][i] == 0) {
				visited[j][i] = cont;
				growingImage.at<uchar>(j, i) = (uchar)color;
				if (i - 1 >= 0) vecinos.push(Point(j, i - 1));
				if (i + 1 < twoColorsImage.cols) vecinos.push(Point(j, i + 1));
				if (j - 1 >= 0) vecinos.push(Point(j - 1, i));
				if (j + 1 < twoColorsImage.rows) vecinos.push(Point(j + 1, i));

				while (!vecinos.empty()) {
					Point pnt = vecinos.front();
					vecinos.pop();
					if (twoColorsImage.at<uchar>(pnt.x, pnt.y) == 255 && visited[pnt.x][pnt.y] == 0) {
						visited[pnt.x][pnt.y] = cont;
						growingImage.at<uchar>(pnt.x, pnt.y) = (uchar)color;

						if (pnt.y - 1 >= 0) vecinos.push(Point(pnt.x, pnt.y - 1));
						if (pnt.y + 1 < twoColorsImage.cols) vecinos.push(Point(pnt.x, pnt.y + 1));
						if (pnt.x - 1 >= 0) vecinos.push(Point(pnt.x - 1, pnt.y));
						if (pnt.x + 1 < twoColorsImage.rows) vecinos.push(Point(pnt.x + 1, pnt.y));

					}
				}

				color += 25;
				cont++;
			}
		}
	}

	cout << cont;
	imshow("Original", image1);
	imshow("Image B&W", twoColorsImage);
	imshow("Image Growing", growingImage);


	waitKey(0); // Wait for a keystroke in the window
	return 0;
}