#pragma once
#include "chimera/base/ViewProjection.hpp"
#include "ecs.hpp"

namespace ce {
    struct ViewProjectionComponent {
        TagInfo tag;
        ViewProjectionComponent() = default;
        IViewProjection* vp = nullptr;
    };
} // namespace ce
