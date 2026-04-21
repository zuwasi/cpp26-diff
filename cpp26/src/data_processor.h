#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include "sensor_data.h"
#include <span>
#include <array>
#include <vector>
#include <optional>

// C++26: inplace_vector for fixed-capacity, bounds-safe buffer
// (Using std::array + size tracking as stand-in until <inplace_vector> is available)

constexpr size_t MAX_FILTER_SIZE = 64;

struct FilterResult {
    std::array<double, MAX_FILTER_SIZE> filtered_values{};  // Value-initialized
    size_t count = 0;
    bool success = false;
};

class DataProcessor {
public:
    // C++26 contract: pre(window_size > 0)
    // C++26 contract: pre(window_size <= data.size())
    // C++26 contract: post(r: r.success == true)
    FilterResult applyMovingAverage(const SensorData& data, size_t window_size);

    // C++26 contract: pre(!data.empty())
    // C++26 contract: post(r: r >= 0.0)
    double computeStdDeviation(const SensorData& data);

    void calibrate(double offset);

    // C++26 contract: pre(src.size() <= MAX_FILTER_SIZE)
    // C++26: span replaces raw pointer — bounds-safe
    void copyToBuffer(std::span<const double> src);

private:
    std::array<double, MAX_FILTER_SIZE> buffer_{};  // Value-initialized, no raw C array
    size_t buffer_used_ = 0;
    double calibration_offset_ = 0.0;
    bool calibrated_ = false;
};

// C++26: Deleted with reason — clear diagnostic instead of linker error
void unsafe_legacy_filter(double* data, int count) = delete("Use DataProcessor::applyMovingAverage instead");

#endif // DATA_PROCESSOR_H
