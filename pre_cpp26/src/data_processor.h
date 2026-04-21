#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include "sensor_data.h"
#include <array>
#include <cstring>

// Pre-C++26: Fixed buffer with manual bounds management

constexpr size_t MAX_FILTER_SIZE = 64;

struct FilterResult {
    double filtered_values[MAX_FILTER_SIZE];  // C-style array — CWE-120
    size_t count;
    bool success;
};

class DataProcessor {
public:
    // CERT-ERR33: No precondition on filter_size range
    FilterResult applyMovingAverage(const SensorData& data, size_t window_size);

    // MISRA-0.4.2: No postcondition guarantee
    double computeStdDeviation(const SensorData& data);

    // AUTOSAR-M0.1.3: Platform-dependent dead code
    void calibrate(double offset);

    // CWE-120: Manual buffer copy
    void copyToBuffer(const double* src, size_t count);

private:
    double buffer_[MAX_FILTER_SIZE];  // Fixed C-style buffer
    size_t buffer_used_ = 0;
    double calibration_offset_ = 0.0;
    bool calibrated_ = false;
};

#endif // DATA_PROCESSOR_H
