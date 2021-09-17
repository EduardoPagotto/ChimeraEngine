#include "chimera/render/vbs/SimpleRender3d.hpp"
//#include "chimera/core/buffers/VertexArray.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void SimpleRender3d::begin(glm::vec3* eye, Core::Frustum* frustrun, bool logData) {
    this->eye = eye;
    this->frustrun = frustrun;
    this->logData = logData;
}
void SimpleRender3d::end() {}

void SimpleRender3d::submit(IRenderable* renderable) { renderable->inject(eye, frustrun, this->logData, &renderQueue); }

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

            r->getIBO()->unbind();
        }

        // if (r->getVao() != nullptr)
        //     r->getVao()->unbind();

        renderQueue.pop_front();
    }
    pLastVao->unbind();
}

} // namespace Chimera