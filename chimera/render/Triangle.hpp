
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/render/VertexData.hpp"
#include <vector>

namespace Chimera {

class Triangle {
  public:
    Triangle(const Triangle& _val);
    Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const std::vector<VertexData>& _vVertexData);
    inline glm::vec3 getNormal() const { return normal; }
    inline uint32_t getSerial() const { return serial; }
    uint32_t p[3]; // PA = 0, PB = 1, PC = 3
  private:
    glm::vec3 normal;
    uint32_t serial;
    static uint32_t master;
};

glm::vec3 calcNormalIndexed(const Triangle& _tris, const std::vector<VertexData>& _vVertexData);
void overWriteNormalIndexed(const Triangle& _tris, std::vector<VertexData>& _vVertexData);

// class Triangle {
//   public:
//     Triangle(const VertexData& va, const VertexData& vb, const VertexData& vc);
//     Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
//     Triangle(const Triangle& _cpy);

//     inline unsigned getSerial() const { return serial; }
//     glm::vec3 normal() const;
//     void generateNormal();

//     VertexData vertex[3];

//   private:
//     unsigned serial;
//     static unsigned master;
// };

} // namespace Chimera
#endif
