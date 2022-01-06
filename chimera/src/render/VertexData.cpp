#include "chimera/render/VertexData.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2) { return fabs(v1 - v2) < EPSILON; } // 0.01f

bool getSimilarVertexIndex(VertexData& in_vertex, std::vector<VertexData>& out_vertex, uint32_t& result) {
    // Percorrer todos os vertex ja existentes na lista
    for (uint32_t i = 0; i < out_vertex.size(); i++) {

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

void vertexDataReorder(std::vector<VertexData>& inVertexData, std::vector<uint32_t>& inIndex, std::vector<VertexData>& outVertexData,
                       std::vector<uint32_t>& outIndex) {

    std::vector<VertexData> tempVertexData;

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Reorder Vertex In: ( %04lu / %04lu )", inVertexData.size(), inIndex.size());
    for (uint32_t i : inIndex) {
        // expand
        VertexData vd = inVertexData[i];
        tempVertexData.push_back(vd);
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Reorder Vertex full: %04lu", tempVertexData.size());

    vertexDataIndexCompile(tempVertexData, outVertexData, outIndex);
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
        min = pVertexList[0].position;
        max = pVertexList[0].position;
    }

    for (uint32_t i = 1; i < vertexSize; i++) {
        min = glm::min(min, pVertexList[i].position);
        max = glm::max(max, pVertexList[i].position);
    }

    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void vertexDataIndexMinMaxSize(VertexData* pVertexList, const uint32_t vertexSize, uint32_t* pIndexList, const uint32_t indexSize,
                               glm::vec3& min, glm::vec3& max, glm::vec3& size) {
    if (indexSize > 0) {
        min = pVertexList[pIndexList[0]].position;
        max = pVertexList[pIndexList[0]].position;
    }

    for (uint32_t i = 1; i < indexSize; i++) {
        min = glm::min(min, pVertexList[pIndexList[i]].position);
        max = glm::max(max, pVertexList[pIndexList[i]].position);
    }
    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void vertexDataMeshDataDebug(MeshData* m, bool _showAll) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Points: %03d Index: %03d", (int)m->point.size(), (int)m->iPoint.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normals: %03d Index: %03d", (int)m->normal.size(), (int)m->iNormal.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UVs:     %03d Index: %03d", (int)m->uv.size(), (int)m->iUv.size());

    if (_showAll == true) {
        for (uint32_t i = 0; i < m->point.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point: %03d (%05.3f; %05.3f; %05.3f)", i, m->point[i].x, m->point[i].y, m->point[i].z);

        for (uint32_t i = 0; i < m->iPoint.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point Index: %03d (%03d; %03d; %03d)", i, m->iPoint[i], m->iPoint[i + 1],
                         m->iPoint[i + 2]);

        for (uint32_t i = 0; i < m->normal.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal: %03d (%05.3f; %05.3f; %05.3f))", i, m->normal[i].x, m->normal[i].y,
                         m->normal[i].z);

        for (uint32_t i = 0; i < m->iNormal.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Index: %03d (%03d; %03d; %03d)", i, m->iNormal[i], m->iNormal[i + 1],
                         m->iNormal[i + 2]);

        for (uint32_t i = 0; i < m->uv.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV: %03d (%05.3f; %05.3f)", i, m->uv[i].x, m->uv[i].y);

        for (uint32_t i = 0; i < m->iUv.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV Index: %03d (%03d; %03d; %03d)", i, m->iUv[i], m->iUv[i + 1], m->iUv[i + 2]);
    }
}

void vertexDataMeshMinMaxSize(MeshData* m, glm::vec3& min, glm::vec3& max, glm::vec3& size) {
    if (m->point.size() > 0) {
        min = m->point[0];
        max = m->point[0];
    }

    for (uint32_t i = 1; i < m->point.size(); i++) {
        min = glm::min(min, m->point[i]);
        max = glm::max(max, m->point[i]);
    }

    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void vertexDataFromMesh(MeshData* m, std::vector<VertexData>& outData) {

    if (m->singleIndex == false) {
        if (m->uv.size() > 0) {
            for (uint32_t i = 0; i < m->iPoint.size(); i++) {
                outData.push_back({m->point[m->iPoint[i]], m->normal[m->iNormal[i]], m->uv[m->iUv[i]]});
            }
        } else {
            for (uint32_t i = 0; i < m->iPoint.size(); i++) {
                outData.push_back({m->point[m->iPoint[i]], m->normal[m->iNormal[i]], glm::vec2(0.0, 0.0)});
            }
        }
    } else {
        // vertices podem ser != 3 !!! sequencie!!!
        if (m->uv.size() > 0) {
            for (uint32_t i = 0; i < m->point.size(); i++) {
                outData.push_back({m->point[i], m->normal[i], m->uv[i]});
            }
        } else {
            for (uint32_t i = 0; i < m->point.size(); i++) {
                outData.push_back({m->point[i], m->normal[i], glm::vec2(0.0, 0.0)});
            }
        }
    }
}

void vertexDataMeshScale(MeshData* m, const float& new_size, const bool& hasTexture) {

    if (hasTexture == false) {
        m->iUv.clear();
        m->uv.clear();
    }

    for (uint32_t i = 0; i < m->point.size(); i++) {
        glm::vec3 val = m->point[i];
        m->point[i] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }
}

void vertexDataMeshClean(MeshData* m) {
    m->iPoint.clear();
    m->point.clear();
    m->iNormal.clear();
    m->normal.clear();
    m->iUv.clear();
    m->uv.clear();
}

// glm::vec3 aproxEpsilon(const glm::vec3& dado) {
//     return glm::vec3((fabs(dado.x) < EPSILON) ? 0.0f : dado.x,  // X
//                      (fabs(dado.y) < EPSILON) ? 0.0f : dado.y,  // Y
//                      (fabs(dado.z) < EPSILON) ? 0.0f : dado.z); // Z
// }

} // namespace Chimera