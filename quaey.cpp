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

    GaussianBlur(grayScale, blurred, Size(0, 0), 1.2);

    Mat ang, angOut, mag, magOut;

    quaey::canny(blurred, mag, ang, 40, 30, ddepth);

    vector<quaey::qline> lines;

    quaey::houghLine(mag, ang, lines, 10 * CV_PI/180, 5, 50, 80);
    drawLines(lines, image);

    namedWindow("Display Original Image ",   WINDOW_GUI_NORMAL);
    imshow("Display Original Image ", image);

    waitKey(0);
    return 0;
}