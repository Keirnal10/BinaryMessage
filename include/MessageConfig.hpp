#pragma once

#include "FieldConfig.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>

namespace BinaryMessage {

/**
 * @brief Configuration class for a binary message layout.
 * 
 * This class manages the configuration of a binary message, including all its fields
 * and their properties. It provides methods to validate field configurations and
 * calculate the total size of the message in bits.
 */
class MessageConfig {
public:
    /**
     * @brief Default constructor.
     * 
     * Creates an empty message configuration.
     */
    MessageConfig();

    /**
     * @brief Constructs a new MessageConfig object from a JSON configuration.
     * 
     * @param config JSON array containing field configurations.
     * 
     * @throws std::runtime_error if the JSON configuration is invalid or if any field
     *         configuration is invalid (e.g., duplicate field names, invalid bit widths).
     */
    explicit MessageConfig(const nlohmann::json& config);

    /**
     * @brief Sets the message configuration from a JSON configuration.
     * 
     * @param config JSON array containing field configurations.
     * 
     * @throws std::runtime_error if the JSON configuration is invalid or if any field
     *         configuration is invalid (e.g., duplicate field names, invalid bit widths).
     */
    void setConfig(const nlohmann::json& config);

    /**
     * @brief Gets the list of field configurations.
     * 
     * @return const std::vector<FieldConfig>& Vector of field configurations.
     */
    const std::vector<FieldConfig>& getFields() const;

    /**
     * @brief Gets the total number of bits required for the message.
     * 
     * This is the sum of all field bit widths.
     * 
     * @return size_t Total number of bits.
     */
    size_t getTotalBits() const;

    /**
     * @brief Gets the field configuration for a specific field name.
     * 
     * @param name The name of the field to find.
     * @return const FieldConfig& Reference to the field configuration.
     * 
     * @throws std::runtime_error if no field with the given name exists.
     */
    const FieldConfig& getFieldConfig(const std::string& name) const;

    /**
     * @brief Checks if a field with the given name exists in the configuration.
     * 
     * @param name The name of the field to check.
     * @return true if the field exists.
     * @return false if the field does not exist.
     */
    bool hasField(const std::string& name) const;

private:
    std::vector<FieldConfig> fields;
    size_t totalBits;

    /**
     * @brief Validates the JSON configuration.
     * 
     * @param config The JSON configuration to validate.
     * 
     * @throws std::runtime_error if the configuration is invalid.
     */
    void validateConfig(const nlohmann::json& config);

    /**
     * @brief Calculates the total number of bits required for the message.
     */
    void calculateTotalBits();
};

} // namespace BinaryMessage 