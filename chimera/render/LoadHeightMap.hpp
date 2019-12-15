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

    bool getMesh(const std::string& _fileName, MeshData& _mesh);
    inline void setMaxHeight(float _maxHeight) { maxHeight = _maxHeight; }

  private:
    void split(MeshData& _mesh);
    inline unsigned int getIndex(const int& _x, const int& _z) { return (pImage->w * _z) + _x; }
    float getHeight(int w, int h);
    Uint32 getpixel(const unsigned int& w, const unsigned int& h);

    SDL_Surface* pImage;
    float maxHeight;
};

} // namespace Chimera

#endif