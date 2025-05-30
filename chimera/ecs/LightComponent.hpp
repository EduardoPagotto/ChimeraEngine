#pragma once
#include "chimera/core/visible/Light.hpp"
#include "ecs.hpp"

namespace ce {

    struct LightComponent {
        TagInfo tag;
        std::shared_ptr<Light> light;
        bool global = true;
        LightComponent() = default;
    };
} // namespace ce
