#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/Shader.hpp"
#include "chimera/core/visible/Texture.hpp"
#include <vector>

namespace Chimera {

struct RenderCommand {
    RenderCommand() = default;
    IRenderable3d* renderable = nullptr;
    Camera* camera = nullptr;
    EyeView* eyeView = nullptr;
    glm::mat4 transform = glm::mat4(1.0f);
    Shader shader;
    MapUniform uniforms;
    std::vector<Texture*> vTex;
    bool logRender = false; // TODO: remover depois
};
} // namespace Chimera
