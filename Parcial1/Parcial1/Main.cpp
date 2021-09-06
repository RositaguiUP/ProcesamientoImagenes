#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int blendImages();
int checkImage(Mat, string);

int main()
{
   /* Mat imageL = imread("L.jpg", IMREAD_GRAYSCALE);   // Read the file
    Mat imageK = imread("K.jpg", IMREAD_GRAYSCALE);   // Read the file
    Mat imageC = imread("C.jpg", IMREAD_GRAYSCALE);   // Read the file
    Mat imageR = imread("R.jpeg", IMREAD_GRAYSCALE);   // Read the file
    if (!imageL.data || !imageK.data || !imageC.data || !imageR.data)                          	// Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    int umbral = 100;

    Mat binaryL(imageL.rows, imageL.cols, CV_8UC1);
    Mat binaryK(imageK.rows, imageK.cols, CV_8UC1);
    Mat binaryC(imageC.rows, imageC.cols, CV_8UC1);
    Mat binaryR(imageR.rows, imageR.cols, CV_8UC1);
    Mat binaryINVL(imageL.rows, imageL.cols, CV_8UC1);
    Mat binaryINVK(imageK.rows, imageK.cols, CV_8UC1);
    Mat binaryINVC(imageC.rows, imageC.cols, CV_8UC1);
    Mat binaryINVR(imageR.rows, imageR.cols, CV_8UC1);

    for (int i = 0; i < imageL.rows; i++) {
        for (int j = 0; j < imageL.cols; j++) {
            // L
            if (imageL.at<uchar>(i, j) > umbral) {
                binaryL.at<uchar>(i, j) = (uchar)(255);
                binaryINVL.at<uchar>(i, j) = (uchar)(0);
            }
            else {
                binaryL.at<uchar>(i, j) = (uchar)(0);
                binaryINVL.at<uchar>(i, j) = (uchar)(255);
            }

            // K
            if (imageK.at<uchar>(i, j) > umbral) {
                binaryK.at<uchar>(i, j) = (uchar)(255);
                binaryINVK.at<uchar>(i, j) = (uchar)(0);
            }
            else {
                binaryK.at<uchar>(i, j) = (uchar)(0);
                binaryINVK.at<uchar>(i, j) = (uchar)(255);
            }

            // C
            if (imageC.at<uchar>(i, j) > umbral) {
                binaryC.at<uchar>(i, j) = (uchar)(255);
                binaryINVC.at<uchar>(i, j) = (uchar)(0);
            }
            else {
                binaryC.at<uchar>(i, j) = (uchar)(0);
                binaryINVC.at<uchar>(i, j) = (uchar)(255);
            }

            // R
            if (imageR.at<uchar>(i, j) > umbral) {
                binaryR.at<uchar>(i, j) = (uchar)(255);
                binaryINVR.at<uchar>(i, j) = (uchar)(0);
            }
            else {
                binaryR.at<uchar>(i, j) = (uchar)(0);
                binaryINVR.at<uchar>(i, j) = (uchar)(255);
            }
        }

    }
    namedWindow("Image", WINDOW_AUTOSIZE);// Create a window for display.
    imshow("BinaryL", binaryL);               	// Show our image inside it.
    imshow("BinaryK", binaryK);               	// Show our image inside it.
    imshow("BinaryC", binaryC);               	// Show our image inside it.
    imshow("BinaryR", binaryR);               	// Show our image inside it.
    imshow("Binary InvertedL", binaryINVL);               	// Show our image inside it.
    imshow("Binary InvertedK", binaryINVK);               	// Show our image inside it.
    imshow("Binary InvertedC", binaryINVC);               	// Show our image inside it.
    imshow("Binary InvertedR", binaryINVR);               	// Show our image inside it.*/

    //waitKey(0);                                      	// Wait for a keystroke in the window

    blendImages();
    return 0;
}

int blendImages() {
    string path1 = samples::findFile("images/uni2.jpg");
    string path2 = samples::findFile("images/uni1.jpg");
    Mat image1 = imread(path1, IMREAD_GRAYSCALE);
    Mat image2 = imread(path2, IMREAD_GRAYSCALE);

    if (checkImage(image1, path1))
        return -1;

    if (checkImage(image2, path2))
        return -1;

    int srows = (image1.rows < image2.rows) ? image1.rows : image2.rows;
    int scols = (image1.cols < image2.cols) ? image1.cols : image2.cols;

    Mat blendImage(srows, scols, CV_8UC1, Scalar(255));
    Mat blendWeightColsImage(srows, scols, CV_8UC1, Scalar(255));
    Mat blendWeightRowsImage(srows, scols, CV_8UC1, Scalar(255));

    int pixelColor;
    float w1, w2, inc;

    inc = (float)1 / (scols);

    for (int j = 0; j < srows; j++) {
        w1 = 0.0f;
        w2 = 1.0f;

        for (int i = 0; i < scols; i++) {
            pixelColor = (w1 * image1.at<uchar>(j, i) + w2 * image2.at<uchar>(j, i));
            blendWeightColsImage.at<uchar>(j, i) = (uchar)(pixelColor);

            w1 += inc;
            w2 -= inc;
        }
    }

    imshow("Blend Weight Cols", blendWeightColsImage);

    int k = waitKey(0); // Wait for a keystroke in the window
    if (k == 's')
    {
        imwrite("Edificios.png", blendWeightColsImage);
    }
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
