#pragma once
#include "chimera_core/gl/ParticleEmitter.hpp"
#include "ecs.hpp"

namespace ce {

    struct EmitterComponent {
        IEmitter* emitter;
        TagInfo tag;
        EmitterComponent() : emitter(nullptr) {}
    };
} // namespace ce
