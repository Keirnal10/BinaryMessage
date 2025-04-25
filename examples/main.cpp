#include "BinaryMessage.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

std::filesystem::path find_config_file() {
    // First try current directory
    std::filesystem::path current_dir_config = "sample_config.json";
    if (std::filesystem::exists(current_dir_config)) {
        return current_dir_config;
    }
    
    // Then try examples directory
    std::filesystem::path examples_config = std::filesystem::current_path() / ".." / "examples" / "sample_config.json";
    if (std::filesystem::exists(examples_config)) {
        return examples_config;
    }
    
    throw std::runtime_error("Could not find sample_config.json in current directory or examples directory");
}

int main() {
    try {
        // Find and load configuration file
        std::filesystem::path config_path = find_config_file();
        std::ifstream config_file(config_path);
        if (!config_file.is_open()) {
            throw std::runtime_error("Could not open configuration file: " + config_path.string());
        }
        
        nlohmann::json config;
        config_file >> config;

        // Create message configuration
        BinaryMessage::MessageConfig message_config(config);

        // Create a message
        BinaryMessage::BinaryMessage message(message_config);

        // Set some values
        message.setField("status", 2);
        message.setField("temperature", -123);
        message.setField("pressure", 2047);
        message.setField("flags", 0xAA);

        // Pack the message
        auto buffer = message.pack();

        // Print the packed buffer
        std::cout << "Packed buffer: ";
        for (uint8_t byte : buffer) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;

        // Create a new message and unpack
        BinaryMessage::BinaryMessage unpacked_message(message_config);
        unpacked_message.unpack(buffer);

        // Print unpacked values
        std::cout << "\nUnpacked values:" << std::endl;
        std::cout << "Status: " << unpacked_message.getField("status") << std::endl;
        std::cout << "Temperature: " << unpacked_message.getField("temperature") << std::endl;
        std::cout << "Pressure: " << unpacked_message.getField("pressure") << std::endl;
        std::cout << "Flags: 0x" << std::hex << unpacked_message.getField("flags") << std::dec << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 