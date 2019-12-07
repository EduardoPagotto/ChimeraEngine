#ifndef __CHIMERA_TERRAIN_DATA__HPP
#define __CHIMERA_TERRAIN_DATA__HPP

#include "chimera/render/MeshData.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class TerrainData {
  public:
    TerrainData();
    virtual ~TerrainData();
    void destroy();
    bool loadBinary(char* fileName, MeshData& _mesh);
    void setDefaults();

  private:
    int getIndex(int X, int Z);
    float getHeight(int X, int Z);

    int sizeHeight;
    int sizeP1;
    float* heights;
};

} // namespace Chimera

#endif