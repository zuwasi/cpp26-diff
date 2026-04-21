#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <vector>
#include <span>
#include <string>
#include <cstdint>
#include <format>
#include <iostream>

// C++26: Contracts, hardened containers, no variadics, modern I/O

struct SensorReading {
    double temperature = 0.0;    // C++26: All members value-initialized (erroneous behavior model)
    double humidity    = 0.0;
    double pressure    = 0.0;
    uint32_t timestamp = 0;
    bool valid         = false;  // No more uninitialized — MISRA-4.8.1 resolved
};

class SensorData {
public:
    SensorData() = default;
    explicit SensorData(size_t capacity);

    // C++26 contract: pre(temp > -273.15)
    // C++26 contract: pre(humidity >= 0.0 && humidity <= 100.0)
    // C++26 contract: pre(pressure > 0.0)
    void addReading(double temp, double humidity, double pressure, uint32_t ts);

    // C++26 contract: pre(index < size())
    SensorReading getReading(size_t index) const;

    // C++26 contract: pre(start < size())
    // C++26 contract: pre(start + count <= size())
    // C++26: Return span instead of raw pointer — bounds-safe view
    std::span<const SensorReading> getReadingsInRange(size_t start, size_t count) const;

    // C++26 contract: pre(!empty())
    double getAverageTemperature() const;

    // C++26 contract: pre(!empty())
    // C++26 contract: post(r: r > 0.0)
    double getMaxPressure() const;

    size_t size() const { return readings_.size(); }
    bool empty() const { return readings_.empty(); }

    // C++26: Type-safe variadic template replaces C-style va_list
    template<typename... Args>
    void logReadings(std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
        for (size_t i = 0; i < readings_.size(); ++i) {
            const auto& r = readings_[i];
            std::cout << std::format("  [{}] temp={:.1f} hum={:.1f} pres={:.1f} valid={}",
                         i, r.temperature, r.humidity, r.pressure, r.valid) << '\n';
        }
    }

private:
    std::vector<SensorReading> readings_;
    size_t max_capacity_ = 1000;
};

#endif // SENSOR_DATA_H
