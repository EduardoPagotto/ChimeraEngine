#ifndef __CHIMERA_PLANE__HPP
#define __CHIMERA_PLANE__HPP

#include "chimera/core/VertexData.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class Plane {
  public:
    Plane();
    Plane(const Plane& _cpy);
    Plane(const glm::vec3& _position, const glm::vec3& _normal);
    ~Plane();

    void set(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
    void set(const glm::vec3& _position, const glm::vec3& _normal);

    inline glm::vec3 getPoint() const { return this->point; }
    inline glm::vec3 getNormal() const { return this->normal; }

    bool collinearNormal(const glm::vec3& _normal);
    SIDE classifyPoint(glm::vec3* point);
    SIDE classifyPoly(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC, glm::vec3* clipTest);
    bool intersect(glm::vec3* linestart, glm::vec3* lineend, glm::vec3* intersection, float* percentage);

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