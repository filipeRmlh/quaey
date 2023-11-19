#include <complex>
#include <utility>
#include <vector>
#include <bit>

#include "./constants.cpp"

using FftSample = std::complex<double>;
typedef std::vector<FftSample> FFtVector;

class FftList: public FFtVector {
protected:
    int _originalSize = -1;
public:
    FftList(size_t N, int value): vector(std::__bit_ceil(N), value) { }

    FftList(): vector() {}

    FftList(std::initializer_list<FftSample> samples): FFtVector(samples) {
        this->resize(std::__bit_ceil(this->size()), FftSample(0));
    }

    FftList(FftList const & samples): FFtVector(samples) {}

    FftList(FftList const && samples)  noexcept :
        vector(samples.begin(), samples.end())
        { }

    int getOriginalSize() const {
        return _originalSize == -1 ? (int) this->size() : _originalSize;
    }

    void setOriginalSize(int originalLength = -1) {
        this->_originalSize = originalLength == -1 ? (int) this->size() : originalLength;
    }
};


void evenOddSplit(const FftList &original, FftList& even, FftList & odd) {
    std::size_t const half_size = original.size() / 2;
    for(size_t i = 0; i < half_size; i++) {
        even.push_back(original[2 * i]);
        odd.push_back(original[2 * i + 1]);
    }
}

FftSample fftOdd(FftSample oddSample, int k, size_t N, bool isInverse) {
    return std::polar(1.0, (isInverse ? 1 : -1 ) * 2*Pi *k/(double) N) * oddSample;
}

FftList f_i_fft(FftList & samples, bool isInverse) {
    auto N = samples.size();
    if(N == 1) {
        return samples;
    }

    FftList even, odd;
    evenOddSplit(samples, even, odd);

    auto sampleEvenResult = f_i_fft(even, isInverse);
    auto sampleOddResult = f_i_fft(odd, isInverse);

    FftList result(N, 0);
    for(int k = 0; k < N/2; k++) {
        auto oddPart = fftOdd(sampleOddResult[k], k, N, isInverse);
        result[k] = sampleEvenResult[k] + oddPart;
        result[N/2 + k] = sampleEvenResult[k] - oddPart;
    }

    return result;
}

FftList operator/(const FftList& samples, size_t n) {
    FftList result;
    for (auto sample: samples)
        result.push_back(sample / (double) n);
    return result;
}

FFtVector ifft(const FftList& samples) {
    auto result = (f_i_fft((FftList &) samples, true) / samples.size());
    return {result.begin(), result.begin()+samples.getOriginalSize() };
}

FftList fft(FftList& samples) {
    auto result =  f_i_fft(samples, false);
    result.setOriginalSize((int) samples.size());
    return result;
}

