#pragma once
#include "IRenderable3d.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class Group3d : public IRenderable3d {
  public:
    Group3d(const glm::mat4& transform) : transformationMatrix(transform) {}
    virtual ~Group3d() {}
    inline void add(IRenderable3d* renderable) { renderables.push_back(renderable); }
    virtual void submit(class RenderCommand& command, class IRenderer3d* renderer) override;

  private:
    std::vector<IRenderable3d*> renderables;
    glm::mat4 transformationMatrix;
};
} // namespace Chimera
