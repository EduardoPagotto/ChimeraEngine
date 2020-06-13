#include "chimera/render/Triangle.hpp"

namespace Chimera {

unsigned Triangle::serialMaster = 0;
u_int32_t TriangleIndex::master = 0;

Triangle::Triangle(const VertexData& va, const VertexData& vb, const VertexData& vc) : serial(serialMaster++) {
    this->vertex[0] = va;
    this->vertex[1] = vb;
    this->vertex[2] = vc;
}

Triangle::Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : serial(serialMaster++) {
    this->vertex[0].position = a;
    this->vertex[1].position = b;
    this->vertex[2].position = c;
}

Triangle::Triangle(const Triangle& _cpy) {
    serial = _cpy.serial;
    for (int i = 0; i < 3; i++) {
        this->vertex[i].position = _cpy.vertex[i].position;
        this->vertex[i].normal = _cpy.vertex[i].normal;
        this->vertex[i].texture = _cpy.vertex[i].texture;
    }
}

glm::vec3 Triangle::normal() const {
    glm::vec3 acc = glm::vec3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        acc = acc + this->vertex[i].normal;
    return glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
}

void Triangle::generateNormal() {
    glm::vec3 u = this->vertex[1].position - this->vertex[0].position;
    glm::vec3 v = this->vertex[2].position - this->vertex[0].position;
    glm::vec3 normal = glm::normalize(glm::cross(u, v));

    for (int i = 0; i < 3; i++)
        this->vertex[i].normal = normal;
}

glm::vec3 calcNormalIndexed(const TriangleIndex& _tris, const std::vector<VertexData>& _vVertexData) {
    glm::vec3 u = _vVertexData[_tris.p[1]].position - _vVertexData[_tris.p[0]].position;
    glm::vec3 v = _vVertexData[_tris.p[2]].position - _vVertexData[_tris.p[0]].position;
    return glm::normalize(glm::cross(u, v));
}

void overWriteNormalIndexed(const TriangleIndex& _tris, std::vector<VertexData>& _vVertexData) {

    glm::vec3 u = _vVertexData[_tris.p[1]].position - _vVertexData[_tris.p[0]].position;
    glm::vec3 v = _vVertexData[_tris.p[2]].position - _vVertexData[_tris.p[0]].position;
    glm::vec3 normal = glm::normalize(glm::cross(u, v));

    for (int i = 0; i < 3; i++)
        _vVertexData[_tris.p[i]].normal = normal;
}

TriangleIndex::TriangleIndex(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const std::vector<VertexData>& _vVertexData)
    : serial(master++) {

    p[0] = _pa;
    p[1] = _pb;
    p[2] = _pc;

    glm::vec3 acc = glm::vec3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        acc = acc + _vVertexData[p[i]].normal;

    this->normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);

    // this->normal = calcNormalIndexed(*this, _vVertexData);
}

// TriangleIndex::normal(std::vector<VertexData>& _vVertexData) {
//     glm::vec3 acc = glm::vec3(0, 0, 0);
//     for (int i = 0; i < 3; i++)
//         acc = acc + _vVertexData[_tris.p[i]].normal;

//     return glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
// }

} // namespace Chimera
