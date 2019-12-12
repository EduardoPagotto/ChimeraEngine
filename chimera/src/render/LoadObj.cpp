#include "chimera/render/LoadObj.hpp"
#include "chimera/core/Exception.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

LoaderObj::LoaderObj(const std::string& _fileName) : materialFile(""), fileName(_fileName) {}

LoaderObj::~LoaderObj() {}

bool LoaderObj::getMesh(MeshData& _mesh) {

    FILE* fp = fopen(fileName.c_str(), "r");
    if (fp == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName.c_str());
        return false;
    }

    bool textuaOn = false;
    bool normalOn = false;
    char* line = NULL;
    size_t len = 0;
    unsigned int pos_linha = 0;
    while ((getline(&line, &len, fp)) != -1) {
        float x, y, z, u, v;
        int A[3];
        int B[3];
        int C[3];

        pos_linha++;

        if (line[0] == '#')
            continue;
        else if (line[0] == 'v') {
            if (line[1] == ' ') {
                if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
                    _mesh.addVertex(glm::vec3(x, y, z));
                    continue;
                }
                throw Exception(
                    std::string("linha " + std::to_string(pos_linha) + " parse invalido arquivo: " + fileName));

            } else if (line[1] == 'n') {
                normalOn = true;
                int n = sscanf(line, "vn %f %f %f", &x, &y, &z);
                _mesh.addNormal(glm::vec3(x, y, z));
            } else if (line[1] == 't') {
                textuaOn = true;
                int n = sscanf(line, "vt %f %f", &u, &v);
                _mesh.addUV(glm::vec2(u, v));
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
                    _mesh.addVertexIndex(A[indice] - 1);

                    if (textuaOn == true)
                        _mesh.addUVIndex(B[indice] - 1);

                    if (normalOn == true)
                        _mesh.addNormalIndex(C[indice] - 1);
                }
            }
        } else {
            std::string nova(line);
            if (getValidData(nova, std::string("mtllib ")) == true)
                materialFile = nova;
        }
    }
    fclose(fp);
    if (line)
        free(line);

    return true;
}

bool LoaderObj::getMaterial(Material& _material) {

    if (materialFile.size() == 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sem dados de Material em %s", fileName.c_str());
        return false;
    }

    FILE* fp = fopen(materialFile.c_str(), "r");
    if (fp == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", materialFile.c_str());
        return false;
    }

    char* line = NULL;
    size_t len = 0;
    unsigned int pos_linha = 0;
    while ((getline(&line, &len, fp)) != -1) {

        float r, g, b, a;
        pos_linha++;

        if (line[0] == '#')
            continue;
        else if (line[0] == 'K') {

            if (line[1] == 'a') {
                int n = sscanf(line, "Ka %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    _material.setAmbient(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    _material.setAmbient(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) +
                                    " material invalido arquivo: " + materialFile);
                }

            } else if (line[1] == 'd') {
                int n = sscanf(line, "Kd %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    _material.setDiffuse(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    _material.setDiffuse(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) +
                                    " material invalido arquivo: " + materialFile);
                }
            } else if (line[1] == 's') {
                int n = sscanf(line, "Ks %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    _material.setSpecular(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    _material.setSpecular(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) +
                                    " material invalido arquivo:" + materialFile);
                }
            }
        } else {
            // load texture
            std::string nova(line);
            if (getValidData(nova, std::string("map_Kd ")) == true)
                _material.addTexture(new TextureImg(SHADE_TEXTURE_DIFFUSE, nova));
        }
    }
    return true;
}

void LoaderObj::eraseAllSubStr(std::string& mainStr, const std::string& toErase) {
    size_t pos = std::string::npos;
    while ((pos = mainStr.find(toErase)) != std::string::npos) {
        mainStr.erase(pos, toErase.length());
    }
}

bool LoaderObj::getValidData(std::string& nova, const std::string& comando) {
    std::string fim_linha("\n");
    std::size_t found = nova.find(comando);
    if (found != std::string::npos) {
        eraseAllSubStr(nova, comando);
        eraseAllSubStr(nova, fim_linha);
        return true;
    }
    return false;
}

// void loadObjMtl(const std::string& _fineNameMtl, MeshData& _mesh, Material& _pMaterial) {
//     std::string matFile;
//     loadObj(_fineNameMtl, _mesh, matFile);

//     if (matFile.size() > 0)
//         loadMtl(matFile, &_pMaterial);
// }
} // namespace Chimera