
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/core/Vertex.hpp"

namespace Chimera {

class Triangle {
  public:
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    Triangle(const Triangle& _cpy);

    inline unsigned getSerial() const { return serial; }
    glm::vec3 normal();
    // glm::vec3 verticeNormal();
    // void debugData();

    VertexData vertex[3];

  private:
    unsigned serial;
    static unsigned serialMaster;
};
} // namespace Chimera
#endif
