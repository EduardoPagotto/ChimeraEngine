#pragma once
#include <stdint.h>
#include <string>

namespace ce {
struct TagComponent {
    std::string tag = "";
    std::string id = "";
    uint32_t serial = 0;
    TagComponent() = default;
};
} // namespace ce