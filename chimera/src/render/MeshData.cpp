#include "chimera/render/MeshData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void meshDataDestroy(MeshData& m) {
    m.vertexIndex.clear();
    m.vertexList.clear();
    m.normalIndex.clear();
    m.normalList.clear();
    m.textureIndex.clear();
    m.textureList.clear();
}

void meshDataMinMaxSize(const MeshData& m, glm::vec3& min, glm::vec3& max, glm::vec3& size) {

    for (unsigned int indice = 0; indice < m.vertexList.size(); indice++) {
        if (indice != 0) {
            min = glm::min(min, m.vertexList[indice]);
            max = glm::max(max, m.vertexList[indice]);
        } else {
            min = m.vertexList[indice];
            max = m.vertexList[indice];
        }
    }

    size.x = (glm::abs(max.x) + glm::abs(min.x)) / 2.0f;
    size.y = (glm::abs(max.y) + glm::abs(min.y)) / 2.0f;
    size.z = (glm::abs(max.z) + glm::abs(min.z)) / 2.0f;
}

void meshDataChangeSize(MeshData& m, const float& new_size, const bool& hasTexture) {

    if (hasTexture == false) {
        m.textureIndex.clear();
        m.textureList.clear();
    }

    for (unsigned int indice = 0; indice < m.vertexList.size(); indice++) {
        glm::vec3 val = m.vertexList[indice];
        m.vertexList[indice] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }
}

void meshDataDebugDados(const MeshData& m) {

    int linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex Indice ----------(%03d)", (int)m.vertexIndex.size());
    for (unsigned int indice = 0; indice < m.vertexIndex.size(); indice += 3) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                     m.vertexIndex[indice], m.vertexIndex[indice + 1], m.vertexIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex Lista ---------(%03d)", (int)m.vertexList.size());
    for (unsigned int indice = 0; indice < m.vertexList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%05.3f; %05.3f; %05.3f)", linha, indice,
                     m.vertexList[indice].x, m.vertexList[indice].y, m.vertexList[indice].z);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Indice ----------(%03d)", (int)m.normalIndex.size());
    for (unsigned int indice = 0; indice < m.normalIndex.size(); indice += 3) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                     m.normalIndex[indice], m.normalIndex[indice + 1], m.normalIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Lista ---------(%03d)", (int)m.normalList.size());
    for (unsigned int indice = 0; indice < m.normalList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%05.3f; %05.3f; %05.3f))", linha, indice,
                     m.normalList[indice].x, m.normalList[indice].y, m.normalList[indice].z);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Texture Indice ----------(%03d)", (int)m.textureIndex.size());
    for (unsigned int indice = 0; indice < m.textureIndex.size(); indice += 3) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                     m.textureIndex[indice], m.textureIndex[indice + 1], m.textureIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Texture Lista ---------(%03d)", (int)m.textureList.size());
    for (unsigned int indice = 0; indice < m.textureList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p: %02d (%05.3f; %05.3f)", linha, indice,
                     m.textureList[indice].x, m.textureList[indice].y);
        linha++;
    }
}

} // namespace Chimera