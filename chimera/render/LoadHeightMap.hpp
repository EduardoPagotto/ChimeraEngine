#ifndef __CHIMERA_LOAD_HEIGHTMAP__HPP
#define __CHIMERA_LOAD_HEIGHTMAP__HPP

#include "chimera/render/MeshData.hpp"
#include <SDL2/SDL_image.h>
#include <string>

namespace Chimera {

class LoadHeightMap {
  public:
    LoadHeightMap(const std::string& _fileName);
    virtual ~LoadHeightMap();

    bool getMesh(MeshData& _mesh);
    bool getMesh2(MeshData& _mesh);

  private:
    int getIndex(int X, int Z);
    float getHeight(int X, int Z);

    float getHeight2(int x, int y);
    int invert_image(int pitch, int height, void* image_pixels);
    Uint32 getpixel(int x, int y);

    SDL_Surface* pImage;

    int sizeHeight;
    int sizeP1;
    float* heights;

    std::string fileName;
};

} // namespace Chimera

#endif