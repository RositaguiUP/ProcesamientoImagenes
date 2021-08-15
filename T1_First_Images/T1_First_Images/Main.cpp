#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
int main()
{
    std::string image_path1 = samples::findFile("images/landscape.jpg");
    std::string image_path2 = samples::findFile("images/fox.jpg");
    std::string image_path3 = samples::findFile("images/triangles.jpg");
    Mat img1 = imread(image_path1, IMREAD_COLOR);
    if (img1.empty())
    {
        std::cout << "Could not read the image: " << image_path1 << std::endl;
        return 1;
    }
    imshow("Landscape", img1);
    Mat img2 = imread(image_path2, IMREAD_COLOR);
    if (img2.empty())
    {
        std::cout << "Could not read the image: " << image_path2 << std::endl;
        return 1;
    }
    imshow("Fox", img2);
    Mat img3 = imread(image_path3, IMREAD_COLOR);
    if (img3.empty())
    {
        std::cout << "Could not read the image: " << image_path3 << std::endl;
        return 1;
    }
    imshow("Triangles", img3);
    int k = waitKey(0); // Wait for a keystroke in the window
    if (k == 's')
    {
        imwrite("starry_night.png", img1);
    }
    return 0;
}