#pragma once
#include "chimera/core/gl/Shader.hpp"
#include "ecs.hpp"

namespace ce {

    struct ShaderComponent {
        TagInfo tag; // FIXME: trocar no nome depois
        ShaderComponent(const std::string& name, std::shared_ptr<Shader> shader) : name(name), shader(shader) {}
        virtual ~ShaderComponent() = default;
        std::string name;
        std::shared_ptr<Shader> shader;
    };
} // namespace ce
