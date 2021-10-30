#include "chimera/render/partition/LoadObj.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/TextureManager.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

void eraseAllSubStr(std::string& mainStr, const std::string& toErase) {
    size_t pos = std::string::npos;
    while ((pos = mainStr.find(toErase)) != std::string::npos) {
        mainStr.erase(pos, toErase.length());
    }
}

bool getValidData(std::string& nova, const std::string& comando) {
    std::string fim_linha("\n");
    std::size_t found = nova.find(comando);
    if (found != std::string::npos) {
        eraseAllSubStr(nova, comando);
        eraseAllSubStr(nova, fim_linha);
        return true;
    }
    return false;
}

int getMaterialFile(const std::string materialFile, Material* material) {
    if (materialFile.size() == 0) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Sem arquivo de material");
        return 1;
    }

    FILE* fp = fopen(materialFile.c_str(), "r");
    if (fp == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", materialFile.c_str());
        return -2;
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
                    material->setAmbient(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    material->setAmbient(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) + " material invalido arquivo: " + materialFile);
                }

            } else if (line[1] == 'd') {
                int n = sscanf(line, "Kd %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    material->setDiffuse(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    material->setDiffuse(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) + " material invalido arquivo: " + materialFile);
                }
            } else if (line[1] == 's') {
                int n = sscanf(line, "Ks %f %f %f %f", &r, &g, &b, &a);
                if (n == 3) {
                    material->setSpecular(glm::vec4(r, g, b, 1.0f));
                } else if (n == 4) {
                    material->setSpecular(glm::vec4(r, g, b, a));
                } else {
                    throw Exception("linha " + std::to_string(pos_linha) + " material invalido arquivo:" + materialFile);
                }
            }
        } else {
            // load texture
            std::string nova(line);
            if (getValidData(nova, std::string("map_Kd ")) == true) {
                TextureManager::loadFromFile(nova, nova, TexParam());
                material->addTexture(SHADE_TEXTURE_DIFFUSE, TextureManager::getLast());
            } else if (getValidData(nova, std::string("sharpness ")) == true) {
                float val;
                int n = sscanf(nova.c_str(), "%f", &val);
                material->setShine(val);
            }
        }
    }
    return 0;
}

/**
 * @brief Carrega Mesh e material
 *
 * @param pathFile arquivo com o mesh e material
 * @param mesh  ponteiro do mesh a ser carregado com dados
 * @param material ponteiro do material a ser carregado com dados
 * @return int 0: ok; 1: Sem material; -1: arquivo nao existe; -2 arquivo de material existe mas nao encontrado
 */
int loadObjFile(const std::string& pathFile, MeshData* mesh, Material* material) {

    int resultOpp = 0;

    FILE* fp = fopen(pathFile.c_str(), "r");
    if (fp == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", pathFile.c_str());
        return -1;
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
                    mesh->vertexList.push_back(glm::vec3(x, y, z));
                    continue;
                }
                throw Exception(std::string("linha " + std::to_string(pos_linha) + " parse invalido arquivo: " + pathFile));

            } else if (line[1] == 'n') {
                normalOn = true;
                int n = sscanf(line, "vn %f %f %f", &x, &y, &z);
                mesh->normalList.push_back(glm::vec3(x, y, z));
            } else if (line[1] == 't') {
                textuaOn = true;
                int n = sscanf(line, "vt %f %f", &u, &v);
                mesh->uvList.push_back(glm::vec2(u, v));
            }
        } else if (line[0] == 'f') {
            if (line[1] == ' ') {
                if ((textuaOn == true) && (normalOn == true)) {
                    int n = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &A[0], &B[0], &C[0], &A[1], &B[1], &C[1], &A[2], &B[2], &C[2]);
                } else if ((textuaOn == false) && (normalOn == true)) {
                    int n = sscanf(line, "f %d//%d %d//%d %d//%d", &A[0], &C[0], &A[1], &C[1], &A[2], &C[2]);
                } else if ((textuaOn == true) && (normalOn == false)) {
                    int n = sscanf(line, "f %d/%d %d/%d %d/%d", &A[0], &B[0], &A[1], &B[1], &A[2], &B[2]);
                } else {
                    int n = sscanf(line, "f %d %d %d", &A[0], &A[1], &A[2]);
                }

                for (int indice = 0; indice < 3; indice++) {
                    mesh->vertexIndex.push_back(A[indice] - 1);

                    if (textuaOn == true)
                        mesh->uvIndex.push_back(B[indice] - 1);

                    if (normalOn == true)
                        mesh->normalIndex.push_back(C[indice] - 1);
                }
            }
        } else {
            std::string nova(line);
            if (getValidData(nova, std::string("mtllib ")) == true) {
                resultOpp = getMaterialFile(nova, material);
            }
        }
    }
    fclose(fp);
    if (line)
        free(line);

    mesh->singleIndex = false;
    return resultOpp;
}
} // namespace Chimera