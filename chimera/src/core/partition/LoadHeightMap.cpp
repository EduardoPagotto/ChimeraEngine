#include "chimera/core/partition/LoadHeightMap.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace ce {

    LoadHeightMap::LoadHeightMap(int _squareX, int _squareZ) : pImage(nullptr), squareX(_squareX), squareZ(_squareZ) {
        clean();
    }

    LoadHeightMap::~LoadHeightMap() { clean(); }

    void LoadHeightMap::clean() {
        if (pImage != nullptr) {
            SDL_DestroySurface(pImage);
            pImage = nullptr;
        }
    }

    uint32_t LoadHeightMap::getHeight(const uint32_t& w, const uint32_t& h) {
        const uint32_t w1{w > pImage->w ? pImage->w : w};
        const uint32_t h1{h > pImage->h ? pImage->h : h};
        return getpixel(w1, h1);
    }

    uint32_t LoadHeightMap::getpixel(const uint32_t& w, const uint32_t& h) {

        const SDL_PixelFormatDetails* detail = SDL_GetPixelFormatDetails(pImage->format);
        const int bpp{detail->bytes_per_pixel};

        uint8_t* p{(uint8_t*)pImage->pixels + h * pImage->pitch + w * bpp};

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
        for (uint32_t z{0}; z < pImage->h; z++) {
            for (uint32_t x{0}; x < pImage->w; x++) {
                const uint32_t val{getHeight(x, z)};

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

        const float halfH{(float)pImage->h / 2.0f};
        const float haldW{(float)pImage->w / 2.0f};
        const float v{1.0f / (pImage->h - 1)};
        const float u{1.0f / (pImage->w - 1)};
        const glm::vec3 scale = defineScale(_size);

        for (uint32_t z = 0; z < pImage->h; z++) {
            for (uint32_t x = 0; x < pImage->w; x++) {

                _mesh.vertex.push_back({glm::vec3(x - haldW, getHeight(x, z) - minimal, halfH - z) * scale, // point
                                        glm::vec3(0.0f),                                                    // normal
                                        glm::vec2(u * x, v * z)});                                          // uv TEX
            }
        }

        const uint32_t tot_h{static_cast<uint32_t>(pImage->h - 1)};
        const uint32_t tot_w{static_cast<uint32_t>(pImage->w - 1)};

        for (uint32_t z{0}; z < tot_h; z++) {
            for (uint32_t x{0}; x < tot_w; x++) {
                // triangles point
                const uint32_t pa{getIndex(x, z)};
                const uint32_t pb{getIndex(x + 1, z)};
                const uint32_t pc{getIndex(x + 1, z + 1)};
                const uint32_t pd{getIndex(x, z + 1)};
                // Face index
                _mesh.iFace.push_back({pa, pb, pc}); // T1
                _mesh.iFace.push_back({pc, pd, pa}); // T2
            }
        }

        // Calcula normal apos todo o mapeamento de altura
        for (uint32_t i{0}; i < _mesh.iFace.size(); i++) {

            const glm::vec3& pa = _mesh.vertex[_mesh.iFace[i].x].point;
            const glm::vec3& pb = _mesh.vertex[_mesh.iFace[i].y].point;
            const glm::vec3& pc = _mesh.vertex[_mesh.iFace[i].z].point;
            const glm::vec3 vn = glm::normalize(glm::cross(pb - pa, pc - pa)); // CROSS(U,V)

            _mesh.vertex[_mesh.iFace[i].x].normal = vn;
            _mesh.vertex[_mesh.iFace[i].y].normal = vn;
            _mesh.vertex[_mesh.iFace[i].z].normal = vn;
        }

        meshDebug(_mesh, false);

        this->width = pImage->w;
        this->height = pImage->h;

        return true;
    }

    void LoadHeightMap::split(TrisIndex& vertexIndexIn, std::vector<TrisIndex>& vTrisIndexOut) {

        bool done{false};
        uint32_t startHeight{0}, startWidth{0}, contador{0};
        const uint32_t totalHeight{(height - 1) * 2}, totalWidth{(width - 1) * 2};
        const uint32_t squareHeight{squareZ}, squareWidth{squareX * 2};
        const uint32_t thresholdWidht{totalHeight * squareZ};

        while (!done) {

            uint32_t endHeight = startHeight + squareHeight;
            uint32_t endWidth = startWidth + squareWidth;
            const uint32_t testeA = startHeight * totalHeight + startWidth;

            if (testeA >= vertexIndexIn.size()) { // all faces
                done = true;
                continue;
            }

            if (endHeight > (height - 1))
                endHeight = (height - 1);

            if (endWidth > totalWidth)
                endWidth = totalWidth;

            TrisIndex node;

            uint32_t face;                                         //, base;
            for (uint32_t h = startHeight; h < endHeight; h++) {   // z
                for (uint32_t w = startWidth; w < endWidth; w++) { // x
                    face = ((h * totalHeight) + w);
                    // base = face * 3;
                    node.push_back(glm::uvec3(vertexIndexIn[face].x, vertexIndexIn[face].y, vertexIndexIn[face].z));
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

} // namespace ce
