#pragma once
#include "chimera/core/visible/Shader.hpp"
#include "chimera/core/visible/Texture.hpp"
#include <vector>

namespace Chimera {

struct RenderCommand {
    RenderCommand() = default;
    glm::mat4 transform = glm::mat4(1.0f);
    Shader shader;
    MapUniform uniforms;
    std::vector<std::shared_ptr<Texture>> vTex;
};
} // namespace Chimera
