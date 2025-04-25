#include "FieldConfig.hpp"
#include <stdexcept>
#include <cmath>

namespace BinaryMessageLibrary {

FieldConfig::FieldConfig(const std::string& name, uint8_t bitWidth, bool isSigned)
    : name_(name), bit_width_(bitWidth), is_signed_(isSigned) {
    if (bitWidth == 0) {
        throw std::runtime_error("Field bit width cannot be 0");
    }
}

bool FieldConfig::isValidValue(int64_t value) const {
    return value >= getMinValue() && value <= getMaxValue();
}

int64_t FieldConfig::getMaxValue() const {
    if (is_signed_) {
        return (1LL << (bit_width_ - 1)) - 1;
    }
    return (1LL << bit_width_) - 1;
}

int64_t FieldConfig::getMinValue() const {
    if (is_signed_) {
        return -(1LL << (bit_width_ - 1));
    }
    return 0;
}

} // namespace BinaryMessageLibrary 