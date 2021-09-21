#ifndef __CHIMERA_GROUP_ZZ__HPP
#define __CHIMERA_GROUP_ZZ__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class Group : public IRenderable2D {
  public:
    Group(const glm::mat4& transform);
    virtual ~Group();
    virtual void submit(class IRenderer2D* renderer) override;
    void add(IRenderable2D* renderable);

    inline glm::vec3 getPosition() const override { return glm::vec3(0.0f); }
    inline glm::vec2 getSize() const override { return glm::vec2(0.0f); }
    inline glm::vec4 getColor() const override { return glm::vec4(0.0f); }

  private:
    std::vector<IRenderable2D*> renderables;
    glm::mat4 transformationMatrix;
};
} // namespace Chimera
#endif