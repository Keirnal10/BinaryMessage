#include "BinaryMessageFactory.hpp"
#include <stdexcept>
#include <unordered_set>

namespace BinaryMessageLibrary {

BinaryMessageFactory::BinaryMessageFactory(const nlohmann::json& config) {
    loadConfigurations(config);
}

std::unique_ptr<BinaryMessage> BinaryMessageFactory::createMessage(const std::string& messageType) const {
    if (!hasMessageType(messageType)) {
        throw std::runtime_error("Message type '" + messageType + "' not found in configuration");
    }
    return std::make_unique<BinaryMessage>(getMessageConfig(messageType));
}

const MessageConfig& BinaryMessageFactory::getMessageConfig(const std::string& messageType) const {
    auto it = messageConfigs.find(messageType);
    if (it == messageConfigs.end()) {
        throw std::runtime_error("Message type '" + messageType + "' not found in configuration");
    }
    return it->second;
}

bool BinaryMessageFactory::hasMessageType(const std::string& messageType) const {
    return messageConfigs.find(messageType) != messageConfigs.end();
}

std::vector<std::string> BinaryMessageFactory::getMessageTypes() const {
    std::vector<std::string> types;
    types.reserve(messageConfigs.size());
    for (const auto& pair : messageConfigs) {
        types.push_back(pair.first);
    }
    return types;
}

void BinaryMessageFactory::loadConfigurations(const nlohmann::json& config) {
    if (!config.is_object()) {
        throw std::runtime_error("Configuration must be a JSON object");
    }

    for (const auto& [messageType, messageDef] : config.items()) {
        validateMessageDefinition(messageType, messageDef);
        messageConfigs[messageType] = MessageConfig(messageDef);
    }
}

void BinaryMessageFactory::validateMessageDefinition(const std::string& messageType, const nlohmann::json& messageDef) {
    if (!messageDef.is_array()) {
        throw std::runtime_error("Message definition for '" + messageType + "' must be an array");
    }

    // Check for duplicate field names
    std::unordered_set<std::string> fieldNames;
    for (const auto& field : messageDef) {
        if (!field.is_object()) {
            throw std::runtime_error("Field definition in message '" + messageType + "' must be an object");
        }

        if (!field.contains("name") || !field["name"].is_string()) {
            throw std::runtime_error("Field in message '" + messageType + "' must have a string 'name'");
        }

        if (!field.contains("bit_width") || !field["bit_width"].is_number_unsigned()) {
            throw std::runtime_error("Field in message '" + messageType + "' must have an unsigned 'bit_width'");
        }

        if (!field.contains("signed") || !field["signed"].is_boolean()) {
            throw std::runtime_error("Field in message '" + messageType + "' must have a boolean 'signed'");
        }

        std::string fieldName = field["name"];
        if (!fieldNames.insert(fieldName).second) {
            throw std::runtime_error("Duplicate field name '" + fieldName + "' in message '" + messageType + "'");
        }
    }
}

} // namespace BinaryMessageLibrary 