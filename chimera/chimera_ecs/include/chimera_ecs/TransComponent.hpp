#pragma once
#include "chimera_base/aux/Transform.hpp"
#include "ecs.hpp"

namespace ce {

    struct TransComponent {
        TagInfo tag;
        ITrans* trans = nullptr;
        bool solid = false;
        TransComponent() = default;
    };
} // namespace ce
