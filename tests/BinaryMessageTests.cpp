#include "BinaryMessage.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace BinaryMessage;

class BinaryMessageTest : public ::testing::Test {
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

TEST_F(BinaryMessageTest, FieldOperations) {
    BinaryMessage::BinaryMessage message(*message_config);

    // Test setting and getting fields
    message.setField("status", 2);
    EXPECT_EQ(message.getField("status"), 2);

    message.setField("value", -50);
    EXPECT_EQ(message.getField("value"), -50);

    message.setField("flags", 0xA);
    EXPECT_EQ(message.getField("flags"), 0xA);

    // Test invalid field name
    EXPECT_THROW(message.getField("nonexistent"), std::runtime_error);
}

TEST_F(BinaryMessageTest, FieldValueValidation) {
    BinaryMessage::BinaryMessage message(*message_config);

    // Test unsigned field range
    EXPECT_NO_THROW(message.setField("status", 0));
    EXPECT_NO_THROW(message.setField("status", 3));
    EXPECT_THROW(message.setField("status", 4), std::runtime_error);
    EXPECT_THROW(message.setField("status", -1), std::runtime_error);

    // Test signed field range
    EXPECT_NO_THROW(message.setField("value", -128));
    EXPECT_NO_THROW(message.setField("value", 127));
    EXPECT_THROW(message.setField("value", -129), std::runtime_error);
    EXPECT_THROW(message.setField("value", 128), std::runtime_error);
}

TEST_F(BinaryMessageTest, PackAndUnpack) {
    BinaryMessage::BinaryMessage message(*message_config);
    
    // Set test values
    message.setField("status", 2);
    message.setField("value", -50);
    message.setField("flags", 0xA);

    // Pack the message
    auto buffer = message.pack();
    
    // Create a new message and unpack
    BinaryMessage::BinaryMessage unpacked_message(*message_config);
    unpacked_message.unpack(buffer);

    // Verify unpacked values
    EXPECT_EQ(unpacked_message.getField("status"), 2);
    EXPECT_EQ(unpacked_message.getField("value"), -50);
    EXPECT_EQ(unpacked_message.getField("flags"), 0xA);
}

TEST_F(BinaryMessageTest, PackAndUnpackEdgeCases) {
    BinaryMessage::BinaryMessage message(*message_config);
    
    // Test maximum values
    message.setField("status", 3);
    message.setField("value", 127);
    message.setField("flags", 0xF);

    auto buffer = message.pack();
    BinaryMessage::BinaryMessage unpacked_message(*message_config);
    unpacked_message.unpack(buffer);

    EXPECT_EQ(unpacked_message.getField("status"), 3);
    EXPECT_EQ(unpacked_message.getField("value"), 127);
    EXPECT_EQ(unpacked_message.getField("flags"), 0xF);

    // Test minimum values
    message.setField("status", 0);
    message.setField("value", -128);
    message.setField("flags", 0);

    buffer = message.pack();
    unpacked_message.unpack(buffer);

    EXPECT_EQ(unpacked_message.getField("status"), 0);
    EXPECT_EQ(unpacked_message.getField("value"), -128);
    EXPECT_EQ(unpacked_message.getField("flags"), 0);
}

TEST_F(BinaryMessageTest, InvalidBufferSize) {
    BinaryMessage::BinaryMessage message(*message_config);
    std::vector<uint8_t> small_buffer = {0x00}; // Too small
    EXPECT_THROW(message.unpack(small_buffer), std::runtime_error);
}

TEST_F(BinaryMessageTest, PackUnpackMultipleMessages) {
    BinaryMessage::BinaryMessage message1(*message_config);
    BinaryMessage::BinaryMessage message2(*message_config);
    
    // Set different values in each message
    message1.setField("status", 1);
    message1.setField("value", 42);
    message1.setField("flags", 0x5);
    
    message2.setField("status", 2);
    message2.setField("value", -42);
    message2.setField("flags", 0xA);
    
    // Pack both messages
    auto buffer1 = message1.pack();
    auto buffer2 = message2.pack();
    
    // Unpack into new messages
    BinaryMessage::BinaryMessage unpacked1(*message_config);
    BinaryMessage::BinaryMessage unpacked2(*message_config);
    
    unpacked1.unpack(buffer1);
    unpacked2.unpack(buffer2);
    
    // Verify values are preserved
    EXPECT_EQ(unpacked1.getField("status"), 1);
    EXPECT_EQ(unpacked1.getField("value"), 42);
    EXPECT_EQ(unpacked1.getField("flags"), 0x5);
    
    EXPECT_EQ(unpacked2.getField("status"), 2);
    EXPECT_EQ(unpacked2.getField("value"), -42);
    EXPECT_EQ(unpacked2.getField("flags"), 0xA);
} 