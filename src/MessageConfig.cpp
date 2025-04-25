#include "MessageConfig.hpp"
#include <stdexcept>

namespace BinaryMessage {

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

        FieldConfig field_config;
        
        // Check for required fields
        if (!field.contains("name")) {
            throw std::runtime_error("Field configuration missing required 'name' field");
        }
        if (!field.contains("bit_width")) {
            throw std::runtime_error("Field configuration missing required 'bit_width' field");
        }

        try {
            field_config.name = field["name"].get<std::string>();
            field_config.bit_width = field["bit_width"].get<size_t>();
            field_config.is_signed = field.value("signed", false);
        } catch (const nlohmann::json::exception& e) {
            throw std::runtime_error("Invalid field configuration: " + std::string(e.what()));
        }
        
        if (field_config.bit_width == 0 || field_config.bit_width > 64) {
            throw std::runtime_error("Invalid bit width for field '" + field_config.name + "': " + 
                                   std::to_string(field_config.bit_width));
        }
        
        fields_.push_back(field_config);
        total_bits_ += field_config.bit_width;
    }
}

const std::vector<FieldConfig>& MessageConfig::getFields() const {
    return fields_;
}

size_t MessageConfig::getTotalBits() const {
    return total_bits_;
}

} // namespace BinaryMessage 