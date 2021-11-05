#pragma once

#include "Plane.hpp"

namespace Chimera {

class Frustum {
  public:
    Frustum();
    ~Frustum();

    void set(const glm::mat4& ViewProjectionMatrixInverse);
    bool AABBVisible(const glm::vec3* AABBVertices) const;
    float AABBDistance(const glm::vec3* AABBVertices) const;
    void render() const;

  private:
    glm::vec3 vertices[8];
    Plane planes[6];
};
} // namespace Chimera
