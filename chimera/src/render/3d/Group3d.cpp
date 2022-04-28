#include "chimera/render/3d/Group3d.hpp"
#include "chimera/core/TransformationStack.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"

namespace Chimera {

void Group3d::submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) {

    renderer->getStack().push(transformationMatrix);

    for (auto renderable : renderables)
        renderable->submit(camera, command, renderer);

    renderer->getStack().pop();
}
} // namespace Chimera