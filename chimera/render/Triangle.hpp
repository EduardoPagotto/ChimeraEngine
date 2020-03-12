
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/render/VertexData.hpp"

namespace Chimera {

enum class SIDE { CP_ONPLANE = 0, CP_FRONT, CP_BACK, CP_SPANNING };

class Triangle {
  public:
    Triangle(const VertexData& va, const VertexData& vb, const VertexData& vc);
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    Triangle(const Triangle& _cpy);

    inline unsigned getSerial() const { return serial; }
    glm::vec3 normal();
    void generateNormal();
    // void debugData();

    VertexData vertex[3];

  private:
    unsigned serial;
    static unsigned serialMaster;
};

SIDE classifyPoint(glm::vec3* pos, Triangle* plane);

SIDE classifyPoly(Triangle* plane, Triangle* poly, glm::vec3* distance);

bool intersect(const glm::vec3& linestart, const glm::vec3& lineend, const glm::vec3& vertex, const glm::vec3& normal,
               glm::vec3& intersection, float& percentage);

} // namespace Chimera
#endif
