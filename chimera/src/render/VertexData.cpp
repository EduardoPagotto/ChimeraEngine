#include "chimera/render/VertexData.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

bool getSimilarVertexIndex(VertexData& in_vertex, std::vector<VertexData>& out_vertex, uint32_t& result) {
    // Percorrer todos os vertex ja existentes na lista
    for (uint32_t i = 0; i < out_vertex.size(); i++) {
        if (IS_NEARVEC3(in_vertex.point, out_vertex[i].point) && IS_NEARVEC3(in_vertex.normal, out_vertex[i].normal) &&
            IS_NEARVEC2(in_vertex.uv, out_vertex[i].uv)) {
            result = i;
            return true;
        }
    }
    return false;
}

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

    // TODO: Era half size ??
    size = GETSIZEMINMAX(min, max);
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
    // TODO: Era half size ??
    size = GETSIZEMINMAX(min, max);
}

void vertexDataFromMesh(Mesh* m, std::vector<VertexData>& outData) {

    for (uint32_t i = 0; i < m->vertex.size(); i++) { // point, normal e UV sao sequenciais sem indice
        outData.push_back({m->vertex[i]});
    }
}
} // namespace Chimera