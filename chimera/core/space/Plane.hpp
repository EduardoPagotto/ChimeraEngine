#pragma once
#include "chimera/core/OpenGLDefs.hpp"

namespace Chimera {

enum class SIDE { CP_ONPLANE, CP_FRONT, CP_BACK, CP_SPANNING };

class Plane {
  public:
    Plane() = default;
    Plane(const Plane& _cpy);
    Plane(const glm::vec3& _position, const glm::vec3& _normal) { this->set(_position, _normal); }
    virtual ~Plane() = default;

    void set(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
    void set(const glm::vec3& _position, const glm::vec3& _normal);

    inline glm::vec3 getPoint() const { return this->point; }
    inline glm::vec3 getNormal() const { return this->normal; }

    bool collinearNormal(const glm::vec3& _normal) const;
    SIDE classifyPoint(const glm::vec3& point) const;
    SIDE classifyPoly(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC, glm::vec3* clipTest) const;
    bool intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection, float& percentage) const;

    inline const bool AABBBehind(const glm::vec3* AABBVertices) const { return glm::dot(normal, AABBVertices[O]) < ND; }
    inline const float AABBDistance(const glm::vec3* AABBVertices) const { return glm::dot(normal, AABBVertices[O]); }

  private:
    glm::vec3 point;  // vertice A
    glm::vec3 normal; // plane calc cross product B and C across A
    float ND;
    int O;
};
} // namespace Chimera
