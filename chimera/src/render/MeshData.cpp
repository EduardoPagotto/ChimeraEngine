#include "chimera/render/MeshData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

MeshData::MeshData() { oneIndex = false; }

MeshData::~MeshData() { destroy(); }

void MeshData::destroy() {
    vertexIndex.clear();
    vertexList.clear();
    normalIndex.clear();
    normalList.clear();
    uvIndex.clear();
    uvList.clear();
}

void MeshData::getMinMaxSize(glm::vec3& min, glm::vec3& max, glm::vec3& size) {

    if (vertexList.size() > 0) {
        min = vertexList[0];
        max = vertexList[0];
    }

    for (unsigned int indice = 1; indice < vertexList.size(); indice++) {
        min = glm::min(min, vertexList[indice]);
        max = glm::max(max, vertexList[indice]);
    }

    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void MeshData::changeSize(const float& new_size, const bool& hasTexture) {

    if (hasTexture == false) {
        uvIndex.clear();
        uvList.clear();
    }

    for (unsigned int indice = 0; indice < vertexList.size(); indice++) {
        glm::vec3 val = vertexList[indice];
        vertexList[indice] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }
}

void MeshData::toVertexData(std::vector<VertexData>& outData) {
    if (oneIndex == false)
        _toVertexDataNotOneIndex(outData);
    else
        _toVertexDataOneIndex(outData);
}

void MeshData::toTriangle(std::vector<Triangle>& vecTriangle) {
    if (oneIndex == false)
        _toTriangleNotOneIndex(vecTriangle);
    else
        _toTriangleOneIndex(vecTriangle);
}

void MeshData::_toVertexDataNotOneIndex(std::vector<VertexData>& outData) {

    unsigned int B, C;
    for (unsigned int A = 0; A < vertexIndex.size(); A += 3) {

        B = A + 1;
        C = A + 2;

        // vertex, normal and texture triangle A
        outData.push_back({vertexList[vertexIndex[A]], normalList[normalIndex[A]],
                           (uvList.size() > 0) ? uvList[uvIndex[A]] : glm::vec2(0.0, 0.0)});

        // vertex, normal and texture triangle A
        outData.push_back({vertexList[vertexIndex[B]], normalList[normalIndex[B]],
                           (uvList.size() > 0) ? uvList[uvIndex[B]] : glm::vec2(0.0, 0.0)});

        // vertex, normal and texture triangle A
        outData.push_back({vertexList[vertexIndex[C]], normalList[normalIndex[C]],
                           (uvList.size() > 0) ? uvList[uvIndex[C]] : glm::vec2(0.0, 0.0)});
    }
}

void MeshData::_toVertexDataOneIndex(std::vector<VertexData>& outData) {

    unsigned int B, C;
    for (unsigned int A = 0; A < vertexList.size(); A += 3) {
        B = A + 1;
        C = A + 2;
        // vertex, normal and texture triangle A
        outData.push_back({vertexList[A], normalList[A], (uvList.size() > 0) ? uvList[A] : glm::vec2(0.0, 0.0)});

        // vertex, normal and texture triangle A
        outData.push_back({vertexList[B], normalList[B], (uvList.size() > 0) ? uvList[B] : glm::vec2(0.0, 0.0)});

        // vertex, normal and texture triangle A
        outData.push_back({vertexList[C], normalList[C], (uvList.size() > 0) ? uvList[C] : glm::vec2(0.0, 0.0)});
    }
}

void MeshData::_toTriangleOneIndex(std::vector<Triangle>& vecTriangle) {

    unsigned int B, C;

    // Load vertex, normal and texture of triangles A,B,C
    for (unsigned int A = 0; A < vertexList.size(); A += 3) {
        B = A + 1;
        C = A + 2;

        Triangle t = Triangle(vertexList[A],  // vertex triangle A
                              vertexList[B],  // vertex triangle B
                              vertexList[C]); // vertex triangle C

        if (normalList.size() > 0) {
            t.vertex[0].normal = normalList[A]; // normal triangle A
            t.vertex[1].normal = normalList[B]; // normal triangle B
            t.vertex[2].normal = normalList[C]; // normal triangle C
        }

        if (uvList.size() > 0) {
            t.vertex[0].texture = uvList[A]; // texture triangle A
            t.vertex[1].texture = uvList[B]; // texture triangle B
            t.vertex[2].texture = uvList[C]; // texture triangle C
        }

        vecTriangle.push_back(t);
        // t.debugData();
    }
}

void MeshData::_toTriangleNotOneIndex(std::vector<Triangle>& vecTriangle) {

    unsigned int B, C;

    // Load vertex, normal and texture of triangles A,B,C
    for (unsigned int A = 0; A < vertexIndex.size(); A += 3) {
        B = A + 1;
        C = A + 2;

        Triangle t = Triangle(vertexList[vertexIndex[A]],  // vertex triangle A
                              vertexList[vertexIndex[B]],  // vertex triangle B
                              vertexList[vertexIndex[C]]); // vertex triangle C

        if (normalList.size() > 0) {
            t.vertex[0].normal = normalList[normalIndex[A]]; // normal triangle A
            t.vertex[1].normal = normalList[normalIndex[B]]; // normal triangle B
            t.vertex[2].normal = normalList[normalIndex[C]]; // normal triangle C
        }

        if (uvList.size() > 0) {
            t.vertex[0].texture = uvList[uvIndex[A]]; // texture triangle A
            t.vertex[1].texture = uvList[uvIndex[B]]; // texture triangle B
            t.vertex[2].texture = uvList[uvIndex[C]]; // texture triangle C
        }

        vecTriangle.push_back(t);
        // t.debugData();
    }
}

void MeshData::debugDados(bool _showAll) {

    int linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex I: %03d L: %03d", (int)vertexIndex.size(), (int)vertexList.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal  I: %03d L: %03d", (int)normalIndex.size(), (int)normalList.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Texture I: %03d L: %03d", (int)uvIndex.size(), (int)uvList.size());

    if (_showAll == true) {
        for (unsigned int indice = 0; indice < vertexIndex.size(); indice += 3) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "L: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                         vertexIndex[indice], vertexIndex[indice + 1], vertexIndex[indice + 2]);
            linha++;
        }

        linha = 0;
        for (unsigned int indice = 0; indice < vertexList.size(); indice++) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "L: %02d : p:%02d (%05.3f; %05.3f; %05.3f)", linha, indice,
                         vertexList[indice].x, vertexList[indice].y, vertexList[indice].z);
            linha++;
        }

        linha = 0;
        for (unsigned int indice = 0; indice < normalIndex.size(); indice += 3) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "L: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                         normalIndex[indice], normalIndex[indice + 1], normalIndex[indice + 2]);
            linha++;
        }

        linha = 0;
        for (unsigned int indice = 0; indice < normalList.size(); indice++) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "L: %02d : p:%02d (%05.3f; %05.3f; %05.3f))", linha, indice,
                         normalList[indice].x, normalList[indice].y, normalList[indice].z);
            linha++;
        }

        linha = 0;
        for (unsigned int indice = 0; indice < uvIndex.size(); indice += 3) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "L: %02d : p:%02d (%02d; %03d; %04d)", linha, indice, uvIndex[indice],
                         uvIndex[indice + 1], uvIndex[indice + 2]);
            linha++;
        }

        linha = 0;
        for (unsigned int indice = 0; indice < uvList.size(); indice++) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "L: %02d : p: %02d (%05.3f; %05.3f)", linha, indice, uvList[indice].x,
                         uvList[indice].y);
            linha++;
        }
    }
}
} // namespace Chimera