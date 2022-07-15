#pragma once
#include "chimera/core/OpenGLDefs.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

struct Prop2D {
    Prop2D() = default;
    glm::vec3 position = glm::vec3(0.0);
    glm::vec2 size = glm::vec2(1.0);
    glm::vec4 color = glm::vec4(1.0);
    class Texture* texture = nullptr;
    std::vector<glm::vec2> uv;
};

class IRenderable2D {
  public:
    virtual ~IRenderable2D() {}
    virtual void submit(class IRenderer2D& renderer) = 0;
    virtual const Prop2D& getProp() = 0;
};
} // namespace Chimera
