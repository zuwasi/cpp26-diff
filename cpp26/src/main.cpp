#include "sensor_data.h"
#include "data_processor.h"
#include "config_manager.h"
#include <iostream>
#include <format>
#include <array>

int main() {
    // === Sensor Data Demo ===
    SensorData sensors(100);

    sensors.addReading(22.5, 45.0, 1013.25, 1000);
    sensors.addReading(23.1, 50.0, 1012.80, 1001);
    // sensors.addReading(-300.0, 110.0, 900.0, 1002);  // Contract violation! Caught at runtime
    sensors.addReading(21.8, 55.0, 1014.10, 1003);

    double avg = sensors.getAverageTemperature();
    double max_p = sensors.getMaxPressure();

    // C++26: std::format replaces printf — MISRA-21.6.1 resolved
    std::cout << std::format("Average temperature: {:.2f}", avg) << '\n';
    std::cout << std::format("Max pressure: {:.2f}", max_p) << '\n';

    // C++26: Type-safe variadic template log
    sensors.logReadings("=== Sensor Log ===");

    // C++26: No unused variables — placeholder _ for intentional discard
    auto _ = 0;  // If needed for RAII guards etc.

    // === Data Processor Demo ===
    DataProcessor processor;
    FilterResult filtered = processor.applyMovingAverage(sensors, 2);

    if (filtered.success) {
        std::cout << std::format("Filtered {} values", filtered.count) << '\n';
        for (size_t i = 0; i < filtered.count; ++i) {
            std::cout << std::format("  [{}] = {:.2f}", i, filtered.filtered_values[i]) << '\n';
        }
    }

    double stddev = processor.computeStdDeviation(sensors);
    std::cout << std::format("Std deviation: {:.4f}", stddev) << '\n';

    // C++26: span-based copy — bounds-safe
    std::array<double, 5> raw_data = {1.0, 2.0, 3.0, 4.0, 5.0};
    processor.copyToBuffer(raw_data);

    processor.calibrate(0.5);

    // === Config Manager Demo ===
    ConfigManager config;

    // C++26: Type-safe span of ConfigEntry — no C variadic
    std::array defaults = {
        ConfigEntry{"sample_rate", "100"},
        ConfigEntry{"precision", "high"}
    };
    config.loadDefaults(defaults);

    config.setConfig("mode", "continuous");

    // C++26: optional return — no accidental map insertion
    auto val = config.getConfig("nonexistent_key");
    if (val.has_value()) {
        std::cout << std::format("Found: {}", *val) << '\n';
    } else {
        std::cout << "Key not found (no side effects)" << '\n';
    }

    std::cout << std::format("Mode: {}", config.getConfig("mode").value_or("unknown")) << '\n';

    return 0;
}
