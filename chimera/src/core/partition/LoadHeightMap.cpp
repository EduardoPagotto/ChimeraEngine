#include "chimera/core/partition/LoadHeightMap.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

LoadHeightMap::LoadHeightMap(int _squareX, int _squareZ) : pImage(nullptr), squareX(_squareX), squareZ(_squareZ) { clean(); }

LoadHeightMap::~LoadHeightMap() { clean(); }

void LoadHeightMap::clean() {
    if (pImage != nullptr) {
        SDL_FreeSurface(pImage);
        pImage = nullptr;
    }
}

uint32_t LoadHeightMap::getHeight(const uint32_t& w, const uint32_t& h) {
    unsigned w1 = w > pImage->w ? pImage->w : w;
    unsigned h1 = h > pImage->h ? pImage->h : h;
    return getpixel(w1, h1);
}

uint32_t LoadHeightMap::getpixel(const uint32_t& w, const uint32_t& h) {
    int bpp = pImage->format->BytesPerPixel;

    uint8_t* p = (uint8_t*)pImage->pixels + h * pImage->pitch + w * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(uint16_t*)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(uint32_t*)p;
            break;

        default:
            return 0; /* shouldn't happen, but avoids warnings */
    }
}

glm::vec3 LoadHeightMap::defineScale(const glm::vec3& _size) {
    uint32_t max;
    minimal = max = getHeight(0, 0);
    for (uint32_t z = 0; z < pImage->h; z++) {
        for (uint32_t x = 0; x < pImage->w; x++) {
            uint32_t val = getHeight(x, z);

            if (val > max)
                max = val;
            if (val < minimal)
                minimal = val;
        }
    }
    return glm::vec3(_size.x / (float)pImage->w, _size.y / (float)(max - minimal), _size.z / (float)pImage->h);
}

// glm::vec3 LoadHeightMap::calcNormalHeight(uint32_t x, uint32_t z) {
//     return glm::normalize(glm::vec3((scale.y * getHeight(x - 1, z)) - (scale.y * getHeight(x + 1, z)),   // norx
//                                     2.0f,                                                                // nory
//                                     (scale.y * getHeight(x, z - 1)) - (scale.y * getHeight(x, z + 1)))); // norz
// }

bool LoadHeightMap::getMesh(const std::string& _fileName, Mesh& _mesh, const glm::vec3& _size) {

    pImage = IMG_Load(_fileName.c_str());
    if (pImage == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error opening file : %s", _fileName.c_str());
        return false;
    }

    float halfH = (float)pImage->h / 2.0f;
    float haldW = (float)pImage->w / 2.0f;

    float v = 1.0f / (pImage->h - 1);
    float u = 1.0f / (pImage->w - 1);

    glm::vec3 scale = defineScale(_size);

    for (uint32_t z = 0; z < pImage->h; z++) {
        for (uint32_t x = 0; x < pImage->w; x++) {

            _mesh.point.push_back(glm::vec3(x - haldW, getHeight(x, z) - minimal, halfH - z) * scale);
            _mesh.normal.push_back(glm::vec3(0.0f));
            _mesh.uv.push_back(glm::vec2(u * x, v * z));
        }
    }

    uint32_t tot_h = pImage->h - 1;
    uint32_t tot_w = pImage->w - 1;

    for (uint32_t z = 0; z < tot_h; z++) {
        for (uint32_t x = 0; x < tot_w; x++) {
            // triangles point
            uint32_t pa = getIndex(x, z);
            uint32_t pb = getIndex(x + 1, z);
            uint32_t pc = getIndex(x + 1, z + 1);
            uint32_t pd = getIndex(x, z + 1);
            // Vertex T1
            _mesh.iPoint.push_back(pa);
            _mesh.iPoint.push_back(pb);
            _mesh.iPoint.push_back(pc);
            // Vertex T2
            _mesh.iPoint.push_back(pc);
            _mesh.iPoint.push_back(pd);
            _mesh.iPoint.push_back(pa);
            // normal T1
            _mesh.iNormal.push_back(pa);
            _mesh.iNormal.push_back(pb);
            _mesh.iNormal.push_back(pc);
            // normal T2
            _mesh.iNormal.push_back(pc);
            _mesh.iNormal.push_back(pd);
            _mesh.iNormal.push_back(pa);
            // UV T1
            _mesh.iUv.push_back(pa);
            _mesh.iUv.push_back(pb);
            _mesh.iUv.push_back(pc);
            // UV T2
            _mesh.iUv.push_back(pc);
            _mesh.iUv.push_back(pd);
            _mesh.iUv.push_back(pa);
        }
    }

    // Calcula normal apos todo o mapeamento de altura
    for (uint32_t i = 0; i < _mesh.iPoint.size(); i += 3) {

        glm::vec3 pa = _mesh.point[_mesh.iPoint[i]];
        glm::vec3 pb = _mesh.point[_mesh.iPoint[i + 1]];
        glm::vec3 pc = _mesh.point[_mesh.iPoint[i + 2]];
        glm::vec3 vn = glm::normalize(glm::cross(pb - pa, pc - pa)); // CROSS(U,V)

        _mesh.normal[_mesh.iNormal[i]] = vn;
        _mesh.normal[_mesh.iNormal[i + 1]] = vn;
        _mesh.normal[_mesh.iNormal[i + 2]] = vn;
    }

    _mesh.serialized = true;
    meshDebug(&_mesh, false);

    this->width = pImage->w;
    this->height = pImage->h;

    return true;
}

void LoadHeightMap::split(std::vector<uint32_t>& vertexIndexIn, std::vector<TrisIndex>& vTrisIndexOut) {

    bool done = false;
    uint32_t startHeight = 0;
    uint32_t startWidth = 0;
    uint32_t contador = 0;
    uint32_t totalHeight = (height - 1) * 2;
    uint32_t totalWidth = (width - 1) * 2;
    uint32_t squareHeight = squareZ;
    uint32_t squareWidth = squareX * 2;
    uint32_t totfaces = vertexIndexIn.size() / 3;
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

        TrisIndex node;

        uint32_t face, base;
        for (uint32_t h = startHeight; h < endHeight; h++) {   // z
            for (uint32_t w = startWidth; w < endWidth; w++) { // x
                face = ((h * totalHeight) + w);
                base = face * 3;
                node.push_back(glm::uvec3(vertexIndexIn[base], vertexIndexIn[base + 1], vertexIndexIn[base + 2]));
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

        if (node.size() != 0)
            vTrisIndexOut.push_back(node);
        else
            done = true;

        // node.debugDados();
    }
}

} // namespace Chimera
