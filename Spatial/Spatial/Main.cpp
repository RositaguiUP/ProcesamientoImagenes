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
    Mat bluredGaussImage(image1.rows, image1.cols, CV_8UC1, Scalar(255));
    Mat thresholdImage(image1.rows, image1.cols, CV_8UC1, Scalar(255));
    Mat thresholdBluredImage(image1.rows, image1.cols, CV_8UC1, Scalar(255));

    int umbral = 128;
    int sum, prom;
    for (int j = 1; j < bluredImage.rows - 1; j++) {
        for (int i = 1; i < bluredImage.cols - 1; i++) {
            sum = 0;
            for (int l = -1; l < 2; l++) {
                for (int k = -1; k < 2; k++) {
                    sum += image1.at<uchar>(j + l, i + k);
                }
            }
            prom = sum / 9;
            bluredImage.at<uchar>(j, i) = (uchar)(prom);

            if (image1.at<uchar>(j, i) > (uchar)(umbral)) {
                // Tozero invert
                thresholdImage.at<uchar>(j, i) = image1.at<uchar>(j, i);
            }
            else {
                // Tozero invert
                thresholdImage.at<uchar>(j, i) = (uchar)(0);
            }
        }
    }


    int var = 5;
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
            prom = sum / (var * var);
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

    int tot = 159;
    int sum2 = 0;
    var = 5;
    inter = var / 2;
    interInf = 0 - inter;
    interSup = 1 + inter;
    int mask[5][5] = { {2, 4, 5, 4, 2}, {4, 9, 12, 9, 4}, {5, 12, 15, 12, 5}, {4, 9, 12, 9, 4}, {2, 4, 5, 4, 2} };
        // { {1, 2, 1 }, {2, 4, 2 }, {1, 2, 1 }}; 16 3x3
        // { {1, 4, 7, 4, 1 }, {4, 16, 26, 16, 4}, {7, 26, 41, 26, 7}, {4, 16, 26, 16, 4}, {1, 4, 7, 4, 1 } }; 273 5x5
        // { {2, 4, 5, 4, 2}, {4, 9, 12, 9, 4}, {5, 12, 15, 12, 5}, {4, 9, 12, 9, 4}, {2, 4, 5, 4, 2}}; 159 5x5
    for (int j = inter; j < bluredGaussImage.rows - inter; j++) {
        for (int i = inter; i < bluredGaussImage.cols - inter; i++) {
            sum = 0;
            sum2 = 0;
            for (int l = interInf; l < interSup; l++) {
                for (int k = interInf; k < interSup; k++) {
                    sum += image1.at<uchar>(j + l, i + k) * mask[interSup+l-1][interSup+k-1];
                    sum2 += thresholdImage.at<uchar>(j + l, i + k) * mask[interSup + l - 1][interSup + k - 1];
                }
            }
            prom = sum / tot;
            bluredGaussImage.at<uchar>(j, i) = (uchar)(prom);
            prom = sum2 / tot;
            thresholdBluredImage.at<uchar>(j, i) = (uchar)(prom);
        }
    }


    imshow("Original", image1);
    // imshow("Blur 3x3", bluredImage);
    imshow("Blur Variable (5x5)", bluredVarImage);

    //imshow("Original", image2);
    //imshow("Pimienta filter", pimientaImage);

    imshow("Gauss Blur 5x5", bluredGaussImage);
    imshow("Threshold", thresholdImage);
    imshow("Threshold Gauss Blur 5x5", thresholdBluredImage);

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
