#include "chimera/core/MeshData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

MeshData::MeshData() {}

MeshData::MeshData(const MeshData& _cpy) {
    vertexIndex.reserve(_cpy.vertexIndex.size());
    copy(_cpy.vertexIndex.begin(), _cpy.vertexIndex.end(), back_inserter(vertexIndex));
    copy(_cpy.vertexList.begin(), _cpy.vertexList.end(), back_inserter(vertexList));

    colorList.reserve(_cpy.colorList.size());
    copy(_cpy.colorList.begin(), _cpy.colorList.end(), back_inserter(colorList));

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
    colorList.clear();
    normalIndex.clear();
    normalList.clear();
    textureIndex.clear();
    textureList.clear();
}

void MeshData::textureFix() {
    // Ajuste de textura do imageSDL invertendo valor de V
    for (int indice = 0; indice < textureIndex.size(); indice++) {
        int posTex = textureIndex[indice];
        textureList[posTex].y = 1 - textureList[posTex].y;
    }
}

glm::vec3 MeshData::getSizeBox() {

    glm::vec3 retorno(0.0f, 0.0f, 0.0f);
    glm::vec3 l_max(0.0f, 0.0f, 0.0f);
    glm::vec3 l_min(0.0f, 0.0f, 0.0f);

    for (unsigned int indice = 0; indice < vertexList.size(); indice++) {

        if (l_max.x < vertexList[indice].x)
            l_max.x = vertexList[indice].x;

        if (l_max.y < vertexList[indice].y)
            l_max.y = vertexList[indice].y;

        if (l_max.z < vertexList[indice].z)
            l_max.z = vertexList[indice].z;

        if (l_min.x > vertexList[indice].x)
            l_min.x = vertexList[indice].x;

        if (l_min.y > vertexList[indice].y)
            l_min.y = vertexList[indice].y;

        if (l_min.z > vertexList[indice].z)
            l_min.z = vertexList[indice].z;
    }

    return glm::vec3((glm::abs(l_max.x) + glm::abs(l_min.x)) / 2, (glm::abs(l_max.y) + glm::abs(l_min.y)) / 2,
                     (glm::abs(l_max.z) + glm::abs(l_min.z)) / 2);
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

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Color Lista ---------(%03d)", (int)colorList.size());
    for (unsigned int indice = 0; indice < colorList.size(); indice++) {
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Linha: %02d : p:%02d (%05.3f; %05.3f; %05.3f)", linha, indice,
                     colorList[indice].x, colorList[indice].y, colorList[indice].z);
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