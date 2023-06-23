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
    for (uint32_t i = 0; i < m->iPoint.size(); i++) {
        const glm::vec3 acc = m->normal[m->iNormal[i].x] + m->normal[m->iNormal[i].y] + m->normal[m->iNormal[i].z];
        vTris.push_back(new Triangle(m->iPoint[i], glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3), false));
    }
}

void meshReCompile(Mesh& inData, Mesh& outData) {

    std::vector<uint32_t> iPoint;
    iPoint.reserve(inData.iPoint.size() * 3);
    for (const glm::uvec3& f : inData.iPoint) {
        iPoint.push_back(f.x);
        iPoint.push_back(f.y);
        iPoint.push_back(f.z);
    }

    std::vector<uint32_t> iNormal;
    iNormal.reserve(inData.iNormal.size() * 3);
    for (const glm::uvec3& f : inData.iNormal) {
        iNormal.push_back(f.x);
        iNormal.push_back(f.y);
        iNormal.push_back(f.z);
    }

    std::vector<uint32_t> iUv;
    iUv.reserve(inData.iUv.size() * 3);
    for (const glm::uvec3& f : inData.iUv) {
        iUv.push_back(f.x);
        iUv.push_back(f.y);
        iUv.push_back(f.z);
    }

    std::vector<uint32_t> index;

    // percorrer todos os vertices
    bool find;
    for (uint32_t i = 0; i < iPoint.size(); i++) {
        // Procura por similar
        find = false;
        for (uint32_t j = 0; j < index.size(); j++) {
            if (is_nearVec3(inData.point[iPoint[i]], outData.point[j]) &&    // compara pontos
                is_nearVec3(inData.normal[iNormal[i]], outData.normal[j]) && // compara normal
                is_nearVec2(inData.uv[iUv[i]], outData.uv[j])) {             // compara uv

                index.push_back(j);
                find = true;
                break;
            }
        }

        if (find)
            continue;

        // se diferente adiciona vertice e cria novo indice
        outData.point.push_back(inData.point[iPoint[i]]);
        outData.normal.push_back(inData.normal[iNormal[i]]);
        outData.uv.push_back(inData.uv[iUv[i]]);

        uint32_t n = outData.point.size() - 1;
        index.push_back(n); // FIXME: usar copia rapida
    }

    for (uint32_t i = 0; i < iPoint.size(); i += 3)
        outData.iPoint.push_back({iPoint[i], iPoint[1 + 1], iPoint[i + 2]});

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
        uint32_t i = 0;
        for (const glm::vec3& v : m->point)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point: %03d (%05.3f; %05.3f; %05.3f)", i++, v.x, v.y, v.z);

        for (const glm::uvec3& face : m->iPoint)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point Index: %03d (%03d; %03d; %03d)", i++, face.x, face.y, face.z);

        i = 0;
        for (const glm::vec3& v : m->normal)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal: %03d (%05.3f; %05.3f; %05.3f))", i++, v.x, v.y, v.z);

        for (const glm::uvec3& face : m->iNormal)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal Index: %03d (%03d; %03d; %03d)", i++, face.x, face.y, face.z);

        i = 0;
        for (const glm::vec2& v : m->uv)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV: %03d (%05.3f; %05.3f)", i++, v.x, v.y);

        for (const glm::uvec3& face : m->iUv)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV Index: %03d (%03d; %03d; %03d)", i++, face.x, face.y, face.z);
    }
}
} // namespace Chimera