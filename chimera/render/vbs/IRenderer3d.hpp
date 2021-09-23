#ifndef __CHIMERA_IRENDERER_3D__HPP
#define __CHIMERA_IRENDERER_3D__HPP

#include "chimera/core/space/Frustum.hpp"
#include <deque>
#include <glm/glm.hpp>

namespace Chimera {

class IRenderer3d {
  public:
    virtual void begin(glm::vec3* eye, Core::Frustum* frustrun, bool logData) = 0;
    virtual void end() = 0;
    virtual void submit(class IRenderable* renderable) = 0;
    virtual void flush() = 0;
};

} // namespace Chimera
#endif