#include "chimera/render/TerrainData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

TerrainData::TerrainData() { setDefaults(); }
TerrainData::~TerrainData() { destroy(); }

void TerrainData::setDefaults() {
    sizeHeight = 0;
    sizeP1 = 0;
    sizeD2 = 0.0f;
    Min = Max = glm::vec3(0.0f);
    // Heights = NULL;
}

void TerrainData::destroy() {
    if (heights.size() > 0)
        heights.clear();

    if (vertices.size() > 0)
        vertices.clear();

    if (indices.size() > 0)
        indices.clear();

    setDefaults();
}

int TerrainData::getIndex(int X, int Z) { return sizeP1 * Z + X; }

float TerrainData::getHeight(int X, int Z) {

    int x1 = X < 0 ? 0 : X > sizeHeight ? sizeHeight : X;
    int z1 = Z < 0 ? 0 : Z > sizeHeight ? sizeHeight : Z;
    int pos = getIndex(x1, z1);

    return heights[pos];
}

bool TerrainData::loadBinary(char* FileName) {

    FILE* File = fopen(FileName, (char*)"rb");
    if (File == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        return false;
    }

    int Size;

    if (fread(&Size, sizeof(int), 1, File) != 1 || Size <= 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        fclose(File);
        return false;
    }

    destroy();

    this->sizeHeight = Size;
    sizeP1 = Size + 1;
    sizeD2 = (float)Size / 2.0f;

    int verticesCount = sizeP1 * sizeP1;

    heights.reserve(verticesCount);

    // FIXME: melhorar esta horrivel, carga de vector deve ser direta do arquivo
    float* h = new float[verticesCount];
    size_t tam = fread(h, sizeof(float), verticesCount, File);
    if (tam != verticesCount) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        fclose(File);
        destroy();
        return false;
    }
    for (int i = 0; i < verticesCount; i++) {
        heights.push_back(h[i]);
    }
    delete[] h;

    fclose(File);

    Min.x = Min.z = -sizeD2;
    Max.x = Max.z = sizeD2;

    Min.y = Max.y = heights[0];

    for (int i = 1; i < verticesCount; i++) {
        if (heights[i] < Min.y)
            Min.y = heights[i];
        if (heights[i] > Max.y)
            Max.y = heights[i];
    }

    vertices.reserve(verticesCount);

    int i = 0;

    for (int z = 0; z <= Size; z++) {
        for (int x = 0; x <= Size; x++) {
            glm::vec3 pos = glm::vec3((float)x - sizeD2,  // posx
                                      heights[i],         // posx
                                      sizeD2 - (float)z); // posx

            glm::vec3 nor = glm::normalize(glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z),   // norx
                                                     2.0f,                                        // nory
                                                     getHeight(x, z + 1) - getHeight(x, z - 1))); // norz

            vertices.push_back({pos, nor, glm::vec2(0.0, 0.0)});
            i++;
        }
    }

    int indicesCount = Size * Size * 2 * 3;
    indices.reserve(indicesCount);

    i = 0;

    for (int z = 0; z < Size; z++) {
        for (int x = 0; x < Size; x++) {

            indices.push_back(getIndex(x, z));
            indices.push_back(getIndex(x + 1, z));
            indices.push_back(getIndex(x + 1, z + 1));

            indices.push_back(getIndex(x + 1, z + 1));
            indices.push_back(getIndex(x, z + 1));
            indices.push_back(getIndex(x, z));
        }
    }

    return true;
}

} // namespace Chimera