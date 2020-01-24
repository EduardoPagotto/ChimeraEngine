#include "chimera/render/LoadHeightMap.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

LoadHeightMap::LoadHeightMap() : pImage(nullptr) { clean(); }

LoadHeightMap::~LoadHeightMap() { clean(); }

void LoadHeightMap::clean() {
    maxHeight = 80.0f;
    if (pImage != nullptr) {
        SDL_FreeSurface(pImage);
        pImage = nullptr;
    }
}

uint32_t LoadHeightMap::getRealHeight(int w, int h) {
    unsigned w1 = w < 0 ? 0 : w > pImage->w ? pImage->w : w;
    unsigned h1 = h < 0 ? 0 : h > pImage->h ? pImage->h : h;
    return getpixel(w1, h1);
}

float LoadHeightMap::getHeight(int w, int h) {

    unsigned w1 = w < 0 ? 0 : w > pImage->w ? pImage->w : w;
    unsigned h1 = h < 0 ? 0 : h > pImage->h ? pImage->h : h;

    Uint32 pixelData = getpixel(w1, h1);
    return (float)(pixelData / maxHeight);
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

void LoadHeightMap::defineMinMax(uint32_t& _min, uint32_t& _max) {
    _min = _max = getRealHeight(0, 0);
    for (int z = 0; z < pImage->h; z++) {
        for (int x = 0; x < pImage->w; x++) {
            uint32_t val = getRealHeight(x, z);

            if (val > _max)
                _max = val;
            if (val < _min)
                _min = val;
        }
    }
}

bool LoadHeightMap::getMesh(const std::string& _fileName, MeshData& _mesh) {

    pImage = IMG_Load(_fileName.c_str());
    if (pImage == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", _fileName.c_str());
        return false;
    }

    float halfH = (float)pImage->h / 2.0f;
    float haldW = (float)pImage->w / 2.0f;

    float v = 1.0f / (pImage->h - 1);
    float u = 1.0f / (pImage->w - 1);

    uint32_t min = 0;
    uint32_t max = 0;
    defineMinMax(min, max);

    for (int z = 0; z < pImage->h; z++) {
        for (int x = 0; x < pImage->w; x++) {

            glm::vec3 pos = glm::vec3((float)x - haldW,  // posx
                                      getHeight(x, z),   // posy
                                      halfH - (float)z); // posz

            glm::vec3 nor = glm::normalize(glm::vec3(getHeight(x - 1, z) - getHeight(x + 1, z),   // norx
                                                     2.0f,                                        // nory
                                                     getHeight(x, z + 1) - getHeight(x, z - 1))); // norz

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
