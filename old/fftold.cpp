#include "./fft.cpp"
#include <iostream>
#include <vector>
#include <valarray>
#include <thread>
#include <mutex>


using namespace std;


std::mutex printLinesMutex;

template<typename T>
ostream& operator<<( ostream& stream, vector<T> const& vector) {

    printLinesMutex.lock();
    stream << "[";
    for(int i=0; i < vector.size() - 1; i++) {
        stream << vector[i] << ", ";
    }
    stream << vector[vector.size() - 1] << "]";
    printLinesMutex.unlock();
    return stream;
}

unsigned int numThreads() {
    return std::thread::hardware_concurrency();
}

void processSpace(FftList & sampleList, FftList & sampleResult) {
    auto output = fft(sampleList);
    auto output2 = ifft(output);
    sampleResult.assign(output2.begin(), output2.end());
}

int nextPot2(int num) {
    return (int) pow(2, ceil(log2(num)));
}

int main() {
    auto a = FftList({2.4, 3.6, 4.333});
    auto result = fft(a);
    auto result2 = ifft(result);
    const auto& b = result2;
//    cout << result << endl;
    cout << b << endl;
    cout << a << endl;

//    int limit = pow(2, 4);
//
//    SampleList v(limit, 0);
//    SampleList result;
//
//    for (int i = 0; i < limit; i++)
//    {
//        v[i] = i+1;
//    }
//
//    auto numT = 5;//numThreads();
//    thread threads[numT];
//
//    int divisionElements = floor((float) v.size() / numT);
//    int space = nextPot2(divisionElements);
//
//    SampleList resultPieces[numT];
//    SampleList inputListPieces[numT];
//
//    for(unsigned int i = 0; i < numT; i++ ) {
//        inputListPieces[i] = SampleList ((i * divisionElements) + v.begin(), v.begin() + ((i + 1) * divisionElements));
//        inputListPieces[i].resize(space, 0);
//        resultPieces[i] = SampleList(space, 0);
//        threads[i] = std::thread(processSpace, std::ref(inputListPieces[i]), std::ref(resultPieces[i]));
//    }
//
//    for(thread & el: threads) {
//        el.join();
//    }
//
//    for(unsigned int i = 0; i < numT; i++ ) {
//        SampleList resultPiece = resultPieces[i];
//        cout << "resultPieces "<< resultPieces[i] << endl;
//    }


//    for (int x = 0; x < limit; x++)
//    {
//        int val = (int) round(output2[x].real());
//        int expectedValue = v[x].real();
//        if (expectedValue != val) {
//            cout << x << " - VAL: " <<val << " - "<< expectedValue << endl;
//        }
//    }
//
//    cout << output2.size() << endl;
}