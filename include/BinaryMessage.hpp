#ifndef BINARY_MESSAGE_BINARY_MESSAGE_HPP
#define BINARY_MESSAGE_BINARY_MESSAGE_HPP

#include "MessageConfig.hpp"
#include <vector>
#include <string>

namespace BinaryMessage {

class BinaryMessage {
public:
    explicit BinaryMessage(const MessageConfig& config);
    
    void setField(const std::string& name, int64_t value);
    int64_t getField(const std::string& name) const;
    
    std::vector<uint8_t> pack() const;
    void unpack(const std::vector<uint8_t>& buffer);

private:
    const MessageConfig& config_;
    std::vector<int64_t> field_values_;
    
    size_t getFieldOffset(const std::string& name) const;
    void validateFieldValue(const std::string& name, int64_t value) const;
};

} // namespace BinaryMessage

#endif // BINARY_MESSAGE_BINARY_MESSAGE_HPP 