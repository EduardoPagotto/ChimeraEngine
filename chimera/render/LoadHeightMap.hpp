#ifndef __CHIMERA_LOAD_HEIGHTMAP__HPP
#define __CHIMERA_LOAD_HEIGHTMAP__HPP

#include "chimera/render/MeshData.hpp"
#include <SDL2/SDL_image.h>
#include <string>

namespace Chimera {

class LoadHeightMap {
  public:
    LoadHeightMap();
    virtual ~LoadHeightMap();
    void clean();

    bool getMesh(const std::string& _fileName, MeshData& _mesh, float _maxX, float _maxZ, float _maxHeight);
    inline int getHeight() { return pImage->h; }
    inline int getWidth() { return pImage->w; }

  private:
    inline unsigned getIndex(const int& _x, const int& _z) { return (pImage->w * _z) + _x; }
    float getHeight(int w, int h);
    uint32_t getRealHeight(int w, int h);
    void defineMaxHeight(float _maxHeight);
    Uint32 getpixel(const unsigned& w, const unsigned& h);

    SDL_Surface* pImage;
    float percentHeight;
};

} // namespace Chimera

#endif