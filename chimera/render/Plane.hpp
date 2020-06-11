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
    glm::vec3 point;  // vertice A
    glm::vec3 normal; // plane calc cross product B and C across A
    float ND;
    int O;
};

} // namespace Chimera
#endif