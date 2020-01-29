#include "chimera/render/LoadHeightMap.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

LoadHeightMap::LoadHeightMap() : pImage(nullptr) { clean(); }

LoadHeightMap::~LoadHeightMap() { clean(); }

void LoadHeightMap::clean() {
    scale = glm::vec3(1.0f);
    if (pImage != nullptr) {
        SDL_FreeSurface(pImage);
        pImage = nullptr;
    }
}

uint32_t LoadHeightMap::getHeight(int w, int h) {
    unsigned w1 = w < 0 ? 0 : w > pImage->w ? pImage->w : w;
    unsigned h1 = h < 0 ? 0 : h > pImage->h ? pImage->h : h;
    return getpixel(w1, h1);
}

Uint32 LoadHeightMap::getpixel(const unsigned& w, const unsigned& h) {
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

void LoadHeightMap::defineScale(const glm::vec3& _size) {
    uint32_t min, max;
    min = max = getHeight(0, 0);
    for (int z = 0; z < pImage->h; z++) {
        for (int x = 0; x < pImage->w; x++) {
            uint32_t val = getHeight(x, z);

            if (val > max)
                max = val;
            if (val < min)
                min = val;
        }
    }

    scale.x = _size.x / (float)pImage->w;
    scale.y = _size.y / (float)(max - min);
    scale.z = _size.z / (float)pImage->h;
}

glm::vec3 LoadHeightMap::calcNormalHeight(int x, int z) {

    // [6][7][8] [x-1, y+1] [x,y+1] [x+1, y+1]
    // [3][4][5] [x-1, y]   [x,y]   [x+1, y]
    // [0][1][2] [x-1, y-1] [x, y-1][x+1, y-1]
    // Then,
    // float s[9];
    // s[0] = getHeight(x - 1, z - 1);
    // s[1] = getHeight(x, z - 1);
    // s[2] = getHeight(x + 1, z - 1);
    // s[3] = getHeight(x - 1, z);
    // s[4] = getHeight(x, z);
    // s[5] = getHeight(x + 1, z);
    // s[6] = getHeight(x - 1, z + 1);
    // s[7] = getHeight(x, z + 1);
    // s[8] = getHeight(x + 1, z + 1);
    // return glm::normalize(glm::vec3(-(s[2] - s[0] + 2 * (s[5] - s[3]) + s[8] - s[6]),   // x
    //                                 1.0f,                                               // y
    //                                 -(s[6] - s[0] + 2 * (s[7] - s[1]) + s[8] - s[2]))); // z

    return glm::normalize(glm::vec3((scale.y * getHeight(x - 1, z)) - (scale.y * getHeight(x + 1, z)),   // norx
                                    2.0f,                                                                // nory
                                    (scale.y * getHeight(x, z - 1)) - (scale.y * getHeight(x, z + 1)))); // norz
}

bool LoadHeightMap::getMesh(const std::string& _fileName, MeshData& _mesh, const glm::vec3& _size) {

    pImage = IMG_Load(_fileName.c_str());
    if (pImage == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", _fileName.c_str());
        return false;
    }

    float halfH = (float)pImage->h / 2.0f;
    float haldW = (float)pImage->w / 2.0f;

    float v = 1.0f / (pImage->h - 1);
    float u = 1.0f / (pImage->w - 1);

    defineScale(_size);

    for (int z = 0; z < pImage->h; z++) {
        for (int x = 0; x < pImage->w; x++) {

            glm::vec3 pos = glm::vec3(scale.x * ((float)x - haldW),  // posx
                                      scale.y * getHeight(x, z),     // posy
                                      scale.z * (halfH - (float)z)); // posz

            glm::vec3 nor = calcNormalHeight(x, z);

            float v1 = v * z;
            float u1 = u * x;

            _mesh.addVertex(pos);
            _mesh.addNormal(nor);
            _mesh.addUV(glm::vec2(u1, v1));
        }
    }

    int tot_h = pImage->h - 1;
    int tot_w = pImage->w - 1;

    for (int z = 0; z < tot_h; z++) {
        for (int x = 0; x < tot_w; x++) {
            // triangles point
            int pa = getIndex(x, z);
            int pb = getIndex(x + 1, z);
            int pc = getIndex(x + 1, z + 1);
            int pd = getIndex(x, z + 1);
            // Vertex T1
            _mesh.addVertexIndex(pa);
            _mesh.addVertexIndex(pb);
            _mesh.addVertexIndex(pc);
            // Vertex T2
            _mesh.addVertexIndex(pc);
            _mesh.addVertexIndex(pd);
            _mesh.addVertexIndex(pa);
            // normal T1
            _mesh.addNormalIndex(pa);
            _mesh.addNormalIndex(pb);
            _mesh.addNormalIndex(pc);
            // normal T2
            _mesh.addNormalIndex(pc);
            _mesh.addNormalIndex(pd);
            _mesh.addNormalIndex(pa);
            // UV T1
            _mesh.addUVIndex(pa);
            _mesh.addUVIndex(pb);
            _mesh.addUVIndex(pc);
            // UV T2
            _mesh.addUVIndex(pc);
            _mesh.addUVIndex(pd);
            _mesh.addUVIndex(pa);
        }
    }

    _mesh.debugDados(true);
    _mesh.setSingleIndex(true);

    return true;
}
} // namespace Chimera
