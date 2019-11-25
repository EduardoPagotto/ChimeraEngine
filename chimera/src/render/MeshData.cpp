#include "chimera/render/MeshData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

MeshData::MeshData() {}

MeshData::MeshData(const MeshData& _cpy) {
    vertexIndex.reserve(_cpy.vertexIndex.size());
    copy(_cpy.vertexIndex.begin(), _cpy.vertexIndex.end(), back_inserter(vertexIndex));
    copy(_cpy.vertexList.begin(), _cpy.vertexList.end(), back_inserter(vertexList));

    normalIndex.reserve(_cpy.normalIndex.size());
    copy(_cpy.normalIndex.begin(), _cpy.normalIndex.end(), back_inserter(normalIndex));
    copy(_cpy.normalList.begin(), _cpy.normalList.end(), back_inserter(normalList));

    textureIndex.reserve(_cpy.textureIndex.size());
    copy(_cpy.textureIndex.begin(), _cpy.textureIndex.end(), back_inserter(textureIndex));
    copy(_cpy.textureList.begin(), _cpy.textureList.end(), back_inserter(textureList));
}

MeshData::~MeshData() {
    vertexIndex.clear();
    vertexList.clear();
    normalIndex.clear();
    normalList.clear();
    textureIndex.clear();
    textureList.clear();
}

glm::vec3 MeshData::defineAABB() {

    glm::vec3 min, max;
    for (unsigned int indice = 0; indice < vertexList.size(); indice++) {
        if (indice != 0) {
            min = glm::min(min, vertexList[indice]);
            max = glm::max(max, vertexList[indice]);
        } else {
            min = vertexList[indice];
            max = vertexList[indice];
        }
    }

    aabb.set(min, max);
    return aabb.getSize();
}

void MeshData::changeSize(const float& new_size, bool hasTexture) {

    if (hasTexture == false) {
        textureIndex.clear();
        textureList.clear();
    }

    for (unsigned int indice = 0; indice < vertexList.size(); indice++) {
        glm::vec3 val = vertexList[indice];
        vertexList[indice] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }

    defineAABB();
}

void MeshData::debugDados() {

    int linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex Indice ----------(%03d)", (int)vertexIndex.size());
    for (unsigned int indice = 0; indice < vertexIndex.size(); indice += 3) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                     vertexIndex[indice], vertexIndex[indice + 1], vertexIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex Lista ---------(%03d)", (int)vertexList.size());
    for (unsigned int indice = 0; indice < vertexList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%05.3f; %05.3f; %05.3f)", linha, indice,
                     vertexList[indice].x, vertexList[indice].y, vertexList[indice].z);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Indice ----------(%03d)", (int)normalIndex.size());
    for (unsigned int indice = 0; indice < normalIndex.size(); indice += 3) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                     normalIndex[indice], normalIndex[indice + 1], normalIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Lista ---------(%03d)", (int)normalList.size());
    for (unsigned int indice = 0; indice < normalList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%05.3f; %05.3f; %05.3f))", linha, indice,
                     normalList[indice].x, normalList[indice].y, normalList[indice].z);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Texture Indice ----------(%03d)", (int)textureIndex.size());
    for (unsigned int indice = 0; indice < textureIndex.size(); indice += 3) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%02d; %03d; %04d)", linha, indice,
                     textureIndex[indice], textureIndex[indice + 1], textureIndex[indice + 2]);
        linha++;
    }

    linha = 0;
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Texture Lista ---------(%03d)", (int)textureList.size());
    for (unsigned int indice = 0; indice < textureList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p: %02d (%05.3f; %05.3f)", linha, indice,
                     textureList[indice].x, textureList[indice].y);
        linha++;
    }
}

} // namespace Chimera