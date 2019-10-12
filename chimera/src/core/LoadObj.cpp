#include "chimera/core/LoadObj.hpp"
#include "chimera/core/Exception.hpp"

#include <SDL2/SDL.h>

namespace Chimera {

void loadObj(const char* _fineName, MeshData* _mesh) {

    FILE* fp = fopen(_fineName, "r");
    if (fp == NULL)
        throw Exception(std::string("Erro ao abrir arquivo:" + std::string(_fineName)));

    short hasColorV = 0;
    bool textuaOn = false;
    bool normalOn = false;
    char* line = NULL;
    size_t len = 0;
    unsigned int pos_linha = 0;
    while ((getline(&line, &len, fp)) != -1) {

        float x, y, z, u, v;
        float r, g, b;
        int A[3];
        int B[3];
        int C[3];

        pos_linha++;

        if (line[0] == '#')
            continue;
        else if (line[0] == 'v') {
            if (line[1] == ' ') {
                int n = sscanf(line, "v %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
                if (n == 3) {
                    if (hasColorV == 2)
                        throw Exception(std::string("linha " + std::to_string(pos_linha) +
                                                    " itens vertice com cor arquivo " + std::string(_fineName)));
                    hasColorV = 1;
                } else if (n == 6) {
                    if (hasColorV == 1)
                        throw Exception(std::string("linha " + std::to_string(pos_linha) +
                                                    " itens vertice sem com arquivo " + std::string(_fineName)));
                    hasColorV = 2;
                    _mesh->colorList.push_back(glm::vec3(r, g, b));
                } else {
                    throw Exception(std::string("linha " + std::to_string(pos_linha) + " parse invalido com " +
                                                std::to_string(n) + " arquivo:" + std::string(_fineName)));
                }
                _mesh->vertexList.push_back(glm::vec3(x, y, z));

            } else if (line[1] == 'n') {
                normalOn = true;
                int n = sscanf(line, "vn %f %f %f", &x, &y, &z);
                _mesh->normalList.push_back(glm::vec3(x, y, z));
            } else if (line[1] == 't') {
                textuaOn = true;
                int n = sscanf(line, "vt %f %f", &u, &v);
                _mesh->textureList.push_back(glm::vec2(u, v));
            }
        } else if (line[0] == 'f') {
            if (line[1] == ' ') {
                if ((textuaOn == true) && (normalOn == true)) {
                    int n = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &A[0], &B[0], &C[0], &A[1], &B[1], &C[1],
                                   &A[2], &B[2], &C[2]);
                } else if ((textuaOn == false) && (normalOn == true)) {
                    int n = sscanf(line, "f %d//%d %d//%d %d//%d", &A[0], &C[0], &A[1], &C[1], &A[2], &C[2]);
                } else if ((textuaOn == true) && (normalOn == false)) {
                    int n = sscanf(line, "f %d/%d %d/%d %d/%d", &A[0], &B[0], &A[1], &B[1], &A[2], &B[2]);
                } else {
                    int n = sscanf(line, "f %d %d %d", &A[0], &A[1], &A[2]);
                }

                for (int indice = 0; indice < 3; indice++) {
                    _mesh->vertexIndex.push_back(A[indice] - 1);

                    if (textuaOn == true)
                        _mesh->textureIndex.push_back(B[indice] - 1);

                    if (normalOn == true)
                        _mesh->normalIndex.push_back(C[indice] - 1);
                }
            }
        }
    }
    fclose(fp);
    if (line)
        free(line);
}

void convertMeshDataTriangle(MeshData* _pMesh, std::vector<Triangle>* _pListPolygon) {

    for (short indice = 0; indice < _pMesh->vertexIndex.size(); indice += 3) {

        Triangle t = Triangle(glm::vec3(0.0, 0.0, 0.0),  // A zerados carga em loop
                              glm::vec3(0.0, 0.0, 0.0),  // B zerados carga em loop
                              glm::vec3(0.0, 0.0, 0.0)); // C zerados carga em loop

        for (short tri = 0; tri < 3; tri++) {

            t.vertex[tri].position = _pMesh->vertexList[_pMesh->vertexIndex[indice + tri]];

            if (_pMesh->normalList.size() > 0)
                t.vertex[tri].normal = _pMesh->normalList[_pMesh->normalIndex[indice + tri]];

            if (_pMesh->colorList.size() > 0)
                t.vertex[tri].color = _pMesh->colorList[_pMesh->vertexIndex[indice + tri]];

            if (_pMesh->textureList.size() > 0)
                t.vertex[tri].texture = _pMesh->textureList[_pMesh->textureIndex[indice + tri]];
        }

        _pListPolygon->push_back(t);
        // t.debugData();
    }
}

void convertMeshDataVertexData(MeshData* _pMesh, std::vector<VertexData>& outData) {
    unsigned l_numFaces = _pMesh->vertexIndex.size() / 3;
    unsigned int l_index = 0;
    unsigned int fa = 0;
    for (unsigned int face = 0; face < l_numFaces; face++) {
        fa = face * 3;
        for (unsigned int point = 0; point < 3; point++) {
            l_index = fa + point;
            outData.push_back({_pMesh->vertexList[_pMesh->vertexIndex[l_index]], // Vertice
                               _pMesh->normalList[_pMesh->normalIndex[l_index]], // Normal
                               (_pMesh->textureList.size() > 0) ? _pMesh->textureList[_pMesh->textureIndex[l_index]]
                                                                : glm::vec2(0.0, 0.0)}); // Texture
        }
    }
}

void convertMeshDataVertexDataFull(MeshData* _pMesh, std::vector<VertexDataFull>& outData) {
    unsigned l_numFaces = _pMesh->vertexIndex.size() / 3;
    unsigned int l_index = 0;
    unsigned int fa = 0;
    for (unsigned int face = 0; face < l_numFaces; face++) {
        fa = face * 3;
        for (unsigned int point = 0; point < 3; point++) {
            l_index = fa + point;
            outData.push_back({_pMesh->vertexList[_pMesh->vertexIndex[l_index]], // Vertice
                               _pMesh->normalList[_pMesh->normalIndex[l_index]], // Normal
                               (_pMesh->colorList.size() > 0) ? _pMesh->colorList[_pMesh->vertexIndex[l_index]]
                                                              : glm::vec3(0.0, 0.0, 0.0), // Color
                               (_pMesh->textureList.size() > 0) ? _pMesh->textureList[_pMesh->textureIndex[l_index]]
                                                                : glm::vec2(0.0, 0.0)}); // Texture
        }
    }
}
} // namespace Chimera