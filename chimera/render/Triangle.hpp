
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/render/VertexData.hpp"

namespace Chimera {

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

} // namespace Chimera
#endif
