#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/core/buffer/IndexBuffer.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Renderer3d::Renderer3d(const bool& logData) : totIBO(0), totFaces(0), logData(logData) {
    uniformsQueue.reserve(500);
    renderQueue.reserve(500);
    textureQueue.reserve(50);
}

Renderer3d::~Renderer3d() {}

void Renderer3d::begin(Camera* camera, ViewProjection* vpo, Octree* octree) {
    this->camera = camera;
    this->vpo = vpo;
    this->octree = octree;
    frustum.set(vpo->getViewProjectionInverse());
    // debug data
    totIBO = 0;
    totFaces = 0;
}

void Renderer3d::end() {
    if (logData == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBOs: %d Faces: %d", totIBO, totFaces);
}

bool Renderer3d::submit(const RenderCommand& command, IRenderable3d* renderable) {
    // Transformation model matrix AABB to know if in frustrum Camera
    Renderable3D* r = (Renderable3D*)renderable;
    const AABB& aabb = r->getAABB();
    AABB nova = aabb.transformation(command.transform);

    if ((this->octree != nullptr) && (r->getIBO() != nullptr) && (vpo->getIndex() == 0)) {
        const AABB& a = r->getAABB();
        this->octree->insert(a.getPosition());
    }

    // adicione apenas o que esta no clip-space
    if (nova.visible(frustum) == true) {
        // Debug info only
        IndexBuffer* ibo = r->getIBO();
        if (ibo != nullptr) {
            totIBO++;
            totFaces += ibo->getSize() / 3;
        }
        // adicionado ao proximo render
        renderQueue.push_back(std::make_tuple(command, r));
        return true;
    }

    return false;
}

void Renderer3d::flush() {

    Shader activeShader;
    VertexArray* pLastVao = nullptr;

    for (auto& [command, r] : renderQueue) {

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
                for (uint8_t i = 0; i < command.vTex.size(); i++)
                    command.vTex[i]->bind(i);

                // bind de texturas globais
                for (uint8_t i = 0; i < textureQueue.size(); i++)
                    textureQueue[i]->bind(command.vTex.size() + i);
            }
        }

        r->draw(logData);
    }

    if (this->octree != nullptr)
        this->octree->debug_render();

    renderQueue.clear(); // Limpa rendercommand
    pLastVao->unbind();

    // Limpa buffer de uniforms ao terminar todos os draws calls
    uniformsQueue.clear();
    textureQueue.clear();
}

} // namespace Chimera