#pragma once
#include "chimera/core/gl/Material.hpp"
#include "ecs.hpp"

namespace ce {

    struct MaterialComponent {
        TagInfo tag;
        Material* material = new Material();
        MaterialComponent() = default;
    };
} // namespace ce
