#ifndef __CHIMERA_RENDERCOMMAND__HPP
#define __CHIMERA_RENDERCOMMAND__HPP

#include "IRenderable3d.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Texture.hpp"
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
#endif