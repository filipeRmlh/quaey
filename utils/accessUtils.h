#ifndef quaey_access_utils
#define quaey_access_utils

#define arrIndex(cols, channels, i, j, ch) channels*(cols*i + j) + ch

#define isValidPosition(rows, cols, i, j) i >= 0 && i < rows && j >= 0 && j < cols

#define pixelValue(elm, cols, channels, i, j, ch) elm[arrIndex(cols, channels, i, j, ch)]

template<typename T> T limitCell(const T* m, const int rows, const int cols, const int channels, const int i, const int j, const int channel) {
    return (isValidPosition(rows, cols, i, j)) ? pixelValue(m, cols, channels, i, j, channel) : 0;
}

typedef struct qPosition {
    int i;
    int j;
} Position;

Position getPosition(int cols, int channels, int index) {
    return Position{index / (channels*cols), index % (channels*cols)};
}

#endif