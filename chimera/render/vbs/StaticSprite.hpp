#ifndef __CHIMERA_STATIC_SPRITE__HPP
#define __CHIMERA_STATIC_SPRITE__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class StaticSprite : public Renderable2D {
  public:
    StaticSprite(float x, float y, float width, float height, glm::vec4 color, Shader* shader);
    virtual ~StaticSprite();
    Core::VertexArray* getVao() const { return vao; }
    Core::IndexBuffer* getIbo() const { return ibo; }
    Shader* getShader() const { return shader; }

  private:
    Core::VertexArray* vao;
    Core::IndexBuffer* ibo;
    Shader* shader;
};

} // namespace Chimera
#endif