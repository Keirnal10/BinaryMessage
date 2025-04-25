#pragma once

#include "BinaryMessage.hpp"
#include "MessageConfig.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace BinaryMessageLibrary {

/**
 * @brief Factory class for creating BinaryMessage objects from configuration files.
 * 
 * This class manages multiple message definitions and provides methods to create
 * BinaryMessage objects for specific message types. The configuration file can
 * contain multiple message definitions, each with its own set of fields.
 */
class BinaryMessageFactory {
public:
    /**
     * @brief Constructs a new BinaryMessageFactory object.
     * 
     * @param config JSON object containing message definitions.
     * 
     * @throws std::runtime_error if the configuration is invalid or if any message
     *         definition is invalid.
     */
    explicit BinaryMessageFactory(const nlohmann::json& config);

    /**
     * @brief Creates a new BinaryMessage object for the specified message type.
     * 
     * @param messageType The type of message to create.
     * @return std::unique_ptr<BinaryMessage> A new BinaryMessage object.
     * 
     * @throws std::runtime_error if the message type is not found in the configuration.
     */
    std::unique_ptr<BinaryMessage> createMessage(const std::string& messageType) const;

    /**
     * @brief Gets the configuration for a specific message type.
     * 
     * @param messageType The type of message to get configuration for.
     * @return const MessageConfig& The message configuration.
     * 
     * @throws std::runtime_error if the message type is not found in the configuration.
     */
    const MessageConfig& getMessageConfig(const std::string& messageType) const;

    /**
     * @brief Checks if a message type exists in the configuration.
     * 
     * @param messageType The type of message to check.
     * @return true if the message type exists.
     * @return false if the message type does not exist.
     */
    bool hasMessageType(const std::string& messageType) const;

    /**
     * @brief Gets a list of all available message types.
     * 
     * @return std::vector<std::string> List of message type names.
     */
    std::vector<std::string> getMessageTypes() const;

private:
    std::unordered_map<std::string, MessageConfig> messageConfigs;

    /**
     * @brief Loads message configurations from the JSON configuration.
     * 
     * @param config JSON object containing message definitions.
     * 
     * @throws std::runtime_error if the configuration is invalid.
     */
    void loadConfigurations(const nlohmann::json& config);

    /**
     * @brief Validates a message definition in the configuration.
     * 
     * @param messageType The type of message being validated.
     * @param messageDef The message definition to validate.
     * 
     * @throws std::runtime_error if the message definition is invalid.
     */
    void validateMessageDefinition(const std::string& messageType, const nlohmann::json& messageDef);
};

} // namespace BinaryMessageLibrary 