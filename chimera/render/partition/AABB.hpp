#pragma once

#include "Frustum.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class AABB {
  public:
    AABB();
    AABB(const AABB& _cpy);
    AABB(const glm::vec3& _min, const glm::vec3& _max);
    virtual ~AABB();

    void setPosition(const glm::vec3& _pos, const glm::vec3& _size);
    void setBoundary(const glm::vec3& _min, const glm::vec3& _max);

    bool intersects(const AABB& _aabb) const;
    bool contains(const glm::vec3& _point) const;
    bool visible(const Frustum& _frustum) const;
    float distance(const Frustum& _frustum) const;
    void render() const;

    AABB transformation(const glm::mat4& transformation) const;

    inline glm::vec3 getMax() const { return vertex[7]; }
    inline glm::vec3 getMin() const { return vertex[0]; }
    inline glm::vec3 getPosition() const { return position; }
    inline glm::vec3 getSize() const { return size; }

  protected:
    glm::vec3 vertex[8];

  private:
    glm::vec3 position;
    glm::vec3 size;
};
} // namespace Chimera
