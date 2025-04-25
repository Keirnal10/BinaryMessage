# Binary Message Library

A C++ library for handling binary messages with configurable field layouts. This library provides a flexible way to define, pack, and unpack binary messages with various field types and bit widths.

## Features

- Configurable message layouts using JSON
- Support for signed and unsigned fields
- Variable bit-width fields
- Efficient binary packing and unpacking
- Comprehensive unit tests
- CMake build system

## Requirements

- C++17 or later
- CMake 3.14 or later
- nlohmann/json library
- Google Test (automatically downloaded during build)

## Building the Project

### Prerequisites

1. Install CMake
2. Install a C++ compiler that supports C++17
3. Install nlohmann/json (or let CMake download it)

### Build Steps

```bash
# Create build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build .

# Run tests
ctest
```

## Usage

### Basic Example

```cpp
#include "BinaryMessage.hpp"
#include <nlohmann/json.hpp>

using namespace BinaryMessage;

// Define message configuration
nlohmann::json config = R"([
    {
        "name": "status",
        "bit_width": 2,
        "signed": false
    },
    {
        "name": "value",
        "bit_width": 8,
        "signed": true
    }
])"_json;

// Create message configuration
MessageConfig message_config(config);

// Create a message
BinaryMessage message(message_config);

// Set field values
message.setField("status", 2);
message.setField("value", -50);

// Pack the message into a buffer
auto buffer = message.pack();

// Unpack the message
BinaryMessage unpacked_message(message_config);
unpacked_message.unpack(buffer);

// Get field values
int status = unpacked_message.getField("status");
int value = unpacked_message.getField("value");
```

## Message Configuration

The message configuration is defined using JSON with the following structure:

```json
[
    {
        "name": "field_name",
        "bit_width": number_of_bits,
        "signed": true/false
    }
]
```

Each field in the configuration must have:
- `name`: A string identifier for the field
- `bit_width`: The number of bits allocated for the field
- `signed`: Boolean indicating if the field is signed

## Testing

The project includes comprehensive unit tests using Google Test. To run the tests:

```bash
cd build
ctest
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request 