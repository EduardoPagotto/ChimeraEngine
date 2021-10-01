#ifndef __CHIMERA_GROUP_ZZ__HPP
#define __CHIMERA_GROUP_ZZ__HPP

#include "../Renderable2D.hpp"

namespace Chimera {

class Group : public IRenderable2D {
  public:
    Group(const glm::mat4& transform) : transformationMatrix(transform) {}
    virtual ~Group() {}
    inline void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
    virtual void submit(class IRenderer2D* renderer) override;

  private:
    std::vector<IRenderable2D*> renderables;
    glm::mat4 transformationMatrix;
};
} // namespace Chimera
#endif