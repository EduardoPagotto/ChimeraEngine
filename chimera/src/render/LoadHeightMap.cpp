#include "chimera/render/LoadHeightMap.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

LoadHeightMap::LoadHeightMap(const std::string& _fileName) {
    fileName = _fileName;
    sizeHeight = 0;
    sizeP1 = 0;
    heights = nullptr;
}

LoadHeightMap::~LoadHeightMap() {
    if (heights != nullptr)
        delete[] heights;
}

int LoadHeightMap::invert_image(int pitch, int height, void* image_pixels) {
    int index;
    void* temp_row;
    int height_div_2;

    temp_row = (void*)malloc(pitch);
    if (NULL == temp_row) {
        SDL_SetError("Not enough memory for image inversion");
        return -1;
    }
    // if height is odd, don't need to swap middle row
    height_div_2 = (int)(height * .5);
    for (index = 0; index < height_div_2; index++) {
        // uses string.h
        memcpy((Uint8*)temp_row, (Uint8*)(image_pixels) + pitch * index, pitch);

        memcpy((Uint8*)(image_pixels) + pitch * index, (Uint8*)(image_pixels) + pitch * (height - index - 1), pitch);
        memcpy((Uint8*)(image_pixels) + pitch * (height - index - 1), temp_row, pitch);
    }
    free(temp_row);
    return 0;
}

float LoadHeightMap::getHeight2(int w, int h) {

    unsigned int w1 = w < 0 ? 0 : w > pImage->w ? pImage->w : w;
    unsigned int h1 = h < 0 ? 0 : h > pImage->h ? pImage->h : h;

    Uint32 pixelData = getpixel(w1, h1);
    return (float)(pixelData / 100.0f);
}

Uint32 LoadHeightMap::getpixel(const unsigned int& w, const unsigned int& h) {
    int bpp = pImage->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8* p = (Uint8*)pImage->pixels + h * pImage->pitch + w * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16*)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32*)p;
            break;

        default:
            return 0; /* shouldn't happen, but avoids warnings */
    }
}

bool LoadHeightMap::getMesh2(MeshData& _mesh) {

    pImage = IMG_Load(fileName.c_str());
    if (pImage == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName.c_str());
        return false;
    }

    // if (invert_image(pImage->pitch, pImage->h, pImage->pixels) != 0) {
    //     SDL_SetError("Falha na inversao de pixels");
    //     return false;
    // }

    for (int z = 0; z < pImage->h; z++) {
        for (int x = 0; x < pImage->w; x++) {

            glm::vec3 pos = glm::vec3((float)x,         // posx
                                      getHeight2(x, z), // posy
                                      (float)z);        // posz

            // glm::vec3 nor = glm::normalize(glm::vec3(getHeight2(x - 1, z) - getHeight2(x + 1, z),   // norx
            //                                          2.0f,                                          // nory
            //                                          getHeight2(x, z + 1) - getHeight2(x, z - 1))); // norz

            _mesh.addVertex(pos);
            // _mesh.addNormal(nor);
            //_mesh.addUV(glm::vec2(0.0, 0.0));
        }
    }

    for (int z = 0; z < pImage->h; z++) {
        for (int x = 0; x < pImage->w; x++) {
            // triangles point
            int pa = getIndex2(x, z);
            int pb = getIndex2(x + 1, z);
            int pc = getIndex2(x + 1, z + 1);
            int pd = getIndex2(x, z + 1);
            // triamgles T1
            _mesh.addVertexIndex(pa);
            _mesh.addVertexIndex(pb);
            _mesh.addVertexIndex(pc);
            // triamgles T2
            _mesh.addVertexIndex(pc);
            _mesh.addVertexIndex(pd);
            _mesh.addVertexIndex(pa);
        }
    }

    std::vector<glm::vec3> vertices = _mesh.getVertexList();
    std::vector<unsigned int> indices = _mesh.getVertexIndex();
    for (int a = 0; a < indices.size(); a += 3) {
        int b = a + 1;
        int c = b + 1;

        glm::vec3 v1 = vertices[indices[a]];
        glm::vec3 v2 = vertices[indices[b]];
        glm::vec3 v3 = vertices[indices[c]];

        glm::vec3 normal = glm::normalize(glm::cross(v3 - v1, v2 - v1));
        _mesh.addNormal(normal);
        _mesh.addNormal(normal);
        _mesh.addNormal(normal);

        _mesh.addNormalIndex(a);
        _mesh.addNormalIndex(b);
        _mesh.addNormalIndex(c);
    }

    return true;
}

bool LoadHeightMap::getMesh(MeshData& _mesh) {

    FILE* file = fopen(fileName.c_str(), (char*)"rb");
    if (file == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName.c_str());
        return false;
    }

    int size;
    if (fread(&size, sizeof(int), 1, file) != 1 || size <= 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName.c_str());
        fclose(file);
        return false;
    }

    this->sizeHeight = size;
    sizeP1 = size + 1;
    float sizeD2 = (float)size / 2.0f;

    int verticesCount = sizeP1 * sizeP1;

    heights = new float[verticesCount];

    size_t tam = fread(heights, sizeof(float), verticesCount, file);
    if (tam != verticesCount) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", fileName.c_str());
        fclose(file);
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

int LoadHeightMap::getIndex(int X, int Z) { return sizeP1 * Z + X; }

float LoadHeightMap::getHeight(int X, int Z) {

    int x1 = X < 0 ? 0 : X > sizeHeight ? sizeHeight : X;
    int z1 = Z < 0 ? 0 : Z > sizeHeight ? sizeHeight : Z;
    int pos = getIndex(x1, z1);

    return heights[pos];
}
} // namespace Chimera