#include "chimera/render/partition/Triangle.hpp"

namespace Chimera {

// void triangleSetNormal(std::vector<VertexData>& vertexList, Triangle& t) {

//     glm::vec3 u = vertexList[t.p[TRI_PB]].position - vertexList[t.p[TRI_PA]].position;
//     glm::vec3 v = vertexList[t.p[TRI_PC]].position - vertexList[t.p[TRI_PA]].position;
//     t.normal = glm::normalize(glm::cross(u, v));
//     vertexList[t.p[TRI_PA]].normal = t.normal;
//     vertexList[t.p[TRI_PB]].normal = t.normal;
//     vertexList[t.p[TRI_PC]].normal = t.normal;
// }

void triangleFromVertexDataIndex(VertexData* vertexData, uint32_t* indexData, const uint32_t& indexSize, std::list<Triangle*>& vTris) {
    // Usa os indices já pre-calculado
    for (uint32_t indice = 0; indice < indexSize; indice += 3) {
        uint32_t pa = indexData[indice];
        uint32_t pb = indexData[indice + 1];
        uint32_t pc = indexData[indice + 2];
        // Calcula Normal Face
        glm::vec3 acc = vertexData[pa].normal + vertexData[pb].normal + vertexData[pc].normal;
        glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
        vTris.push_back(new Triangle(pa, pb, pc, normal, false));
    }
}

void triangleFromVertexData(VertexData* vertexData, const uint32_t& vertexSize, std::list<Triangle*>& vTris) {
    // Calcula os indices na sequencia em que os vertices estão
    for (uint32_t indice = 0; indice < vertexSize; indice += 3) {
        uint32_t pa = indice;
        uint32_t pb = indice + 1;
        uint32_t pc = indice + 2;
        // Calcula Normal Face
        glm::vec3 acc = vertexData[pa].normal + vertexData[pb].normal + vertexData[pc].normal;
        glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
        vTris.push_back(new Triangle(pa, pb, pc, normal, false));
    }
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

//             glm::vec3 u = th1->normal;
//             glm::vec3 v = th2->normal;
//             float val = glm::dot(u, v);
//             if (val > 0.0f) { // if not convex test if is coplanar
//                 glm::vec3 result;
//                 Plane alpha(th1->position(vVertex, 0), th1->normal);
//                 if (alpha.classifyPoly(th2->position(vVertex, 0), th2->position(vVertex, 1), th2->position(vVertex, 2), &result) !=
//                     SIDE::CP_ONPLANE)
//                     return false;

//                 // test if faces has oposites directions aka: convex
//                 if (alpha.collinearNormal(th1->normal) == false)
//                     return false;
//             }
//         }
//     }

//     return true;
// }

} // namespace Chimera
