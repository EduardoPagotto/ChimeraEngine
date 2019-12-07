#include "chimera/render/TerrainData.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

TerrainData::TerrainData() { setDefaults(); }
TerrainData::~TerrainData() { destroy(); }

void TerrainData::setDefaults() {
    sizeHeight = 0;
    sizeP1 = 0;
    sizeD2 = 0.0f;
    heights = nullptr;
}

void TerrainData::destroy() {

    if (heights != nullptr)
        delete[] heights;

    setDefaults();
}

bool TerrainData::loadBinary(char* FileName, MeshData& _mesh) {

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

    verticesCount = sizeP1 * sizeP1;

    heights = new float[verticesCount];

    size_t tam = fread(heights, sizeof(float), verticesCount, File);
    if (tam != verticesCount) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", FileName);
        fclose(File);
        destroy();
        return false;
    }

    fclose(File);

    int i = 0;
    for (int z = 0; z <= Size; z++) {
        for (int x = 0; x <= Size; x++) {
            glm::vec3 pos = glm::vec3((float)x - sizeD2,  // posx
                                      heights[i],         // posx
                                      sizeD2 - (float)z); // posx

            glm::vec3 nor = glm::normalize(glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z),   // norx
                                                     2.0f,                                        // nory
                                                     getHeight(x, z + 1) - getHeight(x, z - 1))); // norz

            _mesh.addVertice(pos);
            _mesh.addNormal(nor);
            //_mesh.addUV(glm::vec2(0.0, 0.0));
            i++;
        }
    }

    i = 0;
    for (int z = 0; z < Size; z++) {
        for (int x = 0; x < Size; x++) {
            // triangles point
            int pa = getIndex(x, z);
            int pb = getIndex(x + 1, z);
            int pc = getIndex(x + 1, z + 1);
            int pd = getIndex(x, z + 1);
            // vertex
            _mesh.addVerticeIndex(pa); // T1
            _mesh.addVerticeIndex(pb);
            _mesh.addVerticeIndex(pc);

            _mesh.addVerticeIndex(pc); // T2
            _mesh.addVerticeIndex(pd);
            _mesh.addVerticeIndex(pa);
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