#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/visible/Light.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Renderer3d::Renderer3d() : totIBO(0), totFaces(0) { uniformsQueue.reserve(300); }

Renderer3d::~Renderer3d() {}

void Renderer3d::begin(ICamera* camera) {
    frustum.set(camera->view()->getViewProjectionInverse());
    // debug data
    totIBO = 0;
    totFaces = 0;
}
void Renderer3d::end() {}

void Renderer3d::submit(const RenderCommand& command) {
    // Transformation model matrix AABB to know if in frustrum Camera
    const AABB& aabb = command.renderable->getAABB();
    AABB nova = aabb.transformation(command.transform);
    // adicione apenas o que esta no clip-space
    if (nova.visible(frustum) == true) {
        // Debug info only
        IndexBuffer* ibo = command.renderable->getIBO();
        if (ibo != nullptr) {
            totIBO++;
            totFaces += ibo->getSize() / 3;
        }
        // adicionado ao proximo render
        commandQueue.push_back(command);
    }
}

void Renderer3d::flush() {

    Shader activeShader;
    VertexArray* pLastVao = nullptr;
    bool logData = false;
    while (!commandQueue.empty()) {

        const RenderCommand& command = commandQueue.front();
        IRenderable3d* r = command.renderable;
        logData = command.logRender;

        if (r->getVao() != nullptr) {      // Possui um novo modelo
            if (r->getVao() != pLastVao) { // Diferente  do anterior
                if (pLastVao != nullptr) { // desvincula o anterior
                    pLastVao->unbind();
                }

                r->getVao()->bind(); // vincula novo modelo
                pLastVao = r->getVao();

                if (activeShader.isInvalid()) { // primeira passada
                    activeShader = command.shader;
                    glUseProgram(activeShader.getID());
                } else {
                    // demais passadas
                    if (activeShader != command.shader) {  // se diferente
                        if (!command.shader.isInvalid()) { // se valido trocar
                            glUseProgram(0);
                            activeShader = command.shader;
                            glUseProgram(activeShader.getID());
                        }
                    }
                }

                // generic bind in each draw call camera, light, etc
                for (const auto& kv : uniformsQueue)
                    activeShader.setUniformU(kv.first.c_str(), kv.second);

                // bind dos uniforms from model
                for (const auto& kv : command.uniforms)
                    activeShader.setUniformU(kv.first.c_str(), kv.second);

                // libera textura antes de passar as novas
                if (command.vTex.size() == 0)
                    Texture::unbind(0);

                // bind de texturas
                for (uint8_t i = 0; i < command.vTex.size(); i++) {
                    command.vTex[i]->bind(i);
                }
            }
        }

        r->draw(command.logRender);
        commandQueue.pop_front();
    }
    pLastVao->unbind();

    // Limpa buffer de uniforms ao terminar todos os draws calls
    uniformsQueue.clear();

    if (logData == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBOs: %d Faces: %d", totIBO, totFaces);
}

} // namespace Chimera