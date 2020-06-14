#include "chimera/render/Triangle.hpp"

namespace Chimera {

// unsigned Triangle::master = 0;
u_int32_t Triangle::master = 0;

// Triangle::Triangle(const VertexData& va, const VertexData& vb, const VertexData& vc) : serial(master++) {
//     this->vertex[0] = va;
//     this->vertex[1] = vb;
//     this->vertex[2] = vc;
// }

// Triangle::Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : serial(master++) {
//     this->vertex[0].position = a;
//     this->vertex[1].position = b;
//     this->vertex[2].position = c;
// }

// Triangle::Triangle(const Triangle& _cpy) {
//     serial = _cpy.serial;
//     for (int i = 0; i < 3; i++) {
//         this->vertex[i].position = _cpy.vertex[i].position;
//         this->vertex[i].normal = _cpy.vertex[i].normal;
//         this->vertex[i].texture = _cpy.vertex[i].texture;
//     }
// }

// glm::vec3 Triangle::normal() const {
//     glm::vec3 acc = glm::vec3(0, 0, 0);
//     for (int i = 0; i < 3; i++)
//         acc = acc + this->vertex[i].normal;
//     return glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
// }

// void Triangle::generateNormal() {
//     glm::vec3 u = this->vertex[1].position - this->vertex[0].position;
//     glm::vec3 v = this->vertex[2].position - this->vertex[0].position;
//     glm::vec3 normal = glm::normalize(glm::cross(u, v));

//     for (int i = 0; i < 3; i++)
//         this->vertex[i].normal = normal;
// }

// glm::vec3 calcNormalIndexed(const Triangle& _tris, const std::vector<VertexData>& _vVertexData) {
//     glm::vec3 u = _vVertexData[_tris.p[1]].position - _vVertexData[_tris.p[0]].position;
//     glm::vec3 v = _vVertexData[_tris.p[2]].position - _vVertexData[_tris.p[0]].position;
//     return glm::normalize(glm::cross(u, v));
// }

// void overWriteNormalIndexed(const Triangle& _tris, std::vector<VertexData>& _vVertexData) {

//     glm::vec3 u = _vVertexData[_tris.p[1]].position - _vVertexData[_tris.p[0]].position;
//     glm::vec3 v = _vVertexData[_tris.p[2]].position - _vVertexData[_tris.p[0]].position;
//     glm::vec3 normal = glm::normalize(glm::cross(u, v));

//     for (int i = 0; i < 3; i++)
//         _vVertexData[_tris.p[i]].normal = normal;
// }

Triangle::Triangle(const Triangle& _val) : serial(master++) {
    this->p[0] = _val.p[0];
    this->p[1] = _val.p[1];
    this->p[2] = _val.p[2];
    this->normal = _val.normal;
}

Triangle::Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const std::vector<VertexData>& _vVertexData)
    : serial(master++) {

    this->p[0] = _pa;
    this->p[1] = _pb;
    this->p[2] = _pc;

    glm::vec3 acc = _vVertexData[_pa].normal + _vVertexData[_pb].normal + _vVertexData[_pc].normal;
    this->normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);

    // // Calc Automatico sobrescreve original
    // glm::vec3 u = _vVertexData[_pb].position - _vVertexData[_pa].position;
    // glm::vec3 v = _vVertexData[_pc].position - _vVertexData[_pa].position;
    // this->normal = glm::normalize(glm::cross(u, v));
}
} // namespace Chimera
