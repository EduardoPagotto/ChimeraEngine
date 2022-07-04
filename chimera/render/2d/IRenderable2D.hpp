#pragma once
#include "chimera/core/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class IRenderable2D {
  public:
    virtual ~IRenderable2D() {}
    virtual void submit(class IRenderer2D& renderer) = 0;
    virtual const glm::vec3 getPosition() const = 0;
    virtual const glm::vec2 getSize() const = 0;
    virtual const glm::vec4 getColor() const = 0;
    virtual const std::vector<glm::vec2>& getUV() const = 0;
    virtual class Texture* getTexture() const = 0;
};
} // namespace Chimera
