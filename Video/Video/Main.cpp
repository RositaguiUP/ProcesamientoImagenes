#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main() {
    VideoCapture cap(0); // abre la cámara de default

    if (!cap.isOpened())  // revisa si se abrió correctamente
        return -1;
    while (true)
    {
        Mat frame;
        cap >> frame; // obtiene un nuevo cuadro de la cámara

        Mat bwFame(frame.rows, frame.cols, CV_8UC1, Scalar(255));
        cvtColor(frame, bwFame, COLOR_RGB2GRAY);

        #pragma region Threshold
        Mat thresholdImages[5];

        for (int i = 0; i < 5; i++)
            thresholdImages[i] = Mat(bwFame.rows, bwFame.cols, CV_8UC1, Scalar(255));

        int umbral = 128;

        for (int j = 0; j < bwFame.rows; j++) {
            for (int i = 0; i < bwFame.cols; i++) {
                if (bwFame.at<uchar>(j, i) > (uchar)(umbral)) {
                    // Binary umbral
                    thresholdImages[0].at<uchar>(j, i) = (uchar)(255);
                    // Binary invert umbral
                    thresholdImages[1].at<uchar>(j, i) = (uchar)(0);
                    // Trunc
                    thresholdImages[2].at<uchar>(j, i) = (uchar)(255);
                    // Tozero
                    thresholdImages[3].at<uchar>(j, i) = (uchar)(0);
                    // Tozero invert
                    thresholdImages[4].at<uchar>(j, i) = bwFame.at<uchar>(j, i);
                }
                else {
                    // Binary umbral
                    thresholdImages[0].at<uchar>(j, i) = (uchar)(0);
                    // Binary invert umbral
                    thresholdImages[1].at<uchar>(j, i) = (uchar)(255);
                    // Trunc
                    thresholdImages[2].at<uchar>(j, i) = bwFame.at<uchar>(j, i);
                    // Tozero
                    thresholdImages[3].at<uchar>(j, i) = bwFame.at<uchar>(j, i);
                    // Tozero invert
                    thresholdImages[4].at<uchar>(j, i) = (uchar)(0);
                }
            }
        }
        #pragma endregion

        #pragma region Blur
        Mat bluredImage(bwFame.rows, bwFame.cols, CV_8UC1, Scalar(255));

        int sum, prom;
        int var = 25;
        int inter = var / 2;
        int interInf = 0 - inter;
        int interSup = 1 + inter;

        for (int j = inter; j < bluredImage.rows - inter; j++) {
            for (int i = inter; i < bluredImage.cols - inter; i++) {
                sum = 0;
                for (int l = interInf; l < interSup; l++) {
                    for (int k = interInf; k < interSup; k++) {
                        sum += bwFame.at<uchar>(j + l, i + k);
                    }
                }
                prom = sum / (var * var);
                bluredImage.at<uchar>(j, i) = (uchar)(prom);
            }
        }
    
        #pragma endregion


        imshow("frame", bwFame);
        imshow("thereshold", thresholdImages[2]);
        imshow("blur", bluredImage);


        if (waitKey(30) >= 0)
            break; 	//rompe si el usuario oprime una tecla
    }
    return 0;
}
