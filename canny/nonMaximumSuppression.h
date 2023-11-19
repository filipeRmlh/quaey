#ifndef quaey_nonMaximumSuppression_h
#define quaey_nonMaximumSuppression_h

#include "utils/accessUtils.h"
#include <opencv2/core.hpp>

namespace quaey {

    using namespace cv;

// Angle range in the sections to be divided
// Note: the divisions are relative to the trigonometric circle, from 0 to 2PI.
    const float ang_divide = CV_2PI / 8;

// Mapping arrays that given an angle section. Gives the correspondent i, j neighbor cell, given the section position.
    const int mapIndexJ[] = { 1,  1,  0, -1, -1, -1,  0,  1,  1};
    const int mapIndexI[] = { 0,  1,  1,  1,  0, -1, -1, -1,  0};

/** Compare the neighbors in the angle direction.
 * @param i: the pixel i coord in the image matrix.
 * @param j: the pixel j coord in the image matrix.
 * @param m: the magnitude input matrix
 * @param a: the angle input matrix
 * @param rows: the number of rows of the image
 * @param cols: the number of columns of the image
 * @return if the analyzed pixel (the center pixel in the mask) has the stronger magnitude over the analysed direction in the mask scope.
 */
    bool qIsGreater(const float *m, const float *a, const int i, const int j, const int cols, const int rows) {
        float ang = pixelValue(a, cols, 1, i, j, 0);
        int place = (int)(ang / ang_divide);
        int maskI = mapIndexI[place];
        int maskJ = mapIndexJ[place];
        return limitCell(m, rows, cols, 1, i + maskI, j + maskJ, 0) < pixelValue(m, cols, 1, i, j, 0)
               && limitCell(m, rows, cols, 1, i - maskI, j - maskJ, 0) < pixelValue(m, cols, 1, i, j, 0);
    }

    void qNonMaxSuppression(Mat &magIn, Mat &angIn, Mat &magOut, Mat &angOut) {

        assert(magIn.cols == angIn.cols && magIn.rows == angIn.rows);

        magOut.create(magIn.rows, magIn.cols, magIn.type());
        angOut.create(angIn.rows, angIn.cols, angIn.type());

        auto *dataMagOut = magOut.ptr<float>();
        auto *dataAngOut = angOut.ptr<float>();
        auto *dataMagIn = magIn.ptr<float>();
        auto *dataAngIn = angIn.ptr<float>();

        auto cols = magIn.cols;
        auto rows = magIn.rows;

        for (auto i = 0; i < rows; i++) {
            for (auto j = 0; j < cols; j++) {
                auto isGreater = qIsGreater(dataMagIn, dataAngIn, i, j, cols, rows);
                pixelValue(dataMagOut, cols, 1, i, j, 0) = isGreater ? pixelValue(dataMagIn, cols, 1, i, j, 0) : 0;
                pixelValue(dataAngOut, cols, 1, i, j, 0) = isGreater ? pixelValue(dataAngIn, cols, 1, i, j, 0) : 0;
            }
        }
    }
}

#endif