#pragma once
#include "chimera/render/VertexData.hpp"
#include <list>

namespace Chimera {

#define TRI_PA 0
#define TRI_PB 1
#define TRI_PC 2

struct Triangle {
    Triangle(const Triangle& t) : normal(t.normal), splitter(t.splitter) {
        this->p[TRI_PA] = t.p[TRI_PA];
        this->p[TRI_PB] = t.p[TRI_PB];
        this->p[TRI_PC] = t.p[TRI_PC];
    }

    Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const glm::vec3& _normal, const bool& split)
        : normal(_normal), splitter(split) {
        this->p[TRI_PA] = _pa;
        this->p[TRI_PB] = _pb;
        this->p[TRI_PC] = _pc;
    }

    inline const glm::vec3& position(std::vector<VertexData>& vertexList, const unsigned& indexTriangle) const {
        return vertexList[this->p[indexTriangle]].position;
    }

    uint32_t p[3]; // TRI_PA = 0, TRI_PB = 1, TRI_PC = 3
    bool splitter;
    glm::vec3 normal;
};

void triangleFromVertexDataIndex(VertexData* vertexData, uint32_t* indexData, const uint32_t& indexSize, std::list<Triangle*>& vTris);
void triangleFromVertexData(VertexData* vertexData, const uint32_t& vertexSize, std::list<Triangle*>& vTris);
// bool tringleListIsConvex(std::vector<VertexData>& vertexList, std::vector<Triangle*>& _vTriangle);
} // namespace Chimera
