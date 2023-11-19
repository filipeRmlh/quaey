#ifndef quaey_houghLines_h
#define quaey_houghLines_h

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "utils/accessUtils.h"

typedef struct {
    double cos;
    double sin;
    double ang;
} trig;



namespace quaey {

    using namespace cv;
    using namespace std;

    enum direction {forward = 0, backward = 1};

    typedef struct {
        int rho;
        double theta;
        int dir;
    } qline;

    void createTrigTable(vector<trig> & trigTable, int size, double resolution) {
        for(int i = 0; i < size; i++) {
            auto angle = i * resolution;
            trig result = {
                sin(angle),
                cos(angle),
                angle
            };
            trigTable.push_back(result);
        }
    }

    void houghLine(Mat & src, Mat & srcAng, vector<qline> & dst, double thetaRes, int rhoRes, float minIntensity = 1, int minVotes = 3) {
        auto srcData = src.ptr<float>();
        auto angData = srcAng.ptr<float>();
        int thetaSize = CV_PI / thetaRes;
        auto srcCols = src.cols;
        auto srcRows = src.rows;
        int maxRho = (int) srcCols+srcRows;
        int minRho = -maxRho;
        int rhoSize = maxRho - minRho;
        vector<trig> trigTable;
        createTrigTable(trigTable, thetaSize, thetaRes);
        int acc [thetaSize * rhoSize * 2];
        memset(acc, 0, (sizeof(int) *(thetaSize*rhoSize)));
        for (auto x = 0; x < srcRows; x++) {
            for (auto y = 0; y < srcCols; y++) {
                auto intensity = pixelValue(srcData, srcCols, src.channels(), x, y, 0);
                if(intensity >= minIntensity) {
                    auto pixelDirection = pixelValue(angData, srcCols, src.channels(), x, y, 0) > CV_PI ? forward : backward;
                    for (int thetaIndex = 0; thetaIndex < thetaSize; thetaIndex++) {
                        auto sinT = trigTable[thetaIndex].sin;
                        auto cosT = trigTable[thetaIndex].cos;
                        auto rhoIndex = x * cosT + y * sinT;
                        int transformedRho = (int)(cvRound(rhoIndex) + maxRho);
                        int accVotesIndex = arrIndex(rhoSize, 2, thetaIndex, transformedRho, pixelDirection);
                        acc[accVotesIndex]++;
                    }
                }
            }
        }

        for (int thetaIndex = 0; thetaIndex < thetaSize; thetaIndex++) {
            for (int rhoIndex = 0; rhoIndex < rhoSize; rhoIndex++) {

                    int accIndexDir1 = arrIndex(rhoSize, 2, thetaIndex, rhoIndex, forward);
                    int accIndexDir2 = arrIndex(rhoSize, 2, thetaIndex, rhoIndex, backward);
                    int votes1 = acc[accIndexDir1];
                    int votes2 = acc[accIndexDir2];
                    int votes = votes1 + votes2;
                    int dir = votes1 > votes2 ? forward : backward;
                    if(votes >= minVotes) {
                        auto ang = trigTable[thetaIndex].ang;
                        qline result = {
                                (rhoIndex - maxRho),
                                ang,
                                dir
                        };
                        dst.push_back(result);
                    }

            }
        }
    }

}

#endif
