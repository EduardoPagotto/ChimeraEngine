#pragma once
#include "chimera/render/2d/Tile.hpp"
#include "ecs.hpp"

namespace ce {
    struct TileComponent {
        TagInfo tag;
        Tile* tile = nullptr;
        TileComponent() = default;
    };
} // namespace ce
