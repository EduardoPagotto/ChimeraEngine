
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
    glm::vec3 normal() const;
    void generateNormal();
    // void debugData();

    VertexData vertex[3];

  private:
    unsigned serial;
    static unsigned serialMaster;
};

class PlanePoint {
  public:
    PlanePoint(Triangle* _point) : point(_point->vertex[0].position), normal(_point->normal()) {}

    SIDE classifyPoint(glm::vec3* point);
    SIDE classifyPoly(Triangle* poly, glm::vec3* distance);
    bool intersect(glm::vec3* linestart, glm::vec3* lineend, glm::vec3* intersection, float* percentage);

  private:
    glm::vec3 point;
    glm::vec3 normal;
};

} // namespace Chimera
#endif
