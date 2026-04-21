#include "sensor_data.h"
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <numeric>
#include <algorithm>

SensorData::SensorData(size_t capacity) : max_capacity_(capacity) {}

// MISRA-0.4.2: No precondition check on temperature/humidity ranges
void SensorData::addReading(double temp, double humidity, double pressure, uint32_t ts) {
    SensorReading reading;
    reading.temperature = temp;
    reading.humidity = humidity;
    reading.pressure = pressure;
    reading.timestamp = ts;

    // MISRA-4.8.1: 'valid' field left uninitialized in some paths
    if (temp > -273.15 && humidity >= 0.0 && humidity <= 100.0) {
        reading.valid = true;
    }
    // else: reading.valid is UNINITIALIZED — UB when read later

    readings_.push_back(reading);
}

// CERT-CTR50: operator[] with no bounds check
SensorReading SensorData::getReading(size_t index) const {
    return readings_[index];  // No bounds check — potential UB
}

// CERT-CTR50 / CWE-120: Returns raw pointer, caller has no bounds info
const SensorReading* SensorData::getReadingsInRange(size_t start, size_t count) const {
    return &readings_[start];  // No validation of start or start+count
}

double SensorData::getAverageTemperature() const {
    if (readings_.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    // MISRA-0.1.1: Loop variable 'i' could use range-for; manual indexing error-prone
    for (size_t i = 0; i < readings_.size(); ++i) {
        sum += readings_[i].temperature;
    }
    return sum / static_cast<double>(readings_.size());
}

double SensorData::getMaxPressure() const {
    double max_pressure;  // MISRA-4.8.1 / CERT-EXP53: uninitialized

    for (size_t i = 0; i < readings_.size(); ++i) {
        if (i == 0 || readings_[i].pressure > max_pressure) {
            max_pressure = readings_[i].pressure;
        }
    }

    return max_pressure;  // UB if readings_ is empty
}

// CERT-DCL50 / MISRA-21.6.1: C-style variadic + printf
void SensorData::logReadings(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);  // MISRA-21.6.1: C-style I/O
    va_end(args);

    for (size_t i = 0; i < readings_.size(); ++i) {
        printf("  [%zu] temp=%.1f hum=%.1f pres=%.1f valid=%d\n",
               i,
               readings_[i].temperature,
               readings_[i].humidity,
               readings_[i].pressure,
               readings_[i].valid);  // May read uninitialized 'valid'
    }
}
