#include "chimera/core/visible/Mesh.hpp"
#include "chimera/core/mathGL.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

MeshType getMeshTypeFromString(const std::string& text) {
    if (text == "ARRAY") {
        return MeshType::ARRAY;
    } else if (text == "BSPTREE") {
        return MeshType::BSTREE;
    }

    return MeshType::SIMPLE;
}

void meshDataScale(Mesh* m, const float& new_size, const bool& hasTexture) {

    if (hasTexture == false) {
        m->iUv.clear();
        m->uv.clear();
    }

    for (uint32_t i = 0; i < m->point.size(); i++) {
        glm::vec3 val = m->point[i];
        m->point[i] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }
}

void meshToTriangle(Mesh* m, std::list<Triangle*>& vTris) {

    for (uint32_t indice = 0; indice < m->iPoint.size(); indice += 3) {
        uint32_t pa = m->iPoint[indice];
        uint32_t pb = m->iPoint[indice + 1];
        uint32_t pc = m->iPoint[indice + 2];
        // Calcula Normal Face
        glm::vec3 acc = m->normal[pa] + m->normal[pb] + m->normal[pc]; // vertexData[pa].normal + vertexData[pb].normal +
                                                                       // vertexData[pc].normal;
        glm::vec3 normal = glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3);
        vTris.push_back(new Triangle(pa, pb, pc, normal, false));
    }
}

void meshReCompile(Mesh& inData, Mesh& outData) {
    // percorrer todos os vertices
    bool find;
    for (uint32_t i = 0; i < inData.iPoint.size(); i++) {
        // Procura por similar
        find = false;
        for (uint32_t j = 0; j < outData.iPoint.size(); j++) {
            if (is_nearVec3(inData.point[inData.iPoint[i]], outData.point[j]) &&    // compara pontos
                is_nearVec3(inData.normal[inData.iNormal[i]], outData.normal[j]) && // compara normal
                is_nearVec2(inData.uv[inData.iUv[i]], outData.uv[j])) {             // compara uv

                outData.iPoint.push_back(j);
                find = true;
                break;
            }
        }

        if (find)
            continue;

        // se diferente adiciona vertice e cria novo indice
        outData.point.push_back(inData.point[inData.iPoint[i]]);
        outData.normal.push_back(inData.normal[inData.iNormal[i]]);
        outData.uv.push_back(inData.uv[inData.iUv[i]]);

        uint32_t n = outData.point.size() - 1;
        outData.iPoint.push_back(n); // FIXME: usar copia rapida
    }

    outData.iNormal.assign(outData.iPoint.begin(), outData.iPoint.end());
    outData.iUv.assign(outData.iPoint.begin(), outData.iPoint.end());

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Compile Mesh In: %04lu Mesh out: %04lu Index out: %04lu ", inData.point.size(),
                 outData.point.size(), outData.iPoint.size());
}

void meshMinMaxSize(Mesh* m, glm::vec3& min, glm::vec3& max, glm::vec3& size) {
    if (m->point.size() > 0) {
        min = m->point[0];
        max = m->point[0];
    }

    for (uint32_t i = 1; i < m->point.size(); i++) {
        min = glm::min(min, m->point[i]);
        max = glm::max(max, m->point[i]);
    }

    // TODO: Era half size ??
    size = getSizeMinMax(min, max);
}

void meshDataClean(Mesh* m) { //??
    m->iPoint.clear();
    m->point.clear();
    m->iNormal.clear();
    m->normal.clear();
    m->iUv.clear();
    m->uv.clear();
}

void meshDebug(Mesh* m, bool _showAll) {

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
} // namespace Chimera