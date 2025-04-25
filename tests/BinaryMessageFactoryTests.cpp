#include "BinaryMessageFactory.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using namespace BinaryMessageLibrary;

class BinaryMessageFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a valid test configuration
        validConfig = R"({
            "status_message": [
                {
                    "name": "device_id",
                    "bit_width": 8,
                    "signed": false
                },
                {
                    "name": "status_code",
                    "bit_width": 4,
                    "signed": false
                }
            ],
            "sensor_data": [
                {
                    "name": "sensor_id",
                    "bit_width": 6,
                    "signed": false
                },
                {
                    "name": "temperature",
                    "bit_width": 10,
                    "signed": true
                }
            ]
        })"_json;

        // Create an invalid test configuration (missing required fields)
        invalidConfig = R"({
            "invalid_message": [
                {
                    "name": "field1"
                }
            ]
        })"_json;
    }

    nlohmann::json validConfig;
    nlohmann::json invalidConfig;
};

TEST_F(BinaryMessageFactoryTest, ConstructorWithValidConfig) {
    EXPECT_NO_THROW({
        BinaryMessageFactory factory(validConfig);
    });
}

TEST_F(BinaryMessageFactoryTest, ConstructorWithInvalidConfig) {
    EXPECT_THROW({
        BinaryMessageFactory factory(invalidConfig);
    }, std::runtime_error);
}

TEST_F(BinaryMessageFactoryTest, CreateMessage) {
    BinaryMessageFactory factory(validConfig);
    
    // Test creating a valid message type
    EXPECT_NO_THROW({
        auto message = factory.createMessage("status_message");
        EXPECT_NE(message, nullptr);
    });

    // Test creating an invalid message type
    EXPECT_THROW({
        factory.createMessage("nonexistent_message");
    }, std::runtime_error);
}

TEST_F(BinaryMessageFactoryTest, GetMessageConfig) {
    BinaryMessageFactory factory(validConfig);
    
    // Test getting a valid message config
    EXPECT_NO_THROW({
        const auto& config = factory.getMessageConfig("status_message");
        EXPECT_EQ(config.getFields().size(), 2);
    });

    // Test getting an invalid message config
    EXPECT_THROW({
        factory.getMessageConfig("nonexistent_message");
    }, std::runtime_error);
}

TEST_F(BinaryMessageFactoryTest, HasMessageType) {
    BinaryMessageFactory factory(validConfig);
    
    EXPECT_TRUE(factory.hasMessageType("status_message"));
    EXPECT_TRUE(factory.hasMessageType("sensor_data"));
    EXPECT_FALSE(factory.hasMessageType("nonexistent_message"));
}

TEST_F(BinaryMessageFactoryTest, GetMessageTypes) {
    BinaryMessageFactory factory(validConfig);
    
    auto types = factory.getMessageTypes();
    EXPECT_EQ(types.size(), 2);
    EXPECT_TRUE(std::find(types.begin(), types.end(), "status_message") != types.end());
    EXPECT_TRUE(std::find(types.begin(), types.end(), "sensor_data") != types.end());
}

TEST_F(BinaryMessageFactoryTest, MessageFieldOperations) {
    BinaryMessageFactory factory(validConfig);
    
    auto message = factory.createMessage("status_message");
    
    // Test setting and getting fields
    EXPECT_NO_THROW({
        message->setField("device_id", 42);
        message->setField("status_code", 3);
    });

    EXPECT_EQ(message->getField("device_id"), 42);
    EXPECT_EQ(message->getField("status_code"), 3);

    // Test setting invalid field
    EXPECT_THROW({
        message->setField("nonexistent_field", 0);
    }, std::runtime_error);
}

TEST_F(BinaryMessageFactoryTest, PackAndUnpack) {
    BinaryMessageFactory factory(validConfig);
    
    // Create and pack a message
    auto originalMessage = factory.createMessage("sensor_data");
    originalMessage->setField("sensor_id", 15);
    originalMessage->setField("temperature", -125);
    
    auto buffer = originalMessage->pack();
    
    // Create a new message and unpack the buffer
    auto unpackedMessage = factory.createMessage("sensor_data");
    unpackedMessage->unpack(buffer);
    
    // Verify values are preserved
    EXPECT_EQ(unpackedMessage->getField("sensor_id"), 15);
    EXPECT_EQ(unpackedMessage->getField("temperature"), -125);
}

TEST_F(BinaryMessageFactoryTest, InvalidBufferSize) {
    BinaryMessageFactory factory(validConfig);
    
    auto message = factory.createMessage("sensor_data");
    std::vector<uint8_t> buffer(1); // Too small buffer
    
    EXPECT_THROW({
        message->unpack(buffer);
    }, std::runtime_error);
}

TEST_F(BinaryMessageFactoryTest, DuplicateFieldNames) {
    nlohmann::json config = R"({
        "invalid_message": [
            {
                "name": "field1",
                "bit_width": 8,
                "signed": false
            },
            {
                "name": "field1",
                "bit_width": 4,
                "signed": false
            }
        ]
    })"_json;
    
    EXPECT_THROW({
        BinaryMessageFactory factory(config);
    }, std::runtime_error);
}

TEST_F(BinaryMessageFactoryTest, InvalidFieldDefinition) {
    nlohmann::json config = R"({
        "invalid_message": [
            {
                "name": "field1",
                "bit_width": "not_a_number",
                "signed": false
            }
        ]
    })"_json;
    
    EXPECT_THROW({
        BinaryMessageFactory factory(config);
    }, std::runtime_error);
} 