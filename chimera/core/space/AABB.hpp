#pragma once
#include "Frustum.hpp"

namespace Chimera {

class AABB { // ref: http://www.3dcpptutorials.sk/index.php?id=59
  public:
    AABB() = default;
    AABB(const AABB& _cpy);
    AABB(const glm::vec3& _min, const glm::vec3& _max);
    virtual ~AABB() = default;

    inline const bool visible(const Frustum& _frustum) const { return _frustum.AABBVisible(vertex); }
    inline const float distance(const Frustum& _frustum) const { return _frustum.AABBDistance(vertex); }
    inline glm::vec3 getMax() const { return vertex[7]; }
    inline glm::vec3 getMin() const { return vertex[0]; }
    inline glm::vec3 getPosition() const { return position; }
    inline glm::vec3 getSize() const { return size; }

    void setPosition(const glm::vec3& _pos, const glm::vec3& _size);
    void setBoundary(const glm::vec3& _min, const glm::vec3& _max);
    bool intersects(const AABB& _aabb) const;
    bool contains(const glm::vec3& _point) const;
    AABB transformation(const glm::mat4& transformation) const;
    const glm::vec3& getVertex(int index) const { return vertex[index]; }
    const glm::vec3* getAllVertex() const { return vertex; }

  protected:
    glm::vec3 vertex[8];

  private:
    glm::vec3 position;
    glm::vec3 size;
};
} // namespace Chimera
