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

    void applyTransformation(const glm::mat4& transformation);

    inline glm::vec3 getMax() const { return vertices[7]; }
    inline glm::vec3 getMin() const { return vertices[0]; }
    inline glm::vec3 getCenter() const { return center; }
    inline glm::vec3 getSize() const { return size; }

  private:
    glm::vec3 vertices[8];
    glm::vec3 center;
    glm::vec3 size;
};
} // namespace Chimera

#endif