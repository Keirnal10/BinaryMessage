#pragma once

#include <string>
#include <cstdint>

namespace BinaryMessage {

/**
 * @brief Configuration class for a single field in a binary message.
 * 
 * This class defines the properties of a field in a binary message, including its name,
 * bit width, and whether it is signed. It provides methods to validate field values
 * against these constraints.
 */
class FieldConfig {
public:
    /**
     * @brief Constructs a new FieldConfig object.
     * 
     * @param name The name of the field.
     * @param bitWidth The number of bits allocated for this field.
     * @param isSigned Whether the field represents a signed value.
     * 
     * @throws std::runtime_error if bitWidth is 0.
     */
    FieldConfig(const std::string& name, uint8_t bitWidth, bool isSigned);

    /**
     * @brief Gets the name of the field.
     * 
     * @return const std::string& The field name.
     */
    const std::string& getName() const;

    /**
     * @brief Gets the bit width of the field.
     * 
     * @return uint8_t The number of bits allocated for this field.
     */
    uint8_t getBitWidth() const;

    /**
     * @brief Checks if the field is signed.
     * 
     * @return true if the field represents a signed value.
     * @return false if the field represents an unsigned value.
     */
    bool isSigned() const;

    /**
     * @brief Validates a value against the field's constraints.
     * 
     * @param value The value to validate.
     * @return true if the value is within the valid range for this field.
     * @return false if the value is outside the valid range.
     */
    bool isValidValue(int64_t value) const;

    /**
     * @brief Gets the maximum valid value for this field.
     * 
     * For unsigned fields, this is 2^bitWidth - 1.
     * For signed fields, this is 2^(bitWidth-1) - 1.
     * 
     * @return int64_t The maximum valid value.
     */
    int64_t getMaxValue() const;

    /**
     * @brief Gets the minimum valid value for this field.
     * 
     * For unsigned fields, this is 0.
     * For signed fields, this is -2^(bitWidth-1).
     * 
     * @return int64_t The minimum valid value.
     */
    int64_t getMinValue() const;

private:
    std::string name;
    uint8_t bitWidth;
    bool signedField;
};

} // namespace BinaryMessage 