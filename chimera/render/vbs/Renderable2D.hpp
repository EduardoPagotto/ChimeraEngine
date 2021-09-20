#ifndef __CHIMERA_RENDERABLE_2D__HPP
#define __CHIMERA_RENDERABLE_2D__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include <deque>

namespace Chimera {

class IRenderable2D {
  public:
    virtual glm::vec3 getPosition() const = 0;
    virtual glm::vec2 getSize() const = 0;
    virtual glm::vec4 getColor() const = 0;
};

class Renderable2D : public IRenderable2D {
  public:
    Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
    virtual ~Renderable2D();

    inline glm::vec3 getPosition() const { return pos; }
    inline glm::vec2 getSize() const { return size; }
    inline glm::vec4 getColor() const { return color; }

  private:
    glm::vec3 pos;
    glm::vec2 size;
    glm::vec4 color;
};

class IRenderer2D {
  public:
    virtual void submit(IRenderable2D* renderable) = 0;
    virtual void flush() = 0;
};
} // namespace Chimera
#endif