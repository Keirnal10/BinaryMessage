#ifndef BINARY_MESSAGE_FIELD_CONFIG_HPP
#define BINARY_MESSAGE_FIELD_CONFIG_HPP

#include <string>

namespace BinaryMessage {

struct FieldConfig {
    std::string name;
    size_t bit_width;
    bool is_signed;
};

} // namespace BinaryMessage

#endif // BINARY_MESSAGE_FIELD_CONFIG_HPP 