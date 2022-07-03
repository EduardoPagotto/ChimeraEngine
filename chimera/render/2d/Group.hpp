#pragma once
#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"
#include "chimera/core/visible/RenderCommand.hpp"

namespace Chimera {

class Group : public IRenderable2D {
  public:
    Group(const glm::mat4& transform) : transformationMatrix(transform) {}
    virtual ~Group() {}
    inline void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
    virtual void submit(const RenderCommand& command, IRenderer2D& renderer) override;

  private:
    std::vector<IRenderable2D*> renderables;
    glm::mat4 transformationMatrix;
};
} // namespace Chimera
