#include <cstdio>
#include <opencv2/opencv.hpp>
#include "canny/canny.h"
#include "quadrangleFormation/houghLines/houghLines.h"
#include "utils/drawUtils.h"
#include "utils/colorConverter.h"

using namespace cv;
using namespace std;
using namespace quaey;

int main()
{
    int ddepth = CV_32FC1;
    Mat image, image2, blurred;

    image = imread( "../img3.jpg", ImreadModes::IMREAD_COLOR);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    Mat grayScale;

    cvtColor(image, grayScale, COLOR_BGR2GRAY);

    GaussianBlur(grayScale, blurred, Size(0, 0), 1.4);

    Mat ang, angOut, mag, magOut;
//    qSobel(grayScale, mag, ang, ddepth);
    quaey::canny(blurred, mag, ang, 30, 20, ddepth);
    convertScaleAbs(mag, magOut);
    convertScaleAbs(ang, angOut);


    vector<quaey::qline> lines;
    double min, max;

    cv::minMaxLoc(magOut, &min, &max);

    quaey::houghLine(magOut, angOut, lines, 3 * CV_PI/180, 3, 40, 100);
    drawLines(lines, image);

    namedWindow("Display Canny Image ",   WINDOW_GUI_NORMAL);
    imshow("Display Canny Image ", magOut);
    namedWindow("Display Original Image ",   WINDOW_GUI_NORMAL);
    imshow("Display Original Image ", image);



    waitKey(0);
    return 0;
}