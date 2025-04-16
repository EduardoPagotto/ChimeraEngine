#pragma once
#include "chimera/core/gl/Shader.hpp"
#include "chimera/core/gl/Texture.hpp"
#include <vector>

namespace ce {

struct RenderCommand {
    RenderCommand() = default;
    glm::mat4 transform = glm::mat4(1.0f);
    std::shared_ptr<Shader> shader;
    MapUniform uniforms;
    std::vector<std::shared_ptr<Texture>> vTex;
};
} // namespace ce
