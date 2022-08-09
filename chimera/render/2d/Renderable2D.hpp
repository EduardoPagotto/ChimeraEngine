#pragma once
#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"
#include "chimera/core/visible/Texture.hpp"
#include "chimera/render/2d/Prop2D.hpp"

namespace Chimera {

class Renderable2D : public IRenderable2D {
  public:
    Renderable2D(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
    Renderable2D(const glm::vec3& pos, const glm::vec2& size, Texture* texture);
    virtual ~Renderable2D() {}
    virtual void submit(IRenderer2D& renderer) override { renderer.submit(this); }
    static const std::vector<glm::vec2>& getDefaultUVs();
    const Prop2D& getProp() const { return prop2d; }

  protected:
    Prop2D prop2d;
};
} // namespace Chimera
