#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat images[8];
    int cols = 500;
    int rows = 500;

    for (int i = 0; i < 8; i++)
        images[i] = Mat(rows, cols, CV_8UC1, Scalar(255));

    int c, ci, cj;

    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
            // a
            images[0].at<uchar>(j, i) = (uchar)(0);

            // b
            if ((j < rows/2 && i < cols/2) || (j >= rows / 2 && i >= cols / 2))
                images[1].at<uchar>(j, i) = (uchar)(0);

            //c
            if (i % 2 == 0)
                images[2].at<uchar>(j, i) = (uchar)(0);

            //d
            if (j % 2 == 0)
                images[3].at<uchar>(j, i) = (uchar)(0);
            
            //e
            if (j < 100 || (j >= 200 && j < 300) || j >= 400) {
                if (i < 100 || (i >= 200 && i < 300) || i >= 400)
                    images[4].at<uchar>(j, i) = (uchar)(0);
            }
            else {
                if ((i >= 100 && i < 200) || (i >= 300 && i < 400))
                    images[4].at<uchar>(j, i) = (uchar)(0);
            }

            //f
            if ((j >= 64 && j < 436) && ((i >= 64 && i < 128) || (i >= 372 && i < 436))
                || ((j >= 64 && j < 128) || (j >= 372 && j < 436)) && (i >= 128 && i < 372))
                images[5].at<uchar>(j, i) = (uchar)(0);

            //g
            c = i % 2 == 0 ? i / 2 : (i + 1) / 2;
            images[6].at<uchar>(j, i) = (uchar)(c);

            //h
            if ((510-j) < i) {
                ci = i % 2 == 0 ? i / 2 : (i + 1) / 2;
                cj = j % 2 == 0 ? j / 2 : (j + 1) / 2;
                images[7].at<uchar>(j, i) = (uchar)(255 - (ci + cj));
            }
        }
    }

    imshow("Img a", images[0]);
    imshow("Img b", images[1]);
    imshow("Img c", images[2]);
    imshow("Img d", images[3]);
    imshow("Img e", images[4]);
    imshow("Img f", images[5]);
    imshow("Img g", images[6]);
    imshow("Img h", images[7]);
    
    int k = waitKey(0); // Wait for a keystroke in the window
    /*if (k == 's')
    {
        imwrite("img1.png", images[0]);
    }*/
    return 0;
}