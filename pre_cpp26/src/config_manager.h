#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <map>

class ConfigManager {
public:
    // CERT-ERR33: No validation on key/value
    void setConfig(const std::string& key, const std::string& value);

    // CERT-CTR50: operator[] on map can insert — no const access
    std::string getConfig(const std::string& key);

    // MISRA-0.4.2: No indication if key existed
    void removeConfig(const std::string& key);

    bool hasConfig(const std::string& key) const;

    // CERT-DCL50: Should not be variadic
    void loadDefaults(int count, ...);

private:
    std::map<std::string, std::string> config_;
};

#endif // CONFIG_MANAGER_H
