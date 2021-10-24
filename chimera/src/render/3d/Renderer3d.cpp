#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void Renderer3d::begin(ICamera* camera) {
    this->camera = camera;
    if (this->camera != nullptr)
        frustum.set(camera->getViewProjectionMatrixInverse());

    lightQueue.clear();

    // debug data
    totIBO = 0;
    totFaces = 0;
}
void Renderer3d::end() {
    if (logData == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBOs: %d Faces: %d", totIBO, totFaces);
}

void Renderer3d::submitLight(Light* light) { light->bindLightInformation(lightQueue); }

void Renderer3d::submit(const RenderCommand& command) {

    // se não há frustrum adicionar tudo
    if (this->camera == nullptr) {
        commandQueue.push_back(command);
        return;
    }

    // se nao ha AABB adicionar tudo
    AABB* pAABB = command.renderable->getAABB();
    if (pAABB == nullptr) {
        commandQueue.push_back(command);
    } else {
        // adicione apenas o que esta no clip-space
        if (pAABB->visible(frustum) == true) {

            IndexBuffer* ibo = command.renderable->getIBO();
            if (ibo != nullptr) {
                totIBO++;
                totFaces += ibo->getCount();
            }

            commandQueue.push_back(command);
        }
    }
}

void Renderer3d::flush() {

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

                activeShader.setUniform("model", command.transform);
                activeShader.setUniform("projection", camera->getProjectionMatrix());
                activeShader.setUniform("view", camera->getViewMatrix());

                // // FIXME: preciso disto aqui ??
                // int shadows = 0;
                // activeShader->setUniform("shadows", shadows);

                // light bind
                for (const UniformVal& uniformLight : lightQueue)
                    uniformLight.setUniform(activeShader);

                // bind dos uniforms
                for (const UniformVal& uniformMat : command.uniforms)
                    uniformMat.setUniform(activeShader);

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