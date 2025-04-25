#include "BinaryMessageFactory.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using namespace BinaryMessageLibrary;

int main() {
    try {
        // Load message definitions from file
        std::ifstream configFile("message_definitions.json");
        nlohmann::json config;
        configFile >> config;

        // Create factory with the configuration
        BinaryMessageFactory factory(config);

        // Print available message types
        std::cout << "Available message types:" << std::endl;
        for (const auto& type : factory.getMessageTypes()) {
            std::cout << "  - " << type << std::endl;
        }
        std::cout << std::endl;

        // Create and use a status message
        auto statusMsg = factory.createMessage("status_message");
        statusMsg->setField("device_id", 42);
        statusMsg->setField("status_code", 3);
        statusMsg->setField("error_code", 0);
        
        std::cout << "Status Message:" << std::endl;
        std::cout << "  Device ID: " << statusMsg->getField("device_id") << std::endl;
        std::cout << "  Status Code: " << statusMsg->getField("status_code") << std::endl;
        std::cout << "  Error Code: " << statusMsg->getField("error_code") << std::endl;
        std::cout << std::endl;

        // Create and use a sensor data message
        auto sensorMsg = factory.createMessage("sensor_data");
        sensorMsg->setField("sensor_id", 15);
        sensorMsg->setField("temperature", -125);  // -12.5°C
        sensorMsg->setField("humidity", 75);
        sensorMsg->setField("battery_level", 12);
        
        std::cout << "Sensor Data:" << std::endl;
        std::cout << "  Sensor ID: " << sensorMsg->getField("sensor_id") << std::endl;
        std::cout << "  Temperature: " << sensorMsg->getField("temperature") << " (0.1°C)" << std::endl;
        std::cout << "  Humidity: " << sensorMsg->getField("humidity") << "%" << std::endl;
        std::cout << "  Battery Level: " << sensorMsg->getField("battery_level") << std::endl;
        std::cout << std::endl;

        // Demonstrate packing and unpacking
        auto buffer = sensorMsg->pack();
        auto unpackedMsg = factory.createMessage("sensor_data");
        unpackedMsg->unpack(buffer);
        
        std::cout << "Unpacked Sensor Data:" << std::endl;
        std::cout << "  Sensor ID: " << unpackedMsg->getField("sensor_id") << std::endl;
        std::cout << "  Temperature: " << unpackedMsg->getField("temperature") << " (0.1°C)" << std::endl;
        std::cout << "  Humidity: " << unpackedMsg->getField("humidity") << "%" << std::endl;
        std::cout << "  Battery Level: " << unpackedMsg->getField("battery_level") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 