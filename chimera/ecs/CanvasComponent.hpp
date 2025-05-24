#pragma once
#include "chimera/base/interfaces.hpp"
#include "ecs.hpp"

namespace ce {

    struct CanvasComponent {
        TagInfo tag;
        std::shared_ptr<ICanva> canvas = nullptr;
        CanvasComponent() = default;
    };
} // namespace ce
