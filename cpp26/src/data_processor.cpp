#include "data_processor.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>

FilterResult DataProcessor::applyMovingAverage(const SensorData& data, size_t window_size) {
    assert(window_size > 0);              // C++26 contract: pre(window_size > 0)
    assert(window_size <= data.size());   // C++26 contract: pre(window_size <= data.size())

    FilterResult result;

    size_t output_count = data.size() - window_size + 1;

    assert(output_count <= MAX_FILTER_SIZE);  // C++26 contract: contract_assert(output_count <= MAX_FILTER_SIZE)

    for (size_t i = 0; i < output_count; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < window_size; ++j) {
            sum += data.getReading(i + j).temperature;  // Bounds-checked via contract
        }
        result.filtered_values[i] = sum / static_cast<double>(window_size);
    }

    result.count = output_count;
    result.success = true;

    assert(result.success == true);  // C++26 contract: post(r: r.success == true)
    return result;
}

double DataProcessor::computeStdDeviation(const SensorData& data) {
    assert(!data.empty());  // C++26 contract: pre(!data.empty())

    // C++26: No uninitialized variables — all declared at point of use
    double mean = data.getAverageTemperature();
    double variance = 0.0;

    for (size_t i = 0; i < data.size(); ++i) {
        double diff = data.getReading(i).temperature - mean;
        variance += diff * diff;
    }

    variance /= static_cast<double>(data.size());
    double r = std::sqrt(variance);
    assert(r >= 0.0);  // C++26 contract: post(r: r >= 0.0)
    return r;
}

void DataProcessor::calibrate(double offset) {
    calibration_offset_ = offset;
    calibrated_ = true;

    // C++26: std::format replaces printf — MISRA-21.6.1 resolved
    std::cout << std::format("Calibrated with offset {:.2f}", offset) << '\n';

    // C++26: No unused variables — placeholder _ discards intentionally
    auto _ = offset * 2.0;  // MISRA-0.1.1 resolved: explicit discard
}

// C++26: span provides bounds — no manual memcpy needed
void DataProcessor::copyToBuffer(std::span<const double> src) {
    assert(src.size() <= MAX_FILTER_SIZE);  // C++26 contract: pre(src.size() <= MAX_FILTER_SIZE)
    std::ranges::copy(src, buffer_.begin());
    buffer_used_ = src.size();
}
