#ifndef quaey_draw_utils
#define quaey_draw_utils

#include "accessUtils.h"
#include <opencv2/opencv.hpp>

namespace quaey {

    using namespace cv;
    using namespace std;
    using namespace quaey;

    template<typename T, typename Q> T convertRange(T max, Q val)  {
        return ((T)(max + (max / CV_2PI) * val)) % max;
    }


    int convertParametricCartesian(int x, float theta, float rho) {
        auto s = sin(theta);
        auto c = cos(theta);
        auto a = -1 * c/s;
        return a * x + rho/s;
    }

    /*
     * int rhoResult = cvRound((float) x * sin +
                                 (float) y * cos);
     */

    void drawLines(vector<qline> & lines, Mat & imageCanvas) {
        for(auto & i : lines)
        {
            double rho = i.rho, theta = i.theta;
            int dir = i.dir;
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 10000*(-b));
            pt1.y = cvRound(y0 + 10000*(a));
            pt2.x = cvRound(x0 - 10000*(-b));
            pt2.y = cvRound(y0 - 10000*(a));
            Scalar colorPurple(128, 0, 255);
            Scalar colorCyan(255, 255, 0);
            line( imageCanvas, pt1, pt2, dir == 1 ? colorCyan : colorPurple, 1, LINE_AA);
        }
        line( imageCanvas, Point(0, imageCanvas.rows/2), Point(imageCanvas.cols, imageCanvas.rows/2), Scalar(0,0,250), 1, LINE_AA);
        line( imageCanvas, Point(imageCanvas.cols/2, 0), Point(imageCanvas.cols/2, imageCanvas.rows), Scalar(0,0,250), 1, LINE_AA);
    }
}

#endif
