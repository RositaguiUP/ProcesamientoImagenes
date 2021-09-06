#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int checkImage(Mat, string);

int main()
{
    string path1 = samples::findFile("images/littleCat.jpg");
    string path2 = samples::findFile("images/pimienta.png");
    Mat image1 = imread(path1, IMREAD_GRAYSCALE);
    Mat image2 = imread(path2, IMREAD_GRAYSCALE);

    if (checkImage(image1, path1))
        return -1;

    Mat bluredImage(image1.rows, image1.cols, CV_8UC1, Scalar(255));
    Mat bluredVarImage(image1.rows, image1.cols, CV_8UC1, Scalar(255));
    Mat pimientaImage(image2.rows, image2.cols, CV_8UC1, Scalar(255));

    int sum, prom;
    for (int j = 1; j < bluredImage.rows-1; j++) {
        for (int i = 1; i < bluredImage.cols-1; i++) {
            sum = 0;
            for (int l = -1; l < 2; l++) {
                for (int k = -1; k < 2; k++) {
                    sum += image1.at<uchar>(j + l, i + k);
                }
            }
            prom = sum/9;
            bluredImage.at<uchar>(j, i) = (uchar)(prom);
        }
    }

    int var = 7;
    int inter = var / 2;
    int interInf = 0 - inter;
    int interSup = 1 + inter;

    for (int j = inter; j < bluredVarImage.rows - inter; j++) {
        for (int i = inter; i < bluredVarImage.cols - inter; i++) {
            sum = 0;
            for (int l = interInf; l < interSup; l++) {
                for (int k = interInf; k < interSup; k++) {
                    sum += image1.at<uchar>(j + l, i + k);
                }
            }
            prom = sum / (var*var);
            bluredVarImage.at<uchar>(j, i) = (uchar)(prom);
        }
    }

    var = 3;
    int mid = (var * var) / 2;
    inter = var / 2;
    interInf = 0 - inter;
    interSup = 1 + inter;
    vector<int> vecPixels;
    for (int j = inter; j < pimientaImage.rows - inter; j++) {
        for (int i = inter; i < pimientaImage.cols - inter; i++) {
            vecPixels.clear();
            for (int l = interInf; l < interSup; l++) {
                for (int k = interInf; k < interSup; k++) {
                    vecPixels.push_back(image2.at<uchar>(j + l, i + k));
                }
            }
            sort(vecPixels.begin(), vecPixels.end());
            pimientaImage.at<uchar>(j, i) = (uchar)(vecPixels[mid]);
        }
    }



    //imshow("Original", image1);
    //imshow("Blur 3x3", bluredImage);
    //imshow("Blur Variable (7x7)", bluredVarImage);

    imshow("Original", image2);
    imshow("Pimienta filter", pimientaImage);

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}

int checkImage(Mat image, string path) {
    if (image.empty())
    {
        cout << "Could not read the image: " << path << endl;
        return -1;
    }

    return 0;
}
