#include "chimera/render/vbs/SimpleRender3d.hpp"
#include "chimera/render/vbs/IRenderable.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void SimpleRender3d::begin(glm::vec3* eye, Core::Frustum* frustrun, bool logData) {
    this->eye = eye;
    this->frustrun = frustrun;
    this->logData = logData;

    // debug data
    totIBO = 0;
    totFaces = 0;
}
void SimpleRender3d::end() {
    // if (logData == true) //FIXME: tirar quando testar todos
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "IBOs: %d Faces: %d", totIBO, totFaces);
}

void SimpleRender3d::submit(IRenderable* renderable) {

    Core::AABB* pAABB = renderable->getAABB();
    if (pAABB == nullptr) {
        renderQueue.push_back(renderable);
    } else {
        if (pAABB->visible(*frustrun) == true) {

            Core::IndexBuffer* ibo = renderable->getIBO();
            if (ibo != nullptr) {
                totIBO++;
                totFaces += ibo->getCount();
            }

            renderQueue.push_back(renderable);
        }
    }
}

void SimpleRender3d::flush() {

    Core::VertexArray* pLastVao = nullptr;
    while (!renderQueue.empty()) {

        IRenderable* r = renderQueue.front();

        if (r->getVao() != nullptr) {
            if (r->getVao() != pLastVao) {
                if (pLastVao != nullptr) {
                    pLastVao->unbind();
                }
                r->getVao()->bind();
                pLastVao = r->getVao();
            }
        }

        if (r->getIBO() != nullptr) {

            r->getIBO()->bind();

            // r->getShader()->setUniform1i("teste", 1);
            glDrawElements(GL_TRIANGLES, r->getIBO()->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

            // if (logData == true) //FIXME: tirar quando testar todos
            r->debugDados();

            r->getIBO()->unbind();
        }
        renderQueue.pop_front();
    }
    pLastVao->unbind();
}

} // namespace Chimera