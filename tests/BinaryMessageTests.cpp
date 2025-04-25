#include "BinaryMessage.hpp"
#include "MessageConfig.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>

using namespace BinaryMessageLibrary;

class BinaryMessageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple configuration for testing
        nlohmann::json config = R"([
            {
                "name": "field1",
                "bit_width": 8,
                "signed": false
            },
            {
                "name": "field2",
                "bit_width": 4,
                "signed": true
            }
        ])"_json;

        messageConfig = std::make_unique<MessageConfig>(config);
    }

    std::unique_ptr<MessageConfig> messageConfig;
};

TEST_F(BinaryMessageTest, FieldOperations) {
    auto message = std::make_unique<BinaryMessage>(*messageConfig);
    
    // Test setting and getting fields
    EXPECT_NO_THROW({
        message->setField("field1", 42);
        message->setField("field2", -3);
    });

    EXPECT_EQ(message->getField("field1"), 42);
    EXPECT_EQ(message->getField("field2"), -3);

    // Test setting invalid field
    EXPECT_THROW({
        message->setField("nonexistent_field", 0);
    }, std::runtime_error);
}

TEST_F(BinaryMessageTest, FieldValueValidation) {
    auto message = std::make_unique<BinaryMessage>(*messageConfig);
    
    // Test unsigned field range
    EXPECT_NO_THROW(message->setField("field1", 0));
    EXPECT_NO_THROW(message->setField("field1", 255));
    EXPECT_THROW(message->setField("field1", -1), std::runtime_error);
    EXPECT_THROW(message->setField("field1", 256), std::runtime_error);

    // Test signed field range
    EXPECT_NO_THROW(message->setField("field2", -8));
    EXPECT_NO_THROW(message->setField("field2", 7));
    EXPECT_THROW(message->setField("field2", -9), std::runtime_error);
    EXPECT_THROW(message->setField("field2", 8), std::runtime_error);
}

TEST_F(BinaryMessageTest, PackAndUnpack) {
    auto message = std::make_unique<BinaryMessage>(*messageConfig);
    
    // Set some values
    message->setField("field1", 42);
    message->setField("field2", -3);
    
    // Pack the message
    auto buffer = message->pack();
    
    // Create a new message and unpack
    auto unpackedMessage = std::make_unique<BinaryMessage>(*messageConfig);
    unpackedMessage->unpack(buffer);
    
    // Verify values are preserved
    EXPECT_EQ(unpackedMessage->getField("field1"), 42);
    EXPECT_EQ(unpackedMessage->getField("field2"), -3);
}

TEST_F(BinaryMessageTest, PackAndUnpackEdgeCases) {
    auto message = std::make_unique<BinaryMessage>(*messageConfig);
    
    // Test maximum and minimum values
    message->setField("field1", 255);  // Max unsigned
    message->setField("field2", -8);   // Min signed
    
    auto buffer = message->pack();
    auto unpackedMessage = std::make_unique<BinaryMessage>(*messageConfig);
    unpackedMessage->unpack(buffer);
    
    EXPECT_EQ(unpackedMessage->getField("field1"), 255);
    EXPECT_EQ(unpackedMessage->getField("field2"), -8);
}

TEST_F(BinaryMessageTest, InvalidBufferSize) {
    auto message = std::make_unique<BinaryMessage>(*messageConfig);
    std::vector<uint8_t> buffer(1); // Too small buffer
    
    EXPECT_THROW({
        message->unpack(buffer);
    }, std::runtime_error);
}

TEST_F(BinaryMessageTest, PackUnpackMultipleMessages) {
    auto message1 = std::make_unique<BinaryMessage>(*messageConfig);
    auto message2 = std::make_unique<BinaryMessage>(*messageConfig);
    
    // Set different values in each message
    message1->setField("field1", 42);
    message1->setField("field2", -3);
    
    message2->setField("field1", 100);
    message2->setField("field2", 5);
    
    // Pack both messages
    auto buffer1 = message1->pack();
    auto buffer2 = message2->pack();
    
    // Create new messages and unpack
    auto unpacked1 = std::make_unique<BinaryMessage>(*messageConfig);
    auto unpacked2 = std::make_unique<BinaryMessage>(*messageConfig);
    
    unpacked1->unpack(buffer1);
    unpacked2->unpack(buffer2);
    
    // Verify values are preserved
    EXPECT_EQ(unpacked1->getField("field1"), 42);
    EXPECT_EQ(unpacked1->getField("field2"), -3);
    EXPECT_EQ(unpacked2->getField("field1"), 100);
    EXPECT_EQ(unpacked2->getField("field2"), 5);
} 