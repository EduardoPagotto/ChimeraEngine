#ifndef __CHIMERA_RENDERABLE_2D__HPP
#define __CHIMERA_RENDERABLE_2D__HPP

#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"
#include "chimera/render/Texture.hpp"
#include <deque>

namespace Chimera {

class Renderable2D : public IRenderable2D {
  public:
    Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
    Renderable2D(glm::vec3 pos, glm::vec2 size, Texture* texture);
    virtual ~Renderable2D();
    virtual void submit(IRenderer2D* renderer) override { renderer->submit(this); }
    inline const glm::vec3 getPosition() const override { return pos; }
    inline const glm::vec2 getSize() const override { return size; }
    inline const glm::vec4 getColor() const override { return color; }
    inline const std::vector<glm::vec2>& getUV() const override { return uv; } // FIXME: passar para os e cima e na classe pai
    inline const GLuint getTID() const override { return texture ? texture->getID() : 0; }

  protected:
    glm::vec3 pos;
    glm::vec2 size;
    glm::vec4 color;
    std::vector<glm::vec2> uv;
    Texture* texture;
};
} // namespace Chimera
#endif