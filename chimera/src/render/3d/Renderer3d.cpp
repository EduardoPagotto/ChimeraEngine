#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Renderer3d::Renderer3d() : camera(nullptr), logData(false), totIBO(0), totFaces(0) {
    // commandQueue.reserve(1000);
    uniformsQueue.reserve(300);
}

Renderer3d::~Renderer3d() {}

void Renderer3d::begin(ICamera* camera) {

    this->camera = camera;
    if (this->camera != nullptr) {
        frustum.set(camera->getViewProjectionMatrixInverse());
    }

    uniformsQueue.clear();

    // debug data
    totIBO = 0;
    totFaces = 0;
}
void Renderer3d::end() {
    if (logData == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBOs: %d Faces: %d", totIBO, totFaces);
}

void Renderer3d::submitLight(Light* light) { light->bindLightInformation(uniformsQueue); }

void Renderer3d::submit(const RenderCommand& command) {

    // se não há frustrum adicionar tudo // TODO: remover quando model estiver no fluxo principal
    if (this->camera == nullptr) {
        commandQueue.push_back(command);
        return;
    }
    // Transformation model matrix AABB to know if in frustrum Camera
    const AABB& aabb = command.renderable->getAABB();
    AABB nova = aabb.transformation(command.transform);
    // adicione apenas o que esta no clip-space
    if (nova.visible(frustum) == true) {
        // Debug info only
        IndexBuffer* ibo = command.renderable->getIBO();
        if (ibo != nullptr) {
            totIBO++;
            totFaces += ibo->getCount();
        }
        // adicionado ao proximo render
        commandQueue.push_back(command);
    }
}

void Renderer3d::flush() {

    Shader activeShader;
    VertexArray* pLastVao = nullptr;
    while (!commandQueue.empty()) {

        const RenderCommand& command = commandQueue.front();
        IRenderable3d* r = command.renderable;

        if (r->getVao() != nullptr) {      // Possui um novo modelo
            if (r->getVao() != pLastVao) { // Diferente  do anterior
                if (pLastVao != nullptr) { // desvincula o anterior
                    pLastVao->unbind();
                }

                r->getVao()->bind(); // vincula novo modelo
                pLastVao = r->getVao();

                if (activeShader.isInvalid()) { // primeira passada
                    activeShader = command.shader;
                    activeShader.enable();
                } else {
                    // demais passadas
                    if (activeShader != command.shader) {  // se diferente
                        if (!command.shader.isInvalid()) { // se valido trocar
                            activeShader.disable();
                            activeShader = command.shader;
                            activeShader.enable();
                        }
                    }
                }

                // generic bind in each draw call camera, light, etc
                for (const UniformVal& uniform : uniformsQueue)
                    uniform.setUniform(activeShader);

                // bind dos uniforms from model
                for (const UniformVal& uniform : command.uniforms)
                    uniform.setUniform(activeShader);

                // TODO: ver se é assim mesmo!!!!
                if (command.vTex.size() == 0)
                    Texture::unbind(0);

                // bind de texturas
                for (uint8_t i = 0; i < command.vTex.size(); i++) {
                    command.vTex[i]->bind(i);
                }
            }
        }

        // Desenhar o IBO
        if (r->getIBO() != nullptr) {

            r->getIBO()->bind();

            glDrawElements(GL_TRIANGLES, r->getIBO()->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

            if (logData == true)
                r->debugDados();

            r->getIBO()->unbind();
        }
        commandQueue.pop_front();
    }
    pLastVao->unbind();
}

} // namespace Chimera