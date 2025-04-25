#include "BinaryMessage.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace BinaryMessage;

class MessageConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple test configuration
        config_json = R"([
            {
                "name": "status",
                "bit_width": 2,
                "signed": false
            },
            {
                "name": "value",
                "bit_width": 8,
                "signed": true
            },
            {
                "name": "flags",
                "bit_width": 4,
                "signed": false
            }
        ])"_json;
        
        message_config = std::make_unique<MessageConfig>(config_json);
    }

    nlohmann::json config_json;
    std::unique_ptr<MessageConfig> message_config;
};

TEST_F(MessageConfigTest, MessageConfigCreation) {
    EXPECT_EQ(message_config->getFields().size(), 3);
    EXPECT_EQ(message_config->getTotalBits(), 14); // 2 + 8 + 4 bits
}

TEST_F(MessageConfigTest, InvalidConfigThrows) {
    MessageConfig config;
    
    // Test invalid JSON structure
    nlohmann::json invalid_config = R"({})"_json;
    EXPECT_THROW(config.setConfig(invalid_config), std::runtime_error);

    // Test invalid bit width
    nlohmann::json invalid_width = R"([
        {
            "name": "test",
            "bit_width": 0
        }
    ])"_json;
    EXPECT_THROW(config.setConfig(invalid_width), std::runtime_error);

    // Test missing required fields
    nlohmann::json missing_fields = R"([
        {
            "name": "test"
        }
    ])"_json;
    EXPECT_THROW(config.setConfig(missing_fields), std::runtime_error);

    // Test invalid field type
    nlohmann::json invalid_field_type = R"([
        {
            "name": 123,
            "bit_width": 8
        }
    ])"_json;
    EXPECT_THROW(config.setConfig(invalid_field_type), std::runtime_error);

    // Test invalid bit width type
    nlohmann::json invalid_bit_width_type = R"([
        {
            "name": "test",
            "bit_width": "8"
        }
    ])"_json;
    EXPECT_THROW(config.setConfig(invalid_bit_width_type), std::runtime_error);
} 