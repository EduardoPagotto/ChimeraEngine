#ifndef __CHIMERA_PLANE__HPP
#define __CHIMERA_PLANE__HPP

#include <glm/glm.hpp>

namespace Chimera {

class Plane {
  public:
    Plane();
    ~Plane();

    void set(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
    bool AABBBehind(const glm::vec3* AABBVertices);
    float AABBDistance(const glm::vec3* AABBVertices);

  private:
    glm::vec3 N;
    float ND;
    int O;
};

} // namespace Chimera
#endif