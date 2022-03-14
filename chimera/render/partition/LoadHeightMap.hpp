#pragma once
#include "chimera/core/space/TrisIndex.hpp"
#include "chimera/render/VertexData.hpp"
#include <SDL2/SDL_image.h>
#include <string>

namespace Chimera {

class LoadHeightMap {
  public:
    LoadHeightMap(int _squareX, int _squareZ);
    virtual ~LoadHeightMap();
    void clean();

    bool getMesh(const std::string& _fileName, Mesh& _mesh, const glm::vec3& _size);
    void split(std::vector<unsigned int> _vVertexIndex);
    VecPrtTrisIndex vNodes;

  private:
    inline unsigned getIndex(const int& _x, const int& _z) { return (pImage->w * _z) + _x; }
    uint32_t getHeight(int w, int h);
    void defineScale(const glm::vec3& _size);
    Uint32 getpixel(const unsigned& w, const unsigned& h);
    glm::vec3 calcNormalHeight(int x, int z);

    SDL_Surface* pImage;
    glm::vec3 scale;
    int width;
    int height;
    int squareX;
    int squareZ;
};
} // namespace Chimera
