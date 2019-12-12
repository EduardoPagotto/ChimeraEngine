#ifndef __CHIMERA_LOAD_HEIGHTMAP__HPP
#define __CHIMERA_LOAD_HEIGHTMAP__HPP

#include "chimera/render/MeshData.hpp"
#include <string>

namespace Chimera {

class LoadHeightMap {
  public:
    LoadHeightMap(const std::string& _fileName);
    virtual ~LoadHeightMap();

    bool getMesh(MeshData& _mesh);

  private:
    int getIndex(int X, int Z);
    float getHeight(int X, int Z);

    int sizeHeight;
    int sizeP1;
    float* heights;

    std::string fileName;
};

} // namespace Chimera

#endif