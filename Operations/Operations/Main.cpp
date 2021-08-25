#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int checkImage(Mat, string);

int main()
{
    string path1 = samples::findFile("images/landscape.jpg");
    string path2 = samples::findFile("images/fox.jpg");
    string path3 = samples::findFile("images/monalisa.jpg");
    Mat image1 = imread(path1, IMREAD_GRAYSCALE);
    Mat image2 = imread(path2, IMREAD_GRAYSCALE);
    Mat image3 = imread(path3, IMREAD_GRAYSCALE);

    if (checkImage(image1, path1))
        return -1;

    if (checkImage(image2, path2))
        return -1;

    

    #pragma region Blend
    int srows = (image1.rows < image2.rows) ? image1.rows : image2.rows;
    int scols = (image1.cols < image2.cols) ? image1.cols : image2.cols;

    Mat blendImage(srows, scols, CV_8UC1, Scalar(255));
    Mat blendWeightColsImage(srows, scols, CV_8UC1, Scalar(255));
    Mat blendWeightRowsImage(srows, scols, CV_8UC1, Scalar(255));

    int pixelColor;
    float w1, w2, inc;

    inc = (float)1/(scols);

    for (int j = 0; j < srows; j++) {
        w1 = 0.0f;
        w2 = 1.0f;

        for (int i = 0; i < scols; i++) {
            pixelColor = (image1.at<uchar>(j, i) + image2.at<uchar>(j, i)) / 2;
            blendImage.at<uchar>(j, i) = (uchar)(pixelColor);

            pixelColor = (w1 * image1.at<uchar>(j, i) + w2 * image2.at<uchar>(j, i));
            blendWeightColsImage.at<uchar>(j, i) = (uchar)(pixelColor);

            w1 += inc;
            w2 -= inc;
        }
    }

    inc = (float)1 / (srows);

    for (int j = 0; j < scols; j++) {
        w1 = 0.0f;
        w2 = 1.0f;

        for (int i = 0; i < srows; i++) {
            pixelColor = (w1 * image1.at<uchar>(i, j) + w2 * image2.at<uchar>(i, j));
            blendWeightRowsImage.at<uchar>(i, j) = (uchar)(pixelColor);

            w1 += inc;
            w2 -= inc;
        }
    }

    imshow("Blend Weight Cols", blendWeightColsImage);
    imshow("Blend Weight Rows", blendWeightRowsImage);
    imshow("Blend", blendImage);
    #pragma endregion
    
    #pragma region AddConst
    Mat addConstImage(image3.rows, image3.cols, CV_8UC1, Scalar(255));

    int constColor = 40;
    for (int j = 0; j < addConstImage.rows; j++) {
        for (int i = 0; i < addConstImage.cols; i++) {
            pixelColor = image3.at<uchar>(j, i) + constColor;
            if (pixelColor > 255)
                pixelColor = 255;
            if (pixelColor < 0)
                pixelColor = 0;

            addConstImage.at<uchar>(j, i) = (uchar)(pixelColor);
        }
    }

    imshow("Original", image3);
    imshow("AddConst", addConstImage);
    #pragma endregion

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
