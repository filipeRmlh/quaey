#ifndef quaey_hysteresisThresholding_h
#define quaey_hysteresisThresholding_h

#include <queue>
#include <unordered_set>
#include <opencv2/core.hpp>
#include "utils/accessUtils.h"

using namespace std;
using namespace cv;

/* Set of all visited pixels.
 * @note Useful to not repeat operations on the same pixel.
 */
typedef  unordered_set<size_t> MarkedList;

float getPixel (const Mat & m, int i, int j) {
    return isValidPosition(m.rows, m.cols, i, j) ? pixelValue(m.ptr<float>(), m.cols, m.channels(), i, j, 0) : 0;
}

void setPixel (Mat & m, int i, int j, float value) {
    if (isValidPosition(m.rows, m.cols, i, j)) {
        pixelValue(m.ptr<float>(), m.cols, m.channels(), i, j, 0) = value;
    }
}

/** Test if a given position is already visited, given i, j coords.
 * @param i: the i coord
 * @param j: the j coord
 * @param visited: the reference to the visited list
 * @return the test result (true or false)
 */
bool isVisited(const Position pos, MarkedList & marked, const Mat & m) {
    auto key = arrIndex(m.cols, m.channels(), pos.i, pos.j, 0);
    return marked.find(key) != marked.end();
}

/** Test if a given position is already visited, given i, j coords.
 * @param i: the i coord
 * @param j: the j coord
 * @param visited: the reference to the visited list
 * @return the test result (true or false)
 */
bool isVisited(const int index, MarkedList & marked) {
    return marked.find(index) != marked.end();
}

/** Mark a position as visited, given i, j coords.
 * @param i: the i coord
 * @param j: the j coord
 * @param visited: the reference to the visited list
 */
void setVisited(Position pos, MarkedList & visited, const Mat & m) {
    auto key = arrIndex(m.cols, m.channels(), pos.i, pos.j, 0);
    visited.insert(key);
}


/** Add node to the queue and mark it as visited.
 * @param position: the Position object
 * @param queue: the queue of accepted position nodes
 * @param visited: the reference to the visited list
 */
void visitNode(
        Position & position, deque<Position> & queue,
        MarkedList & visited,
        const Mat & m
) {
    queue.push_back(position);
    setVisited(position, visited, m);
}

/** Process all adjacent nodes from a given node position.
 * Once the node is processed, it will not be processed again.
 * All the nodes are classified to enter the queue
 * (as children in the tree) or not.
 * The bottom_threshold is used to do so.
 */
void visitNodes(
        const Position & currentPosition,
        const Mat & mag,
        const float bottom_threshold,
        MarkedList & marked,
        deque<Position> & queue
) {
    int current_i = currentPosition.i;
    int current_j = currentPosition.j;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            Position neighborPos = {current_i + i, current_j + j};
            float neighborValue = getPixel(mag, neighborPos.i, neighborPos.j);
            if (
                    isValidPosition(mag.rows, mag.cols, neighborPos.i, neighborPos.j)
                    && !isVisited(neighborPos, marked, mag)
                    ) {
                if (neighborValue >= bottom_threshold  )
                {
                    visitNode(neighborPos, queue, marked, mag);
                } else {

                    // Even if the node doesn't belong to the tree, it will be marked to avoid unnecessary processing in the future.
                    setVisited(neighborPos, marked, mag);
                }
            }
        }
    }
}

void bfs(MarkedList & marked, deque<Position> & queue, Mat & mag, Mat & ang, Mat & outMag, Mat & outAng, const float & bottomThreshold ) {
    while(!queue.empty()) {
        auto curr = queue.front();
        queue.pop_front();
        auto currMagValue = getPixel(mag, curr.i, curr.j);
        auto currAngValue = getPixel(ang, curr.i, curr.j);

        // Set the current pixel got from the queue to the output magnitude and angle matrices.
        setPixel(outMag, curr.i, curr.j, currMagValue);
        setPixel(outAng, curr.i, curr.j, currAngValue);

        visitNodes(
                curr, mag, bottomThreshold, marked, queue
        );
    }
}

/** Find the next tree root.
 * Aka: the next non-visited strong pixel position.
 * If none was found, return an invalid position.
 * The upper_threshold is used to do so.
 */
Position findNextRoot(
        Position current, Mat & mag,
        float upper_threshold, MarkedList & visited
)
{
    const int initialIndex =
            current.i == -1 && current.j == -1 ? -1 :
            arrIndex(mag.cols, mag.channels(), current.i, current.j, 0);

    auto data = mag.ptr<float>();

    for (int i = initialIndex + 1; i < mag.rows * mag.cols; i++) {
        auto index = i*mag.channels();
        auto candidateValue = data[index];
        if (candidateValue >= upper_threshold && !isVisited(index, visited)) {
            return getPosition(mag.cols, mag.channels(), index);
        }
    }

    return {-1, -1};
}


/**
 * Perform the Hysteresis Threshold operation
 *
 * @param mag: the input magnitude matrix
 * @param ang: the input angle matrix
 * @param out_mag: the output magnitude matrix
 * @param out_ang: the output angle matrix
 * @param upper_threshold: Hysteresis upper threshold
 * @param bottom_threshold: Hysteresis bottom threshold
 *
 * @note: All flat array matrices must have l*c size.
 * @note: The possible range for the thresholds must be the magnitude range values of the input magnitude matrix
 */
void hysteresisThresholding( Mat & mag, Mat & ang, Mat & out_mag, Mat & out_ang, const float upper_threshold, const float bottom_threshold)
{
    assert(mag.rows == ang.rows && mag.cols == ang.cols && mag.type() == ang.type());
    out_ang.create(mag.rows, mag.cols, mag.type());
    out_mag.create(mag.rows, mag.cols, mag.type());

    MarkedList markedList;
    deque<Position> queue;

    Position currentRoot = findNextRoot(
            Position {-1, -1}, mag, upper_threshold, markedList
    );

    // Perform the forest building
    while (isValidPosition(mag.rows, mag.cols, currentRoot.i, currentRoot.j)) {
        visitNode(currentRoot, queue, markedList, mag);
        bfs(markedList, queue, mag, ang, out_mag, out_ang, bottom_threshold);
        currentRoot = findNextRoot(
                currentRoot, mag, upper_threshold, markedList
        );
    }
}

#endif