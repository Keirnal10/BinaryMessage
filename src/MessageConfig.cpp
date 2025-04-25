#include "MessageConfig.hpp"
#include <stdexcept>
#include <algorithm>

namespace BinaryMessageLibrary {

MessageConfig::MessageConfig() : total_bits_(0) {}

MessageConfig::MessageConfig(const nlohmann::json& config) {
    setConfig(config);
}

void MessageConfig::setConfig(const nlohmann::json& config) {
    if (!config.is_array()) {
        throw std::runtime_error("Configuration must be an array of fields");
    }

    fields_.clear();
    total_bits_ = 0;
    
    for (const auto& field : config) {
        if (!field.is_object()) {
            throw std::runtime_error("Each field must be a JSON object");
        }

        // Check for required fields
        if (!field.contains("name")) {
            throw std::runtime_error("Field configuration missing required 'name' field");
        }
        if (!field.contains("bit_width")) {
            throw std::runtime_error("Field configuration missing required 'bit_width' field");
        }

        try {
            std::string name = field["name"].get<std::string>();
            uint8_t bit_width = field["bit_width"].get<uint8_t>();
            bool is_signed = field.value("signed", false);

            if (bit_width == 0 || bit_width > 64) {
                throw std::runtime_error("Invalid bit width for field '" + name + "': " + 
                                       std::to_string(bit_width));
            }

            fields_.emplace_back(name, bit_width, is_signed);
            total_bits_ += bit_width;
        } catch (const nlohmann::json::exception& e) {
            throw std::runtime_error("Invalid field configuration: " + std::string(e.what()));
        }
    }
}

const std::vector<FieldConfig>& MessageConfig::getFields() const {
    return fields_;
}

size_t MessageConfig::getTotalBits() const {
    return total_bits_;
}

const FieldConfig& MessageConfig::getFieldConfig(const std::string& name) const {
    auto it = std::find_if(fields_.begin(), fields_.end(),
        [&name](const FieldConfig& field) { return field.name() == name; });
    
    if (it == fields_.end()) {
        throw std::runtime_error("Field not found: " + name);
    }
    
    return *it;
}

bool MessageConfig::hasField(const std::string& name) const {
    return std::any_of(fields_.begin(), fields_.end(),
        [&name](const FieldConfig& field) { return field.name() == name; });
}

} // namespace BinaryMessageLibrary 