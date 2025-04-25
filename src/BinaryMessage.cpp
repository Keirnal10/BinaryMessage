#include "BinaryMessage.hpp"
#include "MessageConfig.hpp"
#include <stdexcept>
#include <bitset>
#include <algorithm>

namespace BinaryMessageLibrary {

BinaryMessage::BinaryMessage(const MessageConfig& config)
    : config_(config), field_values_(config.getFields().size(), 0) {}

void BinaryMessage::setField(const std::string& name, int64_t value) {
    validateFieldValue(name, value);
    size_t index = getFieldOffset(name);
    field_values_[index] = value;
}

int64_t BinaryMessage::getField(const std::string& name) const {
    size_t index = getFieldOffset(name);
    return field_values_[index];
}

std::vector<uint8_t> BinaryMessage::pack() const {
    size_t total_bits = config_.getTotalBits();
    size_t total_bytes = (total_bits + 7) / 8;
    std::vector<uint8_t> buffer(total_bytes, 0);

    size_t current_bit = 0;
    for (size_t i = 0; i < config_.getFields().size(); ++i) {
        const auto& field = config_.getFields()[i];
        int64_t value = field_values_[i];
        
        // Handle signed values
        if (field.is_signed()) {
            uint64_t mask = (1ULL << field.bit_width()) - 1;
            value &= mask;
        }

        // Pack bits into buffer
        for (size_t bit = 0; bit < field.bit_width(); ++bit) {
            size_t byte_index = (current_bit + bit) / 8;
            size_t bit_index = (current_bit + bit) % 8;
            
            if (value & (1ULL << bit)) {
                buffer[byte_index] |= (1 << bit_index);
            }
        }
        
        current_bit += field.bit_width();
    }

    return buffer;
}

void BinaryMessage::unpack(const std::vector<uint8_t>& buffer) {
    size_t total_bits = config_.getTotalBits();
    size_t required_bytes = (total_bits + 7) / 8;
    
    if (buffer.size() < required_bytes) {
        throw std::runtime_error("Buffer too small for message");
    }

    size_t current_bit = 0;
    for (size_t i = 0; i < config_.getFields().size(); ++i) {
        const auto& field = config_.getFields()[i];
        int64_t value = 0;

        // Unpack bits from buffer
        for (size_t bit = 0; bit < field.bit_width(); ++bit) {
            size_t byte_index = (current_bit + bit) / 8;
            size_t bit_index = (current_bit + bit) % 8;
            
            if (buffer[byte_index] & (1 << bit_index)) {
                value |= (1ULL << bit);
            }
        }

        // Handle signed values
        if (field.is_signed() && (value & (1ULL << (field.bit_width() - 1)))) {
            value |= ~((1ULL << field.bit_width()) - 1);
        }

        field_values_[i] = value;
        current_bit += field.bit_width();
    }
}

const MessageConfig& BinaryMessage::getConfig() const {
    return config_;
}

size_t BinaryMessage::getFieldOffset(const std::string& name) const {
    const auto& fields = config_.getFields();
    auto it = std::find_if(fields.begin(), fields.end(),
        [&name](const FieldConfig& field) { return field.name() == name; });
    
    if (it == fields.end()) {
        throw std::runtime_error("Field not found: " + name);
    }
    
    return std::distance(fields.begin(), it);
}

void BinaryMessage::validateFieldName(const std::string& name) const {
    if (!config_.hasField(name)) {
        throw std::runtime_error("Invalid field name: " + name);
    }
}

void BinaryMessage::validateFieldValue(const std::string& name, int64_t value) const {
    validateFieldName(name);
    const auto& field = config_.getFieldConfig(name);
    if (!field.isValidValue(value)) {
        throw std::runtime_error("Value " + std::to_string(value) + 
                               " out of range for field " + name);
    }
}

} // namespace BinaryMessageLibrary 