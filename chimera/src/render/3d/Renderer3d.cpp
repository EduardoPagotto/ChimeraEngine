#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void Renderer3d::begin(Camera* camera, Shader* shader) {
    this->camera = camera;
    if (this->camera != nullptr) {

        shader->setUniform("projection", camera->getProjectionMatrix());
        shader->setUniform("view", camera->getViewMatrix());

        frustum.set(camera->getViewProjectionMatrixInverse());
    }

    this->shader = shader;

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

void Renderer3d::flush(bool useMaterial) {

    VertexArray* pLastVao = nullptr;
    while (!renderQueue.empty()) {

        IRenderable3d* r = renderQueue.front();

        if (r->getVao() != nullptr) {
            if (r->getVao() != pLastVao) {
                if (pLastVao != nullptr) {
                    pLastVao->unbind();
                }

                // novo renderable bind material
                r->getVao()->bind();
                pLastVao = r->getVao();

                if (shader) {
                    Entity entity = r->getEntity();
                    if (entity) {

                        Transform& model = entity.getComponent<Transform>();

                        shader->setUniform("model", model.getMatrix());

                        if (useMaterial) {

                            if (entity.hasComponent<Material>()) {
                                Material& material = entity.getComponent<Material>();

                                material.bindMaterialInformation(shader);
                            }
                        }
                    }
                }
            }
        }

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