#include "chimera/core/visible/Mesh.hpp"
#include "chimera/core/space/space.hpp"
#include <SDL2/SDL.h>

namespace ce {

MeshType getMeshTypeFromString(const std::string& text) {
    if (text == "ARRAY") {
        return MeshType::ARRAY;
    } else if (text == "BSPTREE") {
        return MeshType::BSTREE;
    }

    return MeshType::SIMPLE;
}

void meshToTriangle(Mesh& m, std::list<std::shared_ptr<Triangle>>& vTris) {
    for (uint32_t i = 0; i < m.iFace.size(); i++) {
        const glm::vec3 acc = m.vertex[m.iFace[i].x].normal + m.vertex[m.iFace[i].y].normal + m.vertex[m.iFace[i].z].normal;
        vTris.push_back(std::make_shared<Triangle>(m.iFace[i], glm::vec3(acc.x / 3, acc.y / 3, acc.z / 3), false));
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
            if (const glm::vec2& p2 = out[j]; isNearV2(p1, p2)) { // Procura por similar
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
            if (const glm::vec3& p2 = out[j]; isNearV3(p1, p2)) {
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

    outData.vertex.reserve(inData.iFace.size() * 3); // Reserve Vertex(point, normal, tex)

    for (const glm::uvec3& face : inData.iFace) {
        outData.vertex.push_back({inData.vertex[face.x].point, inData.vertex[face.x].normal, inData.vertex[face.x].uv}); // Vertice A
        outData.vertex.push_back({inData.vertex[face.y].point, inData.vertex[face.y].normal, inData.vertex[face.y].uv}); // Vertice B
        outData.vertex.push_back({inData.vertex[face.z].point, inData.vertex[face.z].normal, inData.vertex[face.z].uv}); // Vertice C
    }

    for (uint32_t i = 0; i < outData.vertex.size(); i += 3) // Serialize Vertex (0,1,2),(3,4,5),...
        outData.iFace.push_back({i, i + 1, i + 2});

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Serialize Vertex: %04lu -> %04lu Faces: %04lu ", inData.vertex.size(), outData.vertex.size(),
                 outData.iFace.size());
}

void meshReindex(Mesh& inData, Mesh& outData) {

    std::vector<uint32_t> idxFace;
    idxFace.reserve(inData.iFace.size() * 3);
    for (const glm::uvec3& f : inData.iFace) {
        idxFace.push_back(f.x);
        idxFace.push_back(f.y);
        idxFace.push_back(f.z);
    }

    std::vector<uint32_t> index;
    index.reserve(inData.iFace.size() * 3);

    outData.vertex.reserve(inData.vertex.size());

    // percorrer todos os vertices
    bool find;
    for (uint32_t i = 0; i < idxFace.size(); i++) {
        // Procura por similar
        find = false;
        for (uint32_t j = 0; j < index.size(); j++) {                                   // FIXME: trocar por vertexdata comp!!!!!
            if (isNearV3(inData.vertex[idxFace[i]].point, outData.vertex[j].point) &&   // compara pontos
                isNearV3(inData.vertex[idxFace[i]].normal, outData.vertex[j].normal) && // compara normal
                isNearV2(inData.vertex[idxFace[i]].uv, outData.vertex[j].uv)) {         // compara uv

                index.push_back(j);
                find = true;
                break;
            }
        }

        if (find)
            continue;

        // se diferente adiciona vertice e cria novo indice
        outData.vertex.push_back({inData.vertex[idxFace[i]].point,  //
                                  inData.vertex[idxFace[i]].normal, //
                                  inData.vertex[idxFace[i]].uv});   //

        index.push_back(outData.vertex.size() - 1);
    }

    for (uint32_t i = 0; i < index.size(); i += 3)
        outData.iFace.push_back({index[i], index[i + 1], index[i + 2]});

    outData.iFace.assign(outData.iFace.begin(), outData.iFace.end());

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "ReIndex Vertex: %04lu -> %04lu Faces: %04lu ", inData.vertex.size(), outData.vertex.size(),
                 outData.iFace.size());
}

void meshDebug(const Mesh& m, bool _showAll) {

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex : %03d, Faces  : %03d", (int)m.vertex.size(), (int)m.iFace.size());
    if (_showAll == true) {
        uint32_t i = 0;
        for (const VertexData& v : m.vertex)
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Vertex: %03d (%05.3f; %05.3f; %05.3f),(%02.3f; %02.3f; %02.3f), (%01.4f; %01.4f)", i++,
                         v.point.x, v.point.y, v.point.z, v.normal.x, v.normal.y, v.normal.z, v.uv.x, v.uv.y);

        i = 0;
        for (const glm::uvec3& face : m.iFace) {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Index: %03d (%03d; %03d; %03d)", i, face.x, face.y, face.z);
            i += 3;
        }
    }
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexBoundaries(std::vector<VertexData>& vArray) {
    glm::vec3 min, max, size;
    if (vArray.size() > 0) {
        min = vArray[0].point;
        max = vArray[0].point;
    }

    for (const VertexData& v : vArray) {
        min = glm::min(min, v.point);
        max = glm::max(max, v.point);
    }

    return {min, max, getSizeMinMax(min, max)};
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> vertexIndexedBoundaries(std::vector<VertexData>& vArray, TrisIndex& tris) {
    glm::vec3 min, max;
    if (tris.size() > 0) {
        max = min = vArray[tris[0].x].point;
        max = min = vArray[tris[0].y].point;
        max = min = vArray[tris[0].z].point;
    }

    for (const glm::uvec3& i : tris) {
        min = glm::min(min, vArray[i.x].point);
        min = glm::min(min, vArray[i.y].point);
        min = glm::min(min, vArray[i.z].point);

        max = glm::max(max, vArray[i.x].point);
        max = glm::max(max, vArray[i.y].point);
        max = glm::max(max, vArray[i.z].point);
    }

    return {min, max, getSizeMinMax(min, max)};
}
} // namespace ce