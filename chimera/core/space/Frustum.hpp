#pragma once
#include "Plane.hpp"

namespace Chimera {

class Frustum {
  public:
    Frustum() = default;
    virtual ~Frustum() = default;

    void set(const glm::mat4& ViewProjectionMatrixInverse);
    const bool AABBVisible(const glm::vec3* AABBVertices) const;
    inline const float AABBDistance(const glm::vec3* AABBVertices) const { return planes[5].AABBDistance(AABBVertices); }
    void render_debug() const;

  private:
    glm::vec3 vertices[8];
    Plane planes[6];
};
} // namespace Chimera
