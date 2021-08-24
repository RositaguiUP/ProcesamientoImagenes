#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int checkImage(Mat, string);
Mat createHistogram(Mat, int[]);
Mat ecualizeImage(Mat, int[]);

int main()
{
    string originalPath = samples::findFile("images/subImg.jpg");
    Mat originalImage = imread(originalPath, IMREAD_GRAYSCALE);

    if (checkImage(originalImage, originalPath))
        return -1;
    
    //int* originalGrayScaleValues = getGrayScaleValues(originalImage);

    int originalGrayScaleValues[256] = {};

    for (int j = 0; j < originalImage.rows; j++) {
        for (int i = 0; i < originalImage.cols; i++) {
            originalGrayScaleValues[int(originalImage.at<uchar>(j, i))] += 1;
        }
    }

    Mat equalizedImage = ecualizeImage(originalImage, originalGrayScaleValues);
    Mat originalHistogram = createHistogram(originalImage, originalGrayScaleValues);

    int equalizedGrayScaleValues[256] = {};

    for (int j = 0; j < equalizedImage.rows; j++) {
        for (int i = 0; i < equalizedImage.cols; i++) {
            equalizedGrayScaleValues[int(equalizedImage.at<uchar>(j, i))] += 1;
        }
    }

    Mat ecualizedHistogram = createHistogram(equalizedImage, equalizedGrayScaleValues);
    
    imshow("Original", originalImage);
    imshow("Ecualization", equalizedImage);
    imshow("Original Histogram", originalHistogram);
    imshow("Ecualized Histogram", ecualizedHistogram);
    

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

Mat createHistogram(Mat originalImage, int grayScaleValues[]) {
    Mat histogramImage(512, 512, CV_8UC1, Scalar(255));

    for (int j = 0; j < histogramImage.rows; j++) {
        for (int i = 0; i < histogramImage.cols; i++) {
            histogramImage.at<uchar>(j, i) = (uchar)(0);
        }
    }

    int max = 0;
    for (int i = 0; i < 256; i++)
        if (max < grayScaleValues[i])
            max = grayScaleValues[i];

    int imagePixels = originalImage.rows * originalImage.cols;
    int histogramPixels = 0;
    int val, y, y0;
    Point pnt1, pnt2;

    for (int i = 0; i < 256; i++) {
        val = grayScaleValues[i];
        y = 512 * val / max;
        y0 = 512 - y;
        pnt1 = Point(i * 2 + 1, 512);
        pnt2 = Point(i * 2 + 1, y0);
        rectangle(histogramImage, pnt1, pnt2, Scalar(255, 255, 255), 5, LINE_8, 0);

        // cout << i << ") " << val << "   \t";
        histogramPixels += val;
    }

    // cout << "\n\n\tHistogram Pixels = " << histogramPixels << "\n\tImage Pixels = " << imagePixels << endl;
    return histogramImage;
}

Mat ecualizeImage(Mat originalImage, int grayScaleValues[]) {
    Mat equalizedImage(originalImage.rows, originalImage.cols, CV_8UC1, Scalar(255));

    int imagePixels = originalImage.rows * originalImage.cols;

    float pmfValues[256] = {};
    float cdfValues[256] = {};

    for (int i = 0; i < 256; i++) {
        pmfValues[i] = (float)(grayScaleValues[i]) / imagePixels;
        cdfValues[i] = pmfValues[i];

        if (i != 0)
            cdfValues[i] += cdfValues[i - 1];

        // cout << "pmf: " << pmfValues[i] << "\tcdf" << cdfValues[i] << endl;
    }

    int grayTone;

    for (int j = 0; j < originalImage.rows; j++) {
        for (int i = 0; i < originalImage.cols; i++) {
            grayTone = int(originalImage.at<uchar>(j, i));
            equalizedImage.at<uchar>(j, i) = (uchar)floor((256 - 1) * (cdfValues[grayTone]));
        }
    }

    return equalizedImage;
}