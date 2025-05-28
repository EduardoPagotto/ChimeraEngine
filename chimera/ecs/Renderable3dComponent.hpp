#pragma once
#include "ecs.hpp"

namespace ce {
    struct Renderable3dComponent {
        TagInfo tag;
        IRenderable3d* renderable; // TODO: Smartpointer!!!!
    };

    struct RenderableParticlesComponent {
        IRenderable3d* renderable;
        bool enable;
    };

} // namespace ce
