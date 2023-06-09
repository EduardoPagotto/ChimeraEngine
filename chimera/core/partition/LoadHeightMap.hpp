#pragma once
#include "chimera/core/space/Triangle.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include <SDL2/SDL_image.h>
#include <string>

namespace Chimera {

class LoadHeightMap {
  public:
    LoadHeightMap(int _squareX, int _squareZ);
    virtual ~LoadHeightMap();
    void clean();

    bool getMesh(const std::string& _fileName, Mesh& _mesh, const glm::vec3& _size);
    void split(std::vector<uint32_t>& vertexIndexIn, std::vector<TrisIndex>& vTrisIndexOut);

  private:
    inline uint32_t getIndex(const uint32_t& _x, const uint32_t& _z) { return (pImage->w * _z) + _x; }
    uint32_t getpixel(const uint32_t& w, const uint32_t& h);
    uint32_t getHeight(const uint32_t& w, const uint32_t& h);
    glm::vec3 defineScale(const glm::vec3& _size);
    // glm::vec3 calcNormalHeight(int x, int z);

    SDL_Surface* pImage;
    uint32_t width;
    uint32_t height;
    uint32_t squareX;
    uint32_t squareZ;

    uint32_t minimal;
};
} // namespace Chimera
