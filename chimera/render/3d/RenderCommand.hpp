#pragma once

#include "IRenderable3d.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/Texture.hpp"
#include <vector>

namespace Chimera {

struct RenderCommand {
    IRenderable3d* renderable;
    glm::mat4 transform;
    Shader shader;
    std::vector<UniformVal> uniforms;
    std::vector<Texture*> vTex;
};
} // namespace Chimera
