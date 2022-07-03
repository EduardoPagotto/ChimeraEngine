#include "chimera/render/2d/Group.hpp"

namespace Chimera {

void Group::submit(const RenderCommand& command, IRenderer2D& renderer) {

    renderer.getStack().push(transformationMatrix);
    for (auto renderable : renderables)
        renderable->submit(command, renderer);

    renderer.getStack().pop();
}
} // namespace Chimera