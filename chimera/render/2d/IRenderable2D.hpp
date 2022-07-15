#pragma once
#include "chimera/core/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class IRenderable2D {
  public:
    virtual ~IRenderable2D() {}
    virtual void submit(class IRenderer2D& renderer) = 0;
};
} // namespace Chimera
