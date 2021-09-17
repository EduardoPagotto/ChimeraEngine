
#ifndef __CHIMERA_TRIANGLE__HPP
#define __CHIMERA_TRIANGLE__HPP

#include "chimera/core/VertexData.hpp"
#include <glm/glm.hpp>
#include <list>

namespace Chimera::Core {

class Triangle {
  public:
    Triangle(const Triangle& _val);
    Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const glm::vec3& _normal);
    inline glm::vec3 getNormal() const { return normal; }

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
};

void triangleFromVertexDataIndex(VertexData* vertexData, uint32_t* indexData, const uint32_t& indexSize, std::list<Triangle*>& vTris);
void triangleFromVertexData(VertexData* vertexData, const uint32_t& vertexSize, std::list<Triangle*>& vTris);
// bool tringleListIsConvex(std::vector<VertexData>& vertexList, std::vector<Triangle*>& _vTriangle);
} // namespace Chimera::Core
#endif
