#pragma once
#include "chimera/core/gl/Material.hpp"
#include "ecs.hpp"

namespace ce {

    struct MaterialComponent {
        TagInfo tag;
        std::shared_ptr<Material> material;
        MaterialComponent() = default;
    };
} // namespace ce
