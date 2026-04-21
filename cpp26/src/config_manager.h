#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <map>
#include <optional>
#include <span>
#include <utility>

struct ConfigEntry {
    std::string key;
    std::string value;
};

class ConfigManager {
public:
    // C++26 contract: pre(!key.empty())
    void setConfig(const std::string& key, const std::string& value);

    // C++26: Returns optional — no silent insertion of empty values
    std::optional<std::string> getConfig(const std::string& key) const;

    // C++26 Contract + return feedback
    bool removeConfig(const std::string& key);

    bool hasConfig(const std::string& key) const;

    // C++26: Type-safe span replaces C-style variadic
    void loadDefaults(std::span<const ConfigEntry> defaults);

    // C++26: Deleted variadic with reason
    void loadDefaults(int count, ...) = delete("Use span<const ConfigEntry> overload for type safety");

private:
    std::map<std::string, std::string> config_;
};

#endif // CONFIG_MANAGER_H
