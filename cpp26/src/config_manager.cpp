#include "config_manager.h"
#include <iostream>
#include <format>
#include <cassert>

// C++26 contract: pre(!key.empty())
void ConfigManager::setConfig(const std::string& key, const std::string& value) {
    assert(!key.empty());  // C++26 contract: pre(!key.empty())
    config_[key] = value;
}

// C++26: const-correct access via find() — no silent map insertion
std::optional<std::string> ConfigManager::getConfig(const std::string& key) const {
    auto it = config_.find(key);
    if (it != config_.end()) {
        return it->second;
    }
    return std::nullopt;  // Explicit "not found" — CERT-CTR50 resolved
}

// C++26: Returns bool to indicate if key existed — MISRA-0.4.2 resolved
bool ConfigManager::removeConfig(const std::string& key) {
    return config_.erase(key) > 0;
}

bool ConfigManager::hasConfig(const std::string& key) const {
    return config_.contains(key);  // C++20 .contains()
}

// C++26: Type-safe span — no va_list, no printf
void ConfigManager::loadDefaults(std::span<const ConfigEntry> defaults) {
    for (const auto& entry : defaults) {
        config_[entry.key] = entry.value;
        std::cout << std::format("Loaded default: {} = {}", entry.key, entry.value) << '\n';
    }
}
