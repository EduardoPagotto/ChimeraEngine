#include "chimera/render/partition/LoadHeightMap.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

LoadHeightMap::LoadHeightMap(int _squareX, int _squareZ) : pImage(nullptr), squareX(_squareX), squareZ(_squareZ) { clean(); }

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

            _mesh.vertexList.push_back(glm::vec3(x - haldW, getHeight(x, z), halfH - z) * scale);
            _mesh.normalList.push_back(calcNormalHeight(x, z));
            _mesh.uvList.push_back(glm::vec2(u * x, v * z));
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
            _mesh.vertexIndex.push_back(pa);
            _mesh.vertexIndex.push_back(pb);
            _mesh.vertexIndex.push_back(pc);
            // Vertex T2
            _mesh.vertexIndex.push_back(pc);
            _mesh.vertexIndex.push_back(pd);
            _mesh.vertexIndex.push_back(pa);
            // normal T1
            _mesh.normalIndex.push_back(pa);
            _mesh.normalIndex.push_back(pb);
            _mesh.normalIndex.push_back(pc);
            // normal T2
            _mesh.normalIndex.push_back(pc);
            _mesh.normalIndex.push_back(pd);
            _mesh.normalIndex.push_back(pa);
            // UV T1
            _mesh.uvIndex.push_back(pa);
            _mesh.uvIndex.push_back(pb);
            _mesh.uvIndex.push_back(pc);
            // UV T2
            _mesh.uvIndex.push_back(pc);
            _mesh.uvIndex.push_back(pd);
            _mesh.uvIndex.push_back(pa);
        }
    }

    _mesh.singleIndex = true;
    vertexDataMeshDataDebug(&_mesh, false);

    this->width = pImage->w;
    this->height = pImage->h;

    return true;
}

void LoadHeightMap::split(std::vector<unsigned int> _vVertexIndex) {

    bool done = false;
    uint32_t startHeight = 0;
    uint32_t startWidth = 0;
    uint32_t contador = 0;
    uint32_t totalHeight = (height - 1) * 2;
    uint32_t totalWidth = (width - 1) * 2;
    uint32_t squareHeight = squareZ;
    uint32_t squareWidth = squareX * 2;
    uint32_t totfaces = _vVertexIndex.size() / 3;
    uint32_t thresholdWidht = totalHeight * squareZ;

    while (!done) {

        uint32_t endHeight = startHeight + squareHeight;
        uint32_t endWidth = startWidth + squareWidth;
        uint32_t testeA = startHeight * totalHeight + startWidth;

        if (testeA >= totfaces) {
            done = true;
            continue;
        }

        if (endHeight > (height - 1))
            endHeight = (height - 1);

        if (endWidth > totalWidth)
            endWidth = totalWidth;

        Renderable3D* pNode = new Renderable3D;

        uint32_t face, base;
        for (uint32_t h = startHeight; h < endHeight; h++) {   // z
            for (uint32_t w = startWidth; w < endWidth; w++) { // x
                face = ((h * totalHeight) + w);
                base = face * 3;
                pNode->addTris(_vVertexIndex[base], _vVertexIndex[base + 1], _vVertexIndex[base + 2]);
                contador++;
            }
        }

        if (contador >= thresholdWidht) {
            startHeight = endHeight;
            contador = 0;
            startWidth = 0;
        } else {
            startWidth = endWidth;
        }

        if (pNode->empty() == true) {
            delete pNode;
            pNode = nullptr;
            done = true;
            // SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "shit!!!");
            continue;
        }

        vNodes.push_back(pNode);
        // pNode->debugDados();
    }
}

} // namespace Chimera
