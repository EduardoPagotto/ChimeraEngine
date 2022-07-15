#pragma once
#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"

namespace Chimera {

class Group : public IRenderable2D {
  public:
    Group(const glm::mat4& transform) : transformationMatrix(transform) {}
    virtual ~Group() {}
    inline void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
    virtual void submit(IRenderer2D& renderer) override;
    virtual const Prop2D& getProp() override { return prop2d; }

  private:
    Prop2D prop2d;
    std::vector<IRenderable2D*> renderables;
    glm::mat4 transformationMatrix;
};
} // namespace Chimera
