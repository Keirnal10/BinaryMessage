#ifndef BINARY_MESSAGE_MESSAGE_CONFIG_HPP
#define BINARY_MESSAGE_MESSAGE_CONFIG_HPP

#include "FieldConfig.hpp"
#include <vector>
#include <nlohmann/json.hpp>

namespace BinaryMessage {

class MessageConfig {
public:
    MessageConfig() = default;
    explicit MessageConfig(const nlohmann::json& config);
    
    void setConfig(const nlohmann::json& config);
    const std::vector<FieldConfig>& getFields() const;
    size_t getTotalBits() const;

private:
    std::vector<FieldConfig> fields_;
    size_t total_bits_ = 0;
};

} // namespace BinaryMessage

#endif // BINARY_MESSAGE_MESSAGE_CONFIG_HPP 