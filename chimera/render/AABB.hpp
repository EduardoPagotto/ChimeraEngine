#ifndef __CHIMERA_AABB__HPP
#define __CHIMERA_AABB__HPP

#include "Frustum.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class AABB {
  public:
    AABB();
    AABB(const AABB& _cpy);
    AABB(const glm::vec3& _min, const glm::vec3& _max);
    virtual ~AABB();

    void set(const glm::vec3& _min, const glm::vec3& _max);
    bool pointInside(const glm::vec3& _point);
    bool visible(Frustum& _frustum);
    float distance(Frustum& _frustum);
    void render();

    AABB transformation(const glm::mat4& transformation);

    inline glm::vec3 getMax() const { return vertex[7]; }
    inline glm::vec3 getMin() const { return vertex[0]; }
    inline glm::vec3 getCenter() const { return center; }
    inline glm::vec3 getSize() const { return size; }

  private:
    glm::vec3 vertex[8];
    glm::vec3 center;
    glm::vec3 size;
};
} // namespace Chimera

#endif