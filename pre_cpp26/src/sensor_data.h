#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <vector>
#include <string>
#include <cstdint>

// Pre-C++26: No contracts, no hardened containers, raw access patterns

struct SensorReading {
    double temperature;
    double humidity;
    double pressure;
    uint32_t timestamp;
    bool valid;
};

class SensorData {
public:
    SensorData() = default;
    explicit SensorData(size_t capacity);

    // CERT-ERR33 / MISRA-0.4.2: No precondition enforcement
    void addReading(double temp, double humidity, double pressure, uint32_t ts);

    // CERT-CTR50: Unchecked index access
    SensorReading getReading(size_t index) const;

    // CERT-CTR50: Raw pointer return, no bounds
    const SensorReading* getReadingsInRange(size_t start, size_t count) const;

    double getAverageTemperature() const;
    double getMaxPressure() const;

    size_t size() const { return readings_.size(); }
    bool empty() const { return readings_.empty(); }

    // CERT-DCL50: C-style variadic function
    void logReadings(const char* format, ...);

private:
    std::vector<SensorReading> readings_;
    size_t max_capacity_ = 1000;
};

#endif // SENSOR_DATA_H
