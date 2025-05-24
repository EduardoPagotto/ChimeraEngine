#pragma once
#include "chimera/base/Transform.hpp"
#include "ecs.hpp"

namespace ce {

    struct TransComponent {
        TagInfo tag;
        ITrans* trans = nullptr;
        bool solid = false;
        TransComponent() = default;
    };
} // namespace ce
