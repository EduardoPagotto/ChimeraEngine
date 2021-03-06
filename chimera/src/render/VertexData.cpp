#include "chimera/render/VertexData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2) { return fabs(v1 - v2) < EPSILON; } // 0.01f

bool getSimilarVertexIndex(VertexData& in_vertex, std::vector<VertexData>& out_vertex, unsigned int& result) {
    // Percorrer todos os vertex ja existentes na lista
    for (unsigned int i = 0; i < out_vertex.size(); i++) {

        if (is_near(in_vertex.position.x, out_vertex[i].position.x) && is_near(in_vertex.position.y, out_vertex[i].position.y) &&
            is_near(in_vertex.position.z, out_vertex[i].position.z) &&

            is_near(in_vertex.normal.x, out_vertex[i].normal.x) && is_near(in_vertex.normal.y, out_vertex[i].normal.y) &&
            is_near(in_vertex.normal.z, out_vertex[i].normal.z) &&

            is_near(in_vertex.texture.x, out_vertex[i].texture.x) && is_near(in_vertex.texture.y, out_vertex[i].texture.y)) {
            result = i;
            return true;
        }
    }
    return false;
}

void vertexDataIndexCompile(std::vector<VertexData>& inData, std::vector<VertexData>& outData, std::vector<unsigned int>& out_indices) {

    // percorrer todos os vertices
    for (unsigned int i = 0; i < inData.size(); i++) {
        // Procura por similar
        unsigned int index;
        if (getSimilarVertexIndex(inData[i], outData, index) == true) {
            // se entrotar usar apenas o indice
            out_indices.push_back(index);

        } else {
            // se nao adiciona a lista de novo vertex
            outData.push_back(inData[i]);
            out_indices.push_back((unsigned int)outData.size() - 1);
        }
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "VBO Vertex In: %04lu Vertex out: %04lu Index out: %04lu ", inData.size(), outData.size(),
                 out_indices.size());
}

glm::vec3 aproxEpsilon(const glm::vec3& dado) {
    return glm::vec3((fabs(dado.x) < EPSILON) ? 0.0f : dado.x,  // X
                     (fabs(dado.y) < EPSILON) ? 0.0f : dado.y,  // Y
                     (fabs(dado.z) < EPSILON) ? 0.0f : dado.z); // Z
}

} // namespace Chimera