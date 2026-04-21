#include "data_processor.h"
#include <cmath>
#include <cassert>
#include <cstdio>

FilterResult DataProcessor::applyMovingAverage(const SensorData& data, size_t window_size) {
    FilterResult result;
    result.count = 0;
    result.success = false;

    // CERT-ERR33: assert is stripped in release builds — no real protection
    assert(window_size > 0 && "Window size must be positive");
    assert(window_size <= data.size() && "Window size exceeds data");

    if (data.size() < window_size) {
        return result;
    }

    size_t output_count = data.size() - window_size + 1;

    // CWE-120: No check if output_count exceeds MAX_FILTER_SIZE
    for (size_t i = 0; i < output_count; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < window_size; ++j) {
            // CERT-CTR50: getReading has no bounds check
            sum += data.getReading(i + j).temperature;
        }
        result.filtered_values[i] = sum / static_cast<double>(window_size);
    }

    result.count = output_count;
    result.success = true;
    return result;
}

double DataProcessor::computeStdDeviation(const SensorData& data) {
    double mean;      // MISRA-4.8.1: uninitialized
    double variance;  // MISRA-4.8.1: uninitialized
    double temp_val;  // MISRA-4.8.1: uninitialized

    if (data.size() == 0) {
        return 0.0;  // MISRA-0.4.2: No postcondition — caller can't tell error from real 0
    }

    mean = data.getAverageTemperature();
    variance = 0.0;

    for (size_t i = 0; i < data.size(); ++i) {
        temp_val = data.getReading(i).temperature - mean;
        variance += temp_val * temp_val;
    }

    variance /= static_cast<double>(data.size());
    return std::sqrt(variance);
}

void DataProcessor::calibrate(double offset) {
    calibration_offset_ = offset;

    // AUTOSAR-M0.1.3: Dead code — platform check that is always true/false at compile time
#ifdef _WIN32
    calibrated_ = true;
    printf("Calibrated on Windows with offset %.2f\n", offset);  // MISRA-21.6.1
#else
    calibrated_ = true;
    printf("Calibrated on Linux with offset %.2f\n", offset);    // MISRA-21.6.1
#endif

    // MISRA-0.1.1: Unused variable
    int status_code = 0;
    double unused_result = offset * 2.0;
}

// CWE-120: Manual memcpy with no bounds validation
void DataProcessor::copyToBuffer(const double* src, size_t count) {
    // No check: count could exceed MAX_FILTER_SIZE
    std::memcpy(buffer_, src, count * sizeof(double));
    buffer_used_ = count;
}
