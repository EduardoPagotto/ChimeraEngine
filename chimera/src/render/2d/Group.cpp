#include "chimera/render/2d/Group.hpp"

namespace Chimera {

void Group::submit(IRenderer2D& renderer) {

    renderer.getStack().push(transformationMatrix);
    for (auto renderable : renderables)
        renderable->submit(renderer);

    renderer.getStack().pop();
}
} // namespace Chimera