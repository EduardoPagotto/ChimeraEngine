#pragma once
#include "chimera/core/space/Triangle.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include <SDL2/SDL_image.h>
#include <string>

namespace ce {

class LoadHeightMap {
  private:
    SDL_Surface* pImage{nullptr};
    uint32_t width{0}, height{0}, squareX{0}, squareZ{0}, minimal{0};

  public:
    LoadHeightMap(int _squareX, int _squareZ);
    virtual ~LoadHeightMap();
    void clean();

    bool getMesh(const std::string& _fileName, Mesh& _mesh, const glm::vec3& _size);
    void split(TrisIndex& vertexIndexIn, std::vector<TrisIndex>& vTrisIndexOut);

  private:
    inline const uint32_t getIndex(const uint32_t& _x, const uint32_t& _z) { return (pImage->w * _z) + _x; }
    uint32_t getpixel(const uint32_t& w, const uint32_t& h);
    uint32_t getHeight(const uint32_t& w, const uint32_t& h);
    glm::vec3 defineScale(const glm::vec3& _size);
    // glm::vec3 calcNormalHeight(int x, int z);
};
} // namespace ce
