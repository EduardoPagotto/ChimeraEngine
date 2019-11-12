#ifndef __CHIMERA_AABB__HPP
#define __CHIMERA_AABB__HPP

#include "Frustum.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class AABB {
  public:
    AABB();
    ~AABB();
    void set(const glm::vec3& _min, const glm::vec3& _max);
    bool pointInside(const glm::vec3& _point);
    bool visible(Frustum& _frustum);
    float distance(Frustum& _frustum);
    void render();

  private:
    glm::vec3 vertices[8];
};
} // namespace Chimera

#endif