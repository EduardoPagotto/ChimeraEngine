#pragma once
#include "chimera_core/visible/Light.hpp"
#include "ecs.hpp"
#include <memory>

namespace ce {

    struct LightComponent {
        TagInfo tag;
        std::shared_ptr<Light> light;
        bool global = true;
        LightComponent() = default;
    };
} // namespace ce
