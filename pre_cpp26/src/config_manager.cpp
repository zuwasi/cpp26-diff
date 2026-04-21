#include "config_manager.h"
#include <cstdarg>
#include <cstdio>

void ConfigManager::setConfig(const std::string& key, const std::string& value) {
    // No validation — empty key, empty value, duplicates all silently accepted
    config_[key] = value;
}

// CERT-CTR50: operator[] inserts default if key missing — modifies map
std::string ConfigManager::getConfig(const std::string& key) {
    return config_[key];  // Inserts empty string if key not found
}

void ConfigManager::removeConfig(const std::string& key) {
    config_.erase(key);  // Silent no-op if key missing — no feedback
}

bool ConfigManager::hasConfig(const std::string& key) const {
    return config_.find(key) != config_.end();
}

// CERT-DCL50 / MISRA-21.6.1: C-style variadic with printf
void ConfigManager::loadDefaults(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; ++i) {
        const char* key = va_arg(args, const char*);    // Type-unsafe
        const char* val = va_arg(args, const char*);    // Type-unsafe
        config_[key] = val;
        printf("Loaded default: %s = %s\n", key, val);  // MISRA-21.6.1
    }

    va_end(args);
}
