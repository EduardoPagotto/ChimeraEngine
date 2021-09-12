#include "chimera/render/vbs/SimpleRender3d.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void SimpleRender3d::submit(IRenderable* renderable) { this->renderQueue.push_back(renderable); }

void SimpleRender3d::flush() {

    while (!renderQueue.empty()) {

        IRenderable* r = renderQueue.front();
        r->getVao()->bind();
        r->getIBO()->bind();

        // r->getShader()->setUniform1i("teste", 1);
        glDrawElements(GL_TRIANGLES, r->getIBO()->getCount(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        r->getIBO()->unbind();
        r->getVao()->unbind();

        renderQueue.pop_front();
    }
}

} // namespace Chimera