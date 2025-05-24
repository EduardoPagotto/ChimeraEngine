#pragma once
#include "chimera/core/visible/Light.hpp"
#include "ecs.hpp"

namespace ce {

    struct LightComponent {
        TagInfo tag;
        Light* light = new Light();
        bool global = true;
        LightComponent() = default;
    };
} // namespace ce
