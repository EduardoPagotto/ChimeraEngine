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

    // if (hasTexture == false) {
    //     m->iUv.clear();
    //     m->uv.clear();
    // }

    for (uint32_t i = 0; i < m->point.size(); i++) {
        glm::vec3 val = m->point[i];
        m->point[i] = glm::vec3(val.x * new_size, val.y * new_size, val.z * new_size);
    }
}

void meshToTriangle(Mesh* m, std::list<Triangle*>& vTris) {
    for (uint32_t i = 0; i < m->iFace.size(); i++) {
        const glm::vec3 acc = m->normal[m->iFace[i].x] + m->normal[m->iFace[i].y] + m->normal[m->iFace[i].z];
        vTris.push_back(new Triangle(m->iFace[i], glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3), false));
    }
}

void idxSimplifieVec2(std::vector<glm::vec2>& in, std::vector<glm::vec2>& out, std::vector<uint32_t>& idxIn,
                      std::vector<uint32_t>& idxOut) {

    // percorrer todos os vertices
    bool find;
    for (uint32_t i = 0; i < idxIn.size(); i++) {

        find = false;
        const glm::vec2& p1 = in[idxIn[i]];
        for (uint32_t j = 0; j < idxOut.size(); j++) {

            const glm::vec2& p2 = out[j];
            if (is_nearVec2(p1, p2)) { // Procura por similar
                idxOut.push_back(j);
                find = true;
                break;
            }
        }

        if (find)
            continue; // FIXME: ver o std C17 para continue com label igual a rust e golang

        // se diferente adiciona ponto e cria novo indice
        out.push_back(p1);
        idxOut.push_back(out.size() - 1);
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Simplify2 In: %04lu out: %04lu faces: %04lu ", in.size(), out.size(), idxOut.size() / 3);
}

void idxSimplifieVec3(std::vector<glm::vec3>& in, std::vector<glm::vec3>& out, std::vector<uint32_t>& idxIn,
                      std::vector<uint32_t>& idxOut) {

    // percorrer todos os vertices
    bool find;
    for (uint32_t i = 0; i < idxIn.size(); i++) {

        // Procura por similar
        find = false;
        const glm::vec3& p1 = in[idxIn[i]];

        for (uint32_t j = 0; j < idxOut.size(); j++) {

            const glm::vec3& p2 = out[j];

            if (is_nearVec3(p1, p2)) {
                idxOut.push_back(j);
                find = true;
                break;
            }
        }

        if (find)
            continue;

        // se diferente adiciona vertice e cria novo indice
        out.push_back(p1);
        idxOut.push_back(out.size() - 1);
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Simplify3 In: %04lu out: %04lu faces: %04lu ", in.size(), out.size(), idxOut.size() / 3);
}

void meshSerialize(Mesh& inData, Mesh& outData) {

    std::vector<uint32_t> idxFace;
    idxFace.reserve(inData.iFace.size() * 3);
    for (const glm::uvec3& f : inData.iFace) {
        idxFace.push_back(f.x);
        idxFace.push_back(f.y);
        idxFace.push_back(f.z);
    }

    for (uint32_t face = 0; face < idxFace.size(); face++) {
        outData.point.push_back(inData.point[idxFace[face]]);
        outData.normal.push_back(inData.normal[idxFace[face]]);
        outData.uv.push_back((inData.uv.size() > 0) ? inData.uv[idxFace[face]] : glm::vec2(0.0, 0.0));
    }

    for (uint32_t i = 0; i < idxFace.size(); i += 3)
        outData.iFace.push_back({i, i + 1, i + 2});

    outData.serialized = true;
}

void meshReCompile(Mesh& inData, Mesh& outData) {

    std::vector<uint32_t> idxFace;
    idxFace.reserve(inData.iFace.size() * 3);
    for (const glm::uvec3& f : inData.iFace) {
        idxFace.push_back(f.x);
        idxFace.push_back(f.y);
        idxFace.push_back(f.z);
    }

    std::vector<uint32_t> index;

    // percorrer todos os vertices
    bool find;
    for (uint32_t i = 0; i < idxFace.size(); i++) {
        // Procura por similar
        find = false;
        for (uint32_t j = 0; j < index.size(); j++) {
            if (is_nearVec3(inData.point[idxFace[i]], outData.point[j]) &&   // compara pontos
                is_nearVec3(inData.normal[idxFace[i]], outData.normal[j]) && // compara normal
                is_nearVec2(inData.uv[idxFace[i]], outData.uv[j])) {         // compara uv

                index.push_back(j);
                find = true;
                break;
            }
        }

        if (find)
            continue;

        // se diferente adiciona vertice e cria novo indice
        outData.point.push_back(inData.point[idxFace[i]]);
        outData.normal.push_back(inData.normal[idxFace[i]]);
        outData.uv.push_back(inData.uv[idxFace[i]]);

        uint32_t n = outData.point.size() - 1;
        index.push_back(n); // FIXME: usar copia rapida
    }

    for (uint32_t i = 0; i < index.size(); i += 3)
        outData.iFace.push_back({index[i], index[i + 1], index[i + 2]});

    outData.iFace.assign(outData.iFace.begin(), outData.iFace.end());

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Compile Mesh In: %04lu Mesh out: %04lu Index out: %04lu ", inData.point.size(),
                 outData.point.size(), outData.iFace.size());
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
    m->point.clear();
    m->normal.clear();
    m->uv.clear();
    m->iFace.clear();
}

void meshDebug(Mesh* m, bool _showAll) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Points : %03d", (int)m->point.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normals: %03d", (int)m->normal.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UVs    : %03d", (int)m->uv.size());
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Faces  : %03d", (int)m->iFace.size());

    if (_showAll == true) {
        uint32_t i = 0;
        for (const glm::vec3& v : m->point)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point: %03d (%05.3f; %05.3f; %05.3f)", i++, v.x, v.y, v.z);

        i = 0;
        for (const glm::vec3& v : m->normal)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Normal: %03d (%05.3f; %05.3f; %05.3f))", i++, v.x, v.y, v.z);

        i = 0;
        for (const glm::vec2& v : m->uv)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "UV: %03d (%05.3f; %05.3f)", i++, v.x, v.y);

        i = 0;
        for (const glm::uvec3& face : m->iFace) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Point Index: %03d (%03d; %03d; %03d)", i, face.x, face.y, face.z);
            i += 3;
        }
    }
}
} // namespace Chimera