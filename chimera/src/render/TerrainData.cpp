#include "chimera/render/TerrainData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

TerrainData::TerrainData() { setDefaults(); }
TerrainData::~TerrainData() { destroy(); }

void TerrainData::setDefaults() {
    sizeHeight = 0;
    sizeP1 = 0;
    heights = nullptr;
}

void TerrainData::destroy() {

    if (heights != nullptr)
        delete[] heights;

    setDefaults();
}

bool TerrainData::loadBinary(char* fileName, MeshData& _mesh) {

    FILE* file = fopen(fileName, (char*)"rb");
    if (file == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName);
        return false;
    }

    int size;
    if (fread(&size, sizeof(int), 1, file) != 1 || size <= 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName);
        fclose(file);
        return false;
    }

    destroy();

    this->sizeHeight = size;
    sizeP1 = size + 1;
    float sizeD2 = (float)size / 2.0f;

    int verticesCount = sizeP1 * sizeP1;

    heights = new float[verticesCount];

    size_t tam = fread(heights, sizeof(float), verticesCount, file);
    if (tam != verticesCount) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName);
        fclose(file);
        destroy();
        return false;
    }

    fclose(file);

    int i = 0;
    for (int z = 0; z <= size; z++) {
        for (int x = 0; x <= size; x++) {
            glm::vec3 pos = glm::vec3((float)x - sizeD2,  // posx
                                      heights[i],         // posx
                                      sizeD2 - (float)z); // posx

            glm::vec3 nor = glm::normalize(glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z),   // norx
                                                     2.0f,                                        // nory
                                                     getHeight(x, z + 1) - getHeight(x, z - 1))); // norz

            _mesh.addVertex(pos);
            _mesh.addNormal(nor);
            //_mesh.addUV(glm::vec2(0.0, 0.0));
            i++;
        }
    }

    i = 0;
    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            // triangles point
            int pa = getIndex(x, z);
            int pb = getIndex(x + 1, z);
            int pc = getIndex(x + 1, z + 1);
            int pd = getIndex(x, z + 1);
            // vertex
            _mesh.addVertexIndex(pa); // T1
            _mesh.addVertexIndex(pb);
            _mesh.addVertexIndex(pc);

            _mesh.addVertexIndex(pc); // T2
            _mesh.addVertexIndex(pd);
            _mesh.addVertexIndex(pa);
            // normal
            _mesh.addNormalIndex(pa); // T1
            _mesh.addNormalIndex(pb);
            _mesh.addNormalIndex(pc);

            _mesh.addNormalIndex(pc); // T2
            _mesh.addNormalIndex(pd);
            _mesh.addNormalIndex(pa);
            // // texture
            // _mesh.addUVIndex(pa); // T1
            // _mesh.addUVIndex(pb);
            // _mesh.addUVIndex(pc);

            // _mesh.addUVIndex(pc); // T2
            // _mesh.addUVIndex(pd);
            // _mesh.addUVIndex(pa);
        }
    }
    _mesh.setOneIndex(true);
    return true;
}

int TerrainData::getIndex(int X, int Z) { return sizeP1 * Z + X; }

float TerrainData::getHeight(int X, int Z) {

    int x1 = X < 0 ? 0 : X > sizeHeight ? sizeHeight : X;
    int z1 = Z < 0 ? 0 : Z > sizeHeight ? sizeHeight : Z;
    int pos = getIndex(x1, z1);

    return heights[pos];
}
} // namespace Chimera