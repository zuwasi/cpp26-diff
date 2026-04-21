#include "sensor_data.h"
#include "data_processor.h"
#include "config_manager.h"
#include <cstdio>

int main() {
    // === Sensor Data Demo ===
    SensorData sensors(100);

    sensors.addReading(22.5, 45.0, 1013.25, 1000);
    sensors.addReading(23.1, 50.0, 1012.80, 1001);
    sensors.addReading(-300.0, 110.0, 900.0, 1002);  // Invalid — 'valid' left uninitialized
    sensors.addReading(21.8, 55.0, 1014.10, 1003);

    // CERT-CTR50: No bounds check on index 99 (only 4 readings)
    // SensorReading bad = sensors.getReading(99);

    double avg = sensors.getAverageTemperature();
    double max_p = sensors.getMaxPressure();

    // MISRA-21.6.1: C-style printf
    printf("Average temperature: %.2f\n", avg);
    printf("Max pressure: %.2f\n", max_p);

    // CERT-DCL50: Variadic call — type safety not enforced
    sensors.logReadings("=== Sensor Log ===\n");

    // MISRA-0.1.1: Unused variables
    int error_code = 0;
    double threshold = 25.0;

    // === Data Processor Demo ===
    DataProcessor processor;
    FilterResult filtered = processor.applyMovingAverage(sensors, 2);

    if (filtered.success) {
        printf("Filtered %zu values\n", filtered.count);
        for (size_t i = 0; i < filtered.count; ++i) {
            printf("  [%zu] = %.2f\n", i, filtered.filtered_values[i]);
        }
    }

    double stddev = processor.computeStdDeviation(sensors);
    printf("Std deviation: %.4f\n", stddev);

    // CWE-120: Copy without bounds check
    double raw_data[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    processor.copyToBuffer(raw_data, 5);

    processor.calibrate(0.5);

    // === Config Manager Demo ===
    ConfigManager config;

    // CERT-DCL50: C-style variadic — type-unsafe
    config.loadDefaults(2, "sample_rate", "100", "precision", "high");

    config.setConfig("mode", "continuous");

    // CERT-CTR50: getConfig uses operator[] — can insert unintended entries
    std::string val = config.getConfig("nonexistent_key");

    printf("Mode: %s\n", config.getConfig("mode").c_str());

    return 0;
}
