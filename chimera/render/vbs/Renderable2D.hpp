#ifndef __CHIMERA_RENDERABLE_2D__HPP
#define __CHIMERA_RENDERABLE_2D__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera {

class IRender2D {
  public:
    virtual void submit(IRenderable2D* renderable) = 0;
    virtual void flush() = 0;
};

class IRenderable2D {
  public:
    virtual uint32_t getSize() const = 0;
    virtual Shader* getShader() const = 0;
    virtual Core::VertexArray* getVao() const = 0;
    virtual Core::IndexBuffer* getIBO() const = 0;
};

class Renderable2D {
  protected:
    Renderable2D(glm::vec3 pos, glm::vec3 size, glm::vec4 color);
    virtual ~Renderable2D();

    inline const glm::vec3 getPosition() const { return pos; }
    inline const glm::vec2 getSize() const { return size; }
    inline const glm::vec4 getColor() const { return color; }

  private:
    glm::vec3 pos;
    glm::vec2 size;
    glm::vec4 color;
    Core::VertexArray* vao;
    Core::IndexBuffer* ibo;
};
} // namespace Chimera
#endif