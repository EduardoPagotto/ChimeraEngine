#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/render/AABB.hpp"
#include "chimera/render/MeshData.hpp"

namespace Chimera {

struct NodeHeightMap {
    int IndexBufferObject;
    AABB aabb;
    std::vector<unsigned int> index;
};

class HeightMap {
  public:
    HeightMap(int _width, int _height, int _squareX, int _squareZ);
    ~HeightMap();

    void split(MeshData& _mesh);

  private:
    void initAABB(MeshData& _mesh);
    std::vector<NodeHeightMap*> vNodes;

    int width;
    int height;
    int squareX;
    int squareZ;
};

} // namespace Chimera
#endif
