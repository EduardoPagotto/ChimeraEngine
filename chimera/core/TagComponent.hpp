#pragma once
#include <string>

namespace Chimera {
struct TagComponent {
    std::string tag = "";
    std::string id = "";
    uint32_t serial = 0;
    TagComponent() = default;
};
} // namespace Chimera