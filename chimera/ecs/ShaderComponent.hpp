#pragma once
#include "chimera/core/gl/Shader.hpp"
#include "ecs.hpp"

namespace ce {

    struct ShaderComponent {
        TagInfo tag; // FIXME: trocar no nome depois
        std::shared_ptr<Shader> shader;
    };
} // namespace ce
