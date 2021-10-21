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

void Renderer3d::begin(Camera* camera) {
    this->camera = camera;
    if (this->camera != nullptr)
        frustum.set(camera->getViewProjectionMatrixInverse());

    // debug data
    totIBO = 0;
    totFaces = 0;
}
void Renderer3d::end() {
    if (logData == true)
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBOs: %d Faces: %d", totIBO, totFaces);
}

void Renderer3d::submit(IRenderable3d* renderable) {

    // se não há frustrum adicionar tudo
    if (this->camera == nullptr) {
        renderQueue.push_back(renderable);
        return;
    }

    // se nao ha AABB adicionar tudo
    AABB* pAABB = renderable->getAABB();
    if (pAABB == nullptr) {
        renderQueue.push_back(renderable);
    } else {
        // adicione apenas o que esta no clip-space
        if (pAABB->visible(frustum) == true) {

            IndexBuffer* ibo = renderable->getIBO();
            if (ibo != nullptr) {
                totIBO++;
                totFaces += ibo->getCount();
            }

            renderQueue.push_back(renderable);
        }
    }
}

void Renderer3d::reloadShader(Entity entity) {

    activeShader->enable();
    activeShader->setUniform("projection", camera->getProjectionMatrix());
    // activeShader->setUniform("view", camera->getViewMatrix());

    if (entity.hasComponent<Light>()) {
        Light& light = entity.getComponent<Light>();
        light.bindLightInformation(activeShader);
    }

    // FIXME: preciso disto aqui ??
    int shadows = 0;
    activeShader->setUniform("shadows", shadows);
}

void Renderer3d::flush(bool useMaterial, Shader* alternativeShader) {

    VertexArray* pLastVao = nullptr;
    while (!renderQueue.empty()) {

        IRenderable3d* r = renderQueue.front();

        if (r->getVao() != nullptr) {      // Possui um novo modelo
            if (r->getVao() != pLastVao) { // Diferente  do anterior
                if (pLastVao != nullptr) { // desvincula o anterior
                    pLastVao->unbind();
                }

                r->getVao()->bind(); // vincula novo modelo
                pLastVao = r->getVao();

                Entity entity = r->getEntity();
                if (entity) {

                    Transform& model = entity.getComponent<Transform>();

                    if (activeShader == nullptr) { // primeira passada, sem shader algum habilitado aqui!!!!

                        if (alternativeShader != nullptr) { // shader passado tem prioridade
                            activeShader = alternativeShader;
                        } else {

                            if (entity.hasComponent<Shader>()) // sem shader prioritario pegar da entidade
                                activeShader = &entity.getComponent<Shader>();
                            else {
                                // TODO: finalizar erro!!!
                            }
                        }

                        this->reloadShader(entity);

                    } else { // demais passadas

                        if (alternativeShader != nullptr) { // alternative tem prioridade

                            if (alternativeShader != activeShader) {
                                activeShader->disable(); // demais passadas sempre tera activeShader nao e necessario testar
                                activeShader = alternativeShader;
                                this->reloadShader(entity);
                            }

                        } else {

                            Shader* newShader = nullptr;
                            if (entity.hasComponent<Shader>()) {
                                newShader = &entity.getComponent<Shader>();
                            } else {
                                // TODO: finalizar
                            }

                            if (newShader != activeShader) {
                                activeShader->disable();
                                activeShader = newShader;
                                this->reloadShader(entity);
                            }
                        }
                    }

                    activeShader->setUniform("model", model.getMatrix());
                    // activeShader->setUniform("projection", camera->getProjectionMatrix());
                    activeShader->setUniform("view", camera->getViewMatrix());

                    if (useMaterial) {
                        if (entity.hasComponent<Material>()) {
                            Material& material = entity.getComponent<Material>();
                            material.bindMaterialInformation(activeShader);
                        }
                    }

                } else {

                    if (activeShader == nullptr) { // primeira passada!!!!
                        activeShader = alternativeShader;
                        activeShader->enable();
                    } else {

                        if (alternativeShader != activeShader) {
                            activeShader->disable();
                            activeShader = alternativeShader;
                            activeShader->enable();
                        }
                    }
                }
            }
        }

        // Desenhar o IBO
        if (r->getIBO() != nullptr) {

            r->getIBO()->bind();

            // r->getShader()->setUniform1i("teste", 1);
            // glm::mat4 modelMatrix(1.0f);
            // glm::mat4 mm = glm::translate(modelMatrix, r->getPosition());
            // r->getShader()->setUniformMatrix4fv("ml_matrix", 1, false, glm::value_ptr(mm));

            glDrawElements(GL_TRIANGLES, r->getIBO()->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

            if (logData == true)
                r->debugDados();

            r->getIBO()->unbind();
        }
        renderQueue.pop_front();
    }
    pLastVao->unbind();
}

} // namespace Chimera