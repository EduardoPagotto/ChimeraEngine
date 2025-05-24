#pragma once
#include "chimera/render/2d/Tile.hpp"
#include "ecs.hpp"

namespace ce {
    struct ComponentTile {
        TagInfo tag;
        Tile* tile = nullptr;
        ComponentTile() = default;
    };
} // namespace ce
