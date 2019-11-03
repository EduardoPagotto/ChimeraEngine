#ifndef __CHIMERA_TERRAIN_DATA__HPP
#define __CHIMERA_TERRAIN_DATA__HPP

#include "Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {

class TerrainData {
  public:
    TerrainData();
    virtual ~TerrainData();
    void destroy();
    bool loadBinary(char* FileName);

    std::vector<VertexData> vertices;
    std::vector<int> indices;

  private:
    int getIndex(int X, int Z);
    float getHeight(int X, int Z);
    void setDefaults();

    int sizeHeight;
    int sizeP1;
    float sizeD2;
    glm::vec3 Min;
    glm::vec3 Max;

    std::vector<float> heights;
};

} // namespace Chimera

#endif