#ifndef __CHIMERA_LOAD_HEIGHTMAP__HPP
#define __CHIMERA_LOAD_HEIGHTMAP__HPP

#include "chimera/core/VertexData.hpp"
#include "chimera/render/vbs/RenderableIBO.hpp"
#include <SDL2/SDL_image.h>
#include <string>

namespace Chimera {

class LoadHeightMap {
  public:
    LoadHeightMap(int _squareX, int _squareZ);
    virtual ~LoadHeightMap();
    void clean();

    bool getMesh(const std::string& _fileName, MeshData& _mesh, const glm::vec3& _size);
    void split(std::vector<unsigned int> _vVertexIndex);
    std::vector<RenderableIBO*> vNodes;

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

#endif