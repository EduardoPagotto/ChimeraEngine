
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

struct PlanePoint {
    PlanePoint(Triangle* _point) : pointOnPlane(_point->vertex[0].position), normal(_point->normal()) {}
    glm::vec3 pointOnPlane;
    glm::vec3 normal;
};

SIDE classifyPoint(glm::vec3* pos, PlanePoint* plane);
SIDE classifyPoly(PlanePoint* plane, Triangle* poly, glm::vec3* distance);

bool intersect(glm::vec3* linestart, glm::vec3* lineend, PlanePoint* plane, glm::vec3* intersection, float* percentage);

} // namespace Chimera
#endif
