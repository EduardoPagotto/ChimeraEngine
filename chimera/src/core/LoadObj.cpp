#include "chimera/core/LoadObj.hpp"
#include "chimera/core/Exception.hpp"

#include <SDL2/SDL.h>

namespace Chimera {

void loadObj(const char* _fineName, MeshData* _mesh) {

    FILE* fp = fopen(_fineName, "r");
    if (fp == NULL)
        throw Exception(std::string("Erro ao abrir arquivo:" + std::string(_fineName)));

    bool textuaOn = false;
    bool normalOn = false;
    char* line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, fp)) != -1) {

        float x, y, z, u, v;
        int A[3];
        int B[3];
        int C[3];

        if (line[0] == '#')
            continue;
        else if (line[0] == 'v') {
            if (line[1] == ' ') {
                int n = sscanf(line, "v %f %f %f", &x, &y, &z);
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Vertice (%f %f %f)", x, y, z);
                _mesh->vertexList.push_back(glm::vec3(x, y, z));

            } else if (line[1] == 'n') {
                normalOn = true;
                int n = sscanf(line, "vn %f %f %f", &x, &y, &z);
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Normal (%f %f %f)", x, y, z);
                _mesh->normalList.push_back(glm::vec3(x, y, z));

            } else if (line[1] == 't') {
                textuaOn = true;
                int n = sscanf(line, "vt %f %f", &u, &v);
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Tex (%f %f)", u, v);
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
                    _mesh->vertexIndex.push_back(A[indice]);

                    if (textuaOn == true)
                        _mesh->textureIndex.push_back(B[indice]);

                    if (normalOn == true)
                        _mesh->normalIndex.push_back(C[indice]);
                }
            }
        }
    }
    fclose(fp);
    if (line)
        free(line);
}
} // namespace Chimera