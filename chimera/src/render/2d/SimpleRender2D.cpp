#include "chimera/render/2d/SimpleRender2D.hpp"
#include "chimera/render/2d/StaticSprite.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

void SimpleRender2D::submit(IRenderable2D* renderable) { renderQueue.push_back((StaticSprite*)renderable); }

void SimpleRender2D::flush() {
    while (!renderQueue.empty()) {
        StaticSprite* r = (StaticSprite*)renderQueue.front();

        r->getVao()->bind();
        r->getIbo()->bind();

        glm::mat4 modelMatrix(1.0f);
        glm::mat4 mm = glm::translate(modelMatrix, r->getPosition());
        r->getShader()->setUniformMatrix4fv("ml_matrix", 1, false, glm::value_ptr(mm));

        glDrawElements(GL_TRIANGLES, r->getIbo()->getCount(), GL_UNSIGNED_INT, 0);

        r->getIbo()->unbind();
        r->getVao()->unbind();

        renderQueue.pop_front();
    }
}
} // namespace Chimera