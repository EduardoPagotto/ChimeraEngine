#include "chimera/render/vbs/Group.hpp"

namespace Chimera {

Group::Group(const glm::mat4& transform) : transformationMatrix(transform) {}

Group::~Group() {}

void Group::submit(IRenderer2D* renderer) {

    renderer->getStack().push(transformationMatrix);

    for (auto renderable : renderables) {
        renderable->submit(renderer);
    }

    renderer->getStack().pop();
}

void Group::add(IRenderable2D* renderable) { renderables.push_back(renderable); }

} // namespace Chimera