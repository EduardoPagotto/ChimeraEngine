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
    bool saveBinary(char* FileName);
    void setDefaults();
    void getMinMax(glm::mat4& ViewMatrix, glm::vec3& Min, glm::vec3& Max);

    inline int getSize() const { return sizeHeight; }
    inline glm::vec3 getMin() const { return Min; }
    inline glm::vec3 getMax() const { return Max; }
    inline int getTrianglesCount() const { return indices.size() / 3.0f; }

    float GetHeight(float X, float Z);

    std::vector<VertexData> vertices;
    std::vector<int> indices;

  private:

    int getIndex(int X, int Z);
    float getHeight(int X, int Z);
    float GetHeight(float* Heights, int Size, float X, float Z);

    int verticesCount;
    int sizeHeight;
    int sizeP1;
    float sizeD2;
    glm::vec3 Min;
    glm::vec3 Max;

    float* heights;
};

} // namespace Chimera

#endif