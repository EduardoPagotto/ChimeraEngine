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

    for (unsigned int i = 1; i < vertexList.size(); i++) {
        min = glm::min(min, vertexList[i]);
        max = glm::max(max, vertexList[i]);
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

    for (unsigned int i = 0; i < vertexList.size(); i++) {
        glm::vec3 val = vertexList[i];
        vertexList[i] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }
}

void MeshData::toVertexData(std::vector<VertexData>& outData) {
    if (oneIndex == false)
        _toVertexDataNotOneIndex(outData);
    else
        _toVertexDataOneIndex(outData);
}

void MeshData::toTriangle(std::vector<Triangle>& vecTriangle, std::vector<unsigned int>& _index) {
    if (oneIndex == false)
        _toTriangleNotOneIndex(vecTriangle, _index);
    else
        _toTriangleOneIndex(vecTriangle, _index);
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

void MeshData::_toTriangleOneIndex(std::vector<Triangle>& vecTriangle, std::vector<unsigned int>& _index) {

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

    faceIndex(_index);
}

void MeshData::_toTriangleNotOneIndex(std::vector<Triangle>& vecTriangle, std::vector<unsigned int>& _index) {

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
        _index.push_back(A);
        // t.debugData();
    }
}

void MeshData::faceIndex(std::vector<unsigned int>& _index) {
    for (unsigned int i = 0; i < vertexIndex.size(); i += 3) {
        unsigned int v = vertexIndex[i];
        _index.push_back(v);
    }
}

void MeshData::debugDados(bool _showAll) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex: %03d Index: %03d", (int)vertexList.size(), (int)vertexIndex.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal: %03d Index: %03d", (int)normalList.size(), (int)normalIndex.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV:     %03d Index: %03d", (int)uvList.size(), (int)uvIndex.size());

    if (_showAll == true) {
        for (unsigned int i = 0; i < vertexList.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex: %03d (%05.3f; %05.3f; %05.3f)", i, vertexList[i].x,
                         vertexList[i].y, vertexList[i].z);

        for (unsigned int i = 0; i < vertexIndex.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex Index: %03d (%03d; %03d; %03d)", i, vertexIndex[i],
                         vertexIndex[i + 1], vertexIndex[i + 2]);

        for (unsigned int i = 0; i < normalList.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal: %03d (%05.3f; %05.3f; %05.3f))", i, normalList[i].x,
                         normalList[i].y, normalList[i].z);

        for (unsigned int i = 0; i < normalIndex.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Index: %03d (%03d; %03d; %03d)", i, normalIndex[i],
                         normalIndex[i + 1], normalIndex[i + 2]);

        for (unsigned int i = 0; i < uvList.size(); i++)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV: %03d (%05.3f; %05.3f)", i, uvList[i].x, uvList[i].y);

        for (unsigned int i = 0; i < uvIndex.size(); i += 3)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV Index: %03d (%03d; %03d; %03d)", i, uvIndex[i], uvIndex[i + 1],
                         uvIndex[i + 2]);
    }
}
} // namespace Chimera