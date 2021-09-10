
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/core/VertexData.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class Triangle {
  public:
    Triangle(const Triangle& _val);
    Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const glm::vec3& _normal);
    inline glm::vec3 getNormal() const { return normal; }
    inline uint32_t getSerial() const { return serial; }

    void calcNormal(std::vector<VertexData>& vertexList);

    inline glm::vec3 position(std::vector<VertexData>& vertexList, const unsigned& indexTriangle) const {
        return vertexList[this->p[indexTriangle]].position;
    }
    inline VertexData vertex(std::vector<VertexData>& vertexList, const unsigned& indexTriangle) const {
        return vertexList[this->p[indexTriangle]];
    }

    uint32_t p[3]; // PA = 0, PB = 1, PC = 3
    bool beenUsedAsSplitter;

  private:
    glm::vec3 normal;

  private:
    uint32_t serial;
    static uint32_t master;
};

// bool tringleListIsConvex(std::vector<VertexData>& vertexList, std::vector<Triangle*>& _vTriangle);

} // namespace Chimera
#endif
