#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    string originalPath = samples::findFile("images/monalisa.jpg");

    Mat originalImage = imread(originalPath, IMREAD_GRAYSCALE);

    Mat rectImage(512, 512, CV_8UC1, Scalar(255));

    if (originalImage.empty())
    {
        cout << "Could not read the image: " << originalPath << endl;
        return -1;
    }

    int count[256] = {};

    for (int j = 0; j < originalImage.rows; j++) {
        for (int i = 0; i < originalImage.cols; i++) {
            count[int(originalImage.at<uchar>(j, i))] += 1;
        }
    }

    for (int j = 0; j < rectImage.rows; j++) {
        for (int i = 0; i < rectImage.cols; i++) {
            rectImage.at<uchar>(j, i) = (uchar)(0);
        }
    }

    int max = 0;
    for (int i = 0; i < 256; i++)
        if (max < count[i])
            max = count[i];

    int imagePixels = originalImage.rows * originalImage.cols;
    int histogramPixels = 0;
    int val, y, y0;
    Point pnt1, pnt2;

    for (int i = 0; i < 256; i++) {
        val = count[i];
        y = 512 * val / max;
        y0 = 512 - y;
        pnt1 = Point(i * 2+1, 512);
        pnt2 = Point(i*2+1, y0);
        rectangle(rectImage, pnt1, pnt2, Scalar(255, 255, 255), 5, LINE_8, 0);

        cout << i << ") " << val << "   \t";
        histogramPixels += val;
    }

    cout << "\n\n\tHistogram Pixels = " << histogramPixels << "\n\tImage Pixels = " << imagePixels << endl;

    //rectangle(rectImage, Point(50, 200), Point(250, 20), Scalar(255,255,255), 5, LINE_8, 0);
    //rectangle(rectImage, Point(350, 300), Point(500, 500), Scalar(255, 255, 255), FILLED, LINE_8, 0);

    namedWindow("Original", WINDOW_AUTOSIZE); // Can configure the window

    imshow("Original", rectImage);

    

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}