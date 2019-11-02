#ifndef __GAME_TERRAIN_FRUSTUM__HPP
#define __GAME_TERRAIN_FRUSTUM__HPP

#include "Plane.hpp"

namespace Chimera {

class Frustum {
  public:
    Frustum();
    ~Frustum();

    void set(const glm::mat4& ViewProjectionMatrixInverse);
    bool AABBVisible(const glm::vec3* AABBVertices);
    float AABBDistance(const glm::vec3* AABBVertices);
    void render();

  private:
    glm::vec3 vertices[8];
    Plane planes[6];
};

} // namespace Chimera

#endif