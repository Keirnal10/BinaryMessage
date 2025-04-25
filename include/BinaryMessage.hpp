#pragma once

#include "MessageConfig.hpp"
#include <vector>
#include <string>

namespace BinaryMessage {

/**
 * @brief Class for handling binary messages with configurable fields.
 * 
 * This class provides functionality to create, manipulate, pack, and unpack binary messages
 * according to a specified configuration. It supports fields of varying bit widths and
 * both signed and unsigned values.
 */
class BinaryMessage {
public:
    /**
     * @brief Constructs a new BinaryMessage object.
     * 
     * @param config The message configuration to use.
     * 
     * @throws std::runtime_error if the configuration is invalid.
     */
    explicit BinaryMessage(const MessageConfig& config);
    
    /**
     * @brief Sets the value of a field in the message.
     * 
     * @param name The name of the field to set.
     * @param value The value to set.
     * 
     * @throws std::runtime_error if the field name is invalid or if the value is
     *         outside the valid range for the field.
     */
    void setField(const std::string& name, int64_t value);

    /**
     * @brief Gets the value of a field in the message.
     * 
     * @param name The name of the field to get.
     * @return int64_t The value of the field.
     * 
     * @throws std::runtime_error if the field name is invalid.
     */
    int64_t getField(const std::string& name) const;
    
    /**
     * @brief Packs the message into a binary buffer.
     * 
     * The buffer will be sized to hold the minimum number of bytes required
     * for the message (rounded up from bits).
     * 
     * @return std::vector<uint8_t> The packed binary buffer.
     */
    std::vector<uint8_t> pack() const;

    /**
     * @brief Unpacks a binary buffer into the message.
     * 
     * @param buffer The binary buffer to unpack.
     * 
     * @throws std::runtime_error if the buffer is too small to hold the message
     *         or if the buffer contains invalid data.
     */
    void unpack(const std::vector<uint8_t>& buffer);

    /**
     * @brief Gets the message configuration.
     * 
     * @return const MessageConfig& The message configuration.
     */
    const MessageConfig& getConfig() const;

private:
    const MessageConfig& config_;
    std::vector<int64_t> field_values_;
    
    /**
     * @brief Validates that a field name exists in the configuration.
     * 
     * @param name The field name to validate.
     * 
     * @throws std::runtime_error if the field name is invalid.
     */
    void validateFieldName(const std::string& name) const;

    /**
     * @brief Validates that a value is within the valid range for a field.
     * 
     * @param name The name of the field to validate against.
     * @param value The value to validate.
     * 
     * @throws std::runtime_error if the value is outside the valid range.
     */
    void validateFieldValue(const std::string& name, int64_t value) const;
};

} // namespace BinaryMessage 