#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    string originalPath = samples::findFile("images/monalisa.jpg");

    Mat originalImage = imread(originalPath, IMREAD_GRAYSCALE);


    if (originalImage.empty())
    {
        cout << "Could not read the image: " << originalPath << endl;
        return -1;
    }

    Mat thresholdImages[5];
    
    for (int i = 0; i < 5; i++)
        thresholdImages[i] = Mat(originalImage.rows, originalImage.cols, CV_8UC1, Scalar(255));

    int umbral = 128;

    for (int j = 0; j < originalImage.rows; j++) {
        for (int i = 0; i < originalImage.cols; i++) {
            if (originalImage.at<uchar>(j, i) > (uchar)(umbral)) {
                // Binary umbral
                thresholdImages[0].at<uchar>(j, i) = (uchar)(255);
                // Binary invert umbral
                thresholdImages[1].at<uchar>(j, i) = (uchar)(0);
                // Trunc
                thresholdImages[2].at<uchar>(j, i) = (uchar)(255);
                // Tozero
                thresholdImages[3].at<uchar>(j, i) = (uchar)(0);
                // Tozero invert
                thresholdImages[4].at<uchar>(j, i) = originalImage.at<uchar>(j, i);
            }
            else {
                // Binary umbral
                thresholdImages[0].at<uchar>(j, i) = (uchar)(0);
                // Binary invert umbral
                thresholdImages[1].at<uchar>(j, i) = (uchar)(255);
                // Trunc
                thresholdImages[2].at<uchar>(j, i) = originalImage.at<uchar>(j, i);
                // Tozero
                thresholdImages[3].at<uchar>(j, i) = originalImage.at<uchar>(j, i);
                // Tozero invert
                thresholdImages[4].at<uchar>(j, i) = (uchar)(0);
            }
        }
    }

    namedWindow("Original", WINDOW_AUTOSIZE); // Can configure the window

    imshow("Original", originalImage);
    imshow("Binary", thresholdImages[0]);
    imshow("Binary Invert", thresholdImages[1]);
    imshow("Trunc", thresholdImages[2]);
    imshow("Tozero", thresholdImages[3]);
    imshow("Tozero invert", thresholdImages[4]);

    
    waitKey(0); // Wait for a keystroke in the window
    return 0;
}