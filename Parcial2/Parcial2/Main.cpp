#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

Mat threshold(Mat, int);
Mat media(Mat, int);
Mat regions(Mat);
void countFigures(int, map<int, vector<Point>>);

int main() {
	string path = samples::findFile("images/figures.png");
	Mat originalImage = imread(path, IMREAD_GRAYSCALE);

	Mat mediaImage = media(originalImage, 3);
	Mat thresholdImage = threshold(mediaImage, 130);
	Mat growingImage = regions(thresholdImage);


	imshow("Original", originalImage);
	imshow("Media", mediaImage);
	imshow("Threshold", thresholdImage);
	imshow("Regions", growingImage);

	int k = waitKey(0); // Wait for a keystroke in the window
	if (k == 's')
	{
		imwrite("Original.png", originalImage);
		imwrite("Media.png", mediaImage);
		imwrite("Threshold.png", thresholdImage);
		imwrite("Regions.png", growingImage);
	}
	return 0;
}

Mat threshold(Mat original, int umbral) {
	Mat newImage(original.rows, original.cols, CV_8UC1);

	for (int j = 0; j < original.rows; j++) {
		for (int i = 0; i < original.cols; i++) {
			if (original.at<uchar>(j, i) > umbral) {
				newImage.at<uchar>(j, i) = (uchar)(255);
			}
			else {
				newImage.at<uchar>(j, i) = (uchar)(0);
			}
		}
	}

	return newImage;
}

Mat media(Mat original, int var) {
	Mat newImage(original.rows, original.cols, CV_8UC1, Scalar(255));

	int mid = (var * var) / 2;
	int inter = var / 2;
	int interInf = 0 - inter;
	int interSup = 1 + inter;
	vector<int> vecPixels;

	for (int j = inter; j < original.rows - inter; j++) {
		for (int i = inter; i < original.cols - inter; i++) {
			vecPixels.clear();
			for (int l = interInf; l < interSup; l++) {
				for (int k = interInf; k < interSup; k++) {
					vecPixels.push_back(original.at<uchar>(j + l, i + k));
				}
			}
			sort(vecPixels.begin(), vecPixels.end());
			newImage.at<uchar>(j, i) = (uchar)(vecPixels[mid]);
		}
	}

	return newImage;
}

Mat regions(Mat original) {
	Mat newImage(original.rows, original.cols, CV_8UC1);

	vector<vector<int>>visited(original.rows, vector<int>(original.cols, 0));
	queue<Point> vecinos;
	map<int, vector<Point> > regions;
	vector<Point> regionPoints;
	int cont = 1;
	int color = 15;

	for (int j = 0; j < original.rows; j++) {
		for (int i = 0; i < original.cols; i++) {
			if (original.at<uchar>(j, i) == 255 && visited[j][i] == 0) {
				visited[j][i] = cont;
				newImage.at<uchar>(j, i) = (uchar)color;
				if (i - 1 >= 0) vecinos.push(Point(j, i - 1));
				if (i + 1 < original.cols) vecinos.push(Point(j, i + 1));
				if (j - 1 >= 0) vecinos.push(Point(j - 1, i));
				if (j + 1 < original.rows) vecinos.push(Point(j + 1, i));

				regionPoints.clear();
				while (!vecinos.empty()) {
					Point pnt = vecinos.front();
					vecinos.pop();
					if (original.at<uchar>(pnt.x, pnt.y) == 255 && visited[pnt.x][pnt.y] == 0) {
						visited[pnt.x][pnt.y] = cont;
						newImage.at<uchar>(pnt.x, pnt.y) = (uchar)color;

						if (pnt.y - 1 >= 0) vecinos.push(Point(pnt.x, pnt.y - 1));
						if (pnt.y + 1 < original.cols) vecinos.push(Point(pnt.x, pnt.y + 1));
						if (pnt.x - 1 >= 0) vecinos.push(Point(pnt.x - 1, pnt.y));
						if (pnt.x + 1 < original.rows) vecinos.push(Point(pnt.x + 1, pnt.y));

					}
					regionPoints.push_back(pnt);
				}
				regions[cont] = regionPoints;
				color += 15;
				cont++;
			}
		}
	}

	countFigures(cont, regions);

	return newImage;
}

void countFigures(int total, map<int, vector<Point>> regions) {
	int smallCircles = 0;
	int rectangles = 0;
	int bigCircles = 0;
	int size;

	for (int i = 1; i < total; i++) {
		size = regions[i].size();
		cout << i << ") " << size << endl;
		if (size >= 3100 && size <= 3120){ smallCircles++; }
		else if (size >= 5950 && size < 5990) { rectangles++; }
		else if (size >= 15930 && size < 16000) { bigCircles++; }
	}

	cout << "\nSmall circles: " << smallCircles << "\nBig circles: " << bigCircles << "\nRectangles: " << rectangles << endl;
}