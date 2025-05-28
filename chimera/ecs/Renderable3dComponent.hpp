#pragma once
#include "ecs.hpp"

namespace ce {
    struct Renderable3dComponent {
        TagInfo tag;
        Renderable3D* renderable; // TODO: Smartpointer!!!!
    };

    struct RenderableParticlesComponent {
        Renderable3D* renderable;
        bool enable;
    };

} // namespace ce
