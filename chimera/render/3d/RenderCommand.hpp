#pragma once
#include "IRenderable3d.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/Texture.hpp"
#include <vector>

namespace Chimera {

struct RenderCommand {
    RenderCommand() = default;
    IRenderable3d* renderable = nullptr;
    glm::mat4 transform = glm::mat4(1.0f);
    Shader shader;
    std::vector<UValue> uniforms;
    std::vector<Texture*> vTex;
    bool logRender = false; // TODO: remover depois
};
} // namespace Chimera
