#include "chimera/render/Triangle.hpp"

namespace Chimera {

Triangle::Triangle(const Triangle& _val) {
    this->p[0] = _val.p[0];
    this->p[1] = _val.p[1];
    this->p[2] = _val.p[2];
    this->normal = _val.normal;
    this->beenUsedAsSplitter = _val.beenUsedAsSplitter;
}

Triangle::Triangle(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc, const glm::vec3& _normal) {
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

// bool tringleListIsConvex(std::vector<VertexData>& vertexList, std::vector<Triangle*>& _vTriangle) {

//     if (_vTriangle.size() <= 1)
//         return false;

//     Triangle* th1 = nullptr;
//     Triangle* th2 = nullptr;
//     float sameDir = 0.0;

//     for (std::vector<Triangle*>::iterator i = _vTriangle.begin(); i != _vTriangle.end(); i++) {

//         th1 = (*i);

//         for (std::vector<Triangle*>::iterator j = i; j != _vTriangle.end(); j++) {

//             if (i == j)
//                 continue;

//             th2 = (*j);

//             glm::vec3 u = th1->getNormal();
//             glm::vec3 v = th2->getNormal();
//             float val = glm::dot(u, v);
//             if (val > 0.0f) { // if not convex test if is coplanar
//                 glm::vec3 result;
//                 Plane alpha(th1->position(vVertex, 0), th1->getNormal());
//                 if (alpha.classifyPoly(th2->position(vVertex, 0), th2->position(vVertex, 1), th2->position(vVertex, 2), &result) !=
//                     SIDE::CP_ONPLANE)
//                     return false;

//                 // test if faces has oposites directions aka: convex
//                 if (alpha.collinearNormal(th1->getNormal()) == false)
//                     return false;
//             }
//         }
//     }

//     return true;
// }

} // namespace Chimera
