#pragma once
#include "chimera_core/gl/ParticleEmitter.hpp"
#include "ecs.hpp"

namespace ce {

    struct EmitterComponent {
        IEmitter* emitter{nullptr};
        TagInfo tag;
        EmitterComponent() = default;
    };
} // namespace ce
