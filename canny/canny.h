#ifndef quaey_canny_h
#define quaey_canny_h

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "nonMaximumSuppression.h"
#include "hysteresisThresholding.h"
#include "sobel.h"

namespace quaey {
    using namespace cv;
    void canny(const Mat & blurred, Mat &magOut, Mat &angOut, float threshold1, float threshold2, int ddepth = CV_32FC1) {
        Mat sobelMag, sobelAng, nmsMag, nmsAng;
        qSobel(blurred, sobelMag, sobelAng, ddepth);
        qNonMaxSuppression(sobelMag, sobelAng, nmsMag, nmsAng);
        hysteresisThresholding(nmsMag, nmsAng, magOut, angOut, threshold1, threshold2);
    }
}

#endif