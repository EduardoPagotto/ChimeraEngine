#include "chimera/render/VertexData.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/space/Plane.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

bool getSimilarVertexIndex(VertexData& in_vertex, std::vector<VertexData>& out_vertex, uint32_t& result) {
    // Percorrer todos os vertex ja existentes na lista
    for (uint32_t i = 0; i < out_vertex.size(); i++) {
        if (is_nearVec3(in_vertex.point, out_vertex[i].point) && is_nearVec3(in_vertex.normal, out_vertex[i].normal) &&
            is_nearVec2(in_vertex.uv, out_vertex[i].uv)) {
            result = i;
            return true;
        }
    }
    return false;
}

// void vertexDataReorder(std::vector<VertexData>& inVertexData, std::vector<uint32_t>& inIndex, std::vector<VertexData>& outVertexData,
//                        std::vector<uint32_t>& outIndex) {

//     std::vector<VertexData> tempVertexData;

//     SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Reorder Vertex In: ( %04lu / %04lu )", inVertexData.size(), inIndex.size());
//     for (uint32_t i : inIndex) {
//         // expand
//         VertexData vd = inVertexData[i];
//         tempVertexData.push_back(vd);
//     }

//     SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Reorder Vertex full: %04lu", tempVertexData.size());

//     vertexDataIndexCompile(tempVertexData, outVertexData, outIndex);
// }

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<uint32_t>& out_indices) {

    // percorrer todos os vertices
    for (uint32_t i = 0; i < inData.size(); i++) {
        // Procura por similar
        uint32_t index;
        if (getSimilarVertexIndex(inData[i], outData, index) == true) {
            // se entrotar usar apenas o indice
            out_indices.push_back(index);

        } else {
            // se nao adiciona a lista de novo vertex
            outData.push_back(inData[i]);
            out_indices.push_back((uint32_t)outData.size() - 1);
        }
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Compile Vertex In: %04lu Vertex out: %04lu Index out: %04lu ", inData.size(), outData.size(),
                 out_indices.size());
}

void vertexDataMinMaxSize(VertexData* pVertexList, const uint32_t& vertexSize, glm::vec3& min, glm::vec3& max, glm::vec3& size) {
    if (vertexSize > 0) {
        min = pVertexList[0].point;
        max = pVertexList[0].point;
    }

    for (uint32_t i = 1; i < vertexSize; i++) {
        min = glm::min(min, pVertexList[i].point);
        max = glm::max(max, pVertexList[i].point);
    }

    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void vertexDataIndexMinMaxSize(VertexData* pVertexList, const uint32_t vertexSize, uint32_t* pIndexList, const uint32_t indexSize,
                               glm::vec3& min, glm::vec3& max, glm::vec3& size) {
    if (indexSize > 0) {
        min = pVertexList[pIndexList[0]].point;
        max = pVertexList[pIndexList[0]].point;
    }

    for (uint32_t i = 1; i < indexSize; i++) {
        min = glm::min(min, pVertexList[pIndexList[i]].point);
        max = glm::max(max, pVertexList[pIndexList[i]].point);
    }
    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void vertexDataFromMesh(Mesh* m, std::vector<VertexData>& outData) {

    if (m->serialized == false) {
        if (m->uv.size() > 0) {
            for (uint32_t i = 0; i < m->iPoint.size(); i++) { // Todos os indices tem mesmo tamanho mas diferentes
                outData.push_back({m->point[m->iPoint[i]], m->normal[m->iNormal[i]], m->uv[m->iUv[i]]});
            }
        } else {
            for (uint32_t i = 0; i < m->iPoint.size(); i++) { // Todos os indices tem mesmo tamanho mas diferentes
                outData.push_back({m->point[m->iPoint[i]], m->normal[m->iNormal[i]], glm::vec2(0.0, 0.0)});
            }
        }
    } else {
        // vertices podem ser != 3 !!! sequencie!!!
        if (m->uv.size() > 0) {
            for (uint32_t i = 0; i < m->point.size(); i++) { // point, normal e UV sao sequenciais sem indice
                outData.push_back({m->point[i], m->normal[i], m->uv[i]});
            }
        } else {
            for (uint32_t i = 0; i < m->point.size(); i++) { // point, normal e UV sao sequenciais sem indice
                outData.push_back({m->point[i], m->normal[i], glm::vec2(0.0, 0.0)});
            }
        }
    }
}

// void vertexDataIndexToTriangle(VertexData* vertexData, uint32_t* indexData, const uint32_t& indexSize, std::list<Triangle*>& vTris) {
//     // Usa os indices já pre-calculado
//     for (uint32_t indice = 0; indice < indexSize; indice += 3) {
//         uint32_t pa = indexData[indice];
//         uint32_t pb = indexData[indice + 1];
//         uint32_t pc = indexData[indice + 2];
//         // Calcula Normal Face
//         glm::vec3 acc = vertexData[pa].normal + vertexData[pb].normal + vertexData[pc].normal;
//         glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
//         vTris.push_back(new Triangle(pa, pb, pc, normal, false));
//     }
// }

// void vertexDataToTriangle(VertexData* vertexData, const uint32_t& vertexSize, std::list<Triangle*>& vTris) {
//     // Calcula os indices na sequencia em que os vertices estão
//     for (uint32_t indice = 0; indice < vertexSize; indice += 3) {
//         uint32_t pa = indice;
//         uint32_t pb = indice + 1;
//         uint32_t pc = indice + 2;
//         // Calcula Normal Face
//         glm::vec3 acc = vertexData[pa].normal + vertexData[pb].normal + vertexData[pc].normal;
//         glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
//         vTris.push_back(new Triangle(pa, pb, pc, normal, false));
//     }
// }
} // namespace Chimera