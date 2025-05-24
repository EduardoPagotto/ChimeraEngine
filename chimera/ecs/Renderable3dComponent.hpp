#pragma once
#include "chimera/render/3d/IRenderable3d.hpp"
#include "ecs.hpp"

namespace ce {
    struct Renderable3dComponent {
        TagInfo tag;
        IRenderable3d* renderable;
    };

    struct RenderableParticlesComponent {
        IRenderable3d* renderable;
        bool enable;
    };

} // namespace ce
