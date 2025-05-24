#pragma once
#include "chimera/core/visible/Mesh.hpp"
#include "ecs.hpp"

namespace ce {

    struct MeshComponent {
        TagInfo tag;
        MeshType type = MeshType::SIMPLE;
        Mesh* mesh = new Mesh();
        std::vector<TrisIndex> vTrisIndex;
        MeshComponent() = default;
    };
} // namespace ce
