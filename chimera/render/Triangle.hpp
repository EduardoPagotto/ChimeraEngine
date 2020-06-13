
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/render/VertexData.hpp"

namespace Chimera {

enum class SIDE { CP_ONPLANE = 0, CP_FRONT, CP_BACK, CP_SPANNING };

// struct TriangleIndex {
//     uint32 a, b, c;
// };

class Triangle {
  public:
    Triangle(const VertexData& va, const VertexData& vb, const VertexData& vc);
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    Triangle(const Triangle& _cpy);

    inline unsigned getSerial() const { return serial; }
    glm::vec3 normal() const;
    void generateNormal();

    VertexData vertex[3];

  private:
    unsigned serial;
    static unsigned serialMaster;
};

class PlanePoint {
  public:
    PlanePoint() {}
    void set(const glm::vec3& _position, const glm::vec3& _normal);
    SIDE classifyPoint(glm::vec3* point);
    SIDE classifyPoly(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC, glm::vec3* clipTest);
    bool intersect(glm::vec3* linestart, glm::vec3* lineend, glm::vec3* intersection, float* percentage);

  private:
    glm::vec3 point;
    glm::vec3 normal;
};

} // namespace Chimera
#endif
