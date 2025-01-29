#pragma once
#include <format>
#include <stdint.h>
#include <string>

struct Range {
    std::string name;
    int32_t value{0}, value_max{0};
    Range() : name(""), value(0), value_max(0) {}
    inline const std::string show() const noexcept {
        return std::format("{}: {}/{} ", this->name, this->value, this->value_max);
    }
};
