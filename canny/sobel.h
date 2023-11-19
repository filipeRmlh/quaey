#ifndef quaey_sobel_h
#define quaey_sobel_h

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include "utils/accessUtils.h"

namespace quaey {
    using namespace cv;

    void qSobel(const Mat &img, Mat &mag, Mat &ang, const int ddepth) {
        Mat gradX, gradY, outMag, outAng;
        Sobel(img, gradX, ddepth, 1, 0, 3);
        Sobel(img, gradY, ddepth, 0, 1, 3);
        cartToPolar(gradX, gradY, mag, ang);
    }
}

#endif