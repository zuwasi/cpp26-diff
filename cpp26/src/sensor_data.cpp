#include "sensor_data.h"
#include <numeric>
#include <algorithm>
#include <ranges>
#include <cassert>

SensorData::SensorData(size_t capacity) : max_capacity_(capacity) {}

// C++26 contract: pre(temp > -273.15), pre(humidity >= 0.0 && humidity <= 100.0), pre(pressure > 0.0)
void SensorData::addReading(double temp, double humidity, double pressure, uint32_t ts) {
    assert(temp > -273.15);                        // C++26 contract: pre(temp > -273.15)
    assert(humidity >= 0.0 && humidity <= 100.0);   // C++26 contract: pre(humidity >= 0.0 && humidity <= 100.0)
    assert(pressure > 0.0);                         // C++26 contract: pre(pressure > 0.0)

    SensorReading reading{
        .temperature = temp,
        .humidity    = humidity,
        .pressure    = pressure,
        .timestamp   = ts,
        .valid       = true  // Always initialized — no more MISRA-4.8.1
    };
    readings_.push_back(reading);
}

// C++26 contract: pre(index < size())
// C++26: Precondition guarantees index is valid — hardened vector also bounds-checks
SensorReading SensorData::getReading(size_t index) const {
    assert(index < size());  // C++26 contract: pre(index < size())
    return readings_.at(index);  // Hardened: .at() with contract precondition
}

// C++26 contract: pre(start < size()), pre(start + count <= size())
// C++26: Returns span — caller gets bounds-safe view, no raw pointer
std::span<const SensorReading> SensorData::getReadingsInRange(size_t start, size_t count) const {
    assert(start < size());            // C++26 contract: pre(start < size())
    assert(start + count <= size());   // C++26 contract: pre(start + count <= size())
    return std::span(readings_).subspan(start, count);
}

// C++26 contract: pre(!empty())
double SensorData::getAverageTemperature() const {
    assert(!empty());  // C++26 contract: pre(!empty())
    // C++26: ranges-based — no manual indexing, no off-by-one risk
    auto temps = readings_ | std::views::transform(&SensorReading::temperature);
    double sum = std::accumulate(std::ranges::begin(temps), std::ranges::end(temps), 0.0);
    return sum / static_cast<double>(readings_.size());
}

// C++26 contract: pre(!empty()), post(r: r > 0.0)
double SensorData::getMaxPressure() const {
    assert(!empty());  // C++26 contract: pre(!empty())
    // C++26: No uninitialized variable — direct algorithm usage
    auto it = std::ranges::max_element(readings_, {}, &SensorReading::pressure);
    double r = it->pressure;  // Precondition guarantees non-empty
    assert(r > 0.0);  // C++26 contract: post(r: r > 0.0)
    return r;
}
