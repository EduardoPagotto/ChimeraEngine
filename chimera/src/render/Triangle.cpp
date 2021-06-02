#include "chimera/render/Triangle.hpp"

namespace Chimera {

u_int32_t Triangle::master = 0;

Triangle::Triangle(const Triangle& _val) : serial(master++) {
    this->p[0] = _val.p[0];
    this->p[1] = _val.p[1];
    this->p[2] = _val.p[2];
    this->normal = _val.normal;
    this->beenUsedAsSplitter = _val.beenUsedAsSplitter;
}

Triangle::Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const glm::vec3& _normal) : serial(master++) {
    this->p[0] = _pa;
    this->p[1] = _pb;
    this->p[2] = _pc;
    this->normal = _normal;
    this->beenUsedAsSplitter = false;
}

void Triangle::calcNormal(std::vector<VertexData>& vertexList) {

    glm::vec3 u = vertexList[this->p[1]].position - vertexList[this->p[0]].position;
    glm::vec3 v = vertexList[this->p[2]].position - vertexList[this->p[0]].position;
    this->normal = glm::normalize(glm::cross(u, v));

    vertexList[this->p[0]].normal = this->normal;
    vertexList[this->p[1]].normal = this->normal;
    vertexList[this->p[2]].normal = this->normal;
}

} // namespace Chimera
