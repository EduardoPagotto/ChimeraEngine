#ifndef __CHIMERA_HEIGHTMAP__HPP
#define __CHIMERA_HEIGHTMAP__HPP

#include "chimera/render/AABB.hpp"
#include "chimera/render/MeshData.hpp"

namespace Chimera {

class NodeHeightMap {
  public:
    NodeHeightMap();
    ~NodeHeightMap();

    inline void addIndex(unsigned int _val) { index.push_back(_val); }
    inline bool empty() { return (index.size() == 0); }
    void initIndexBufferObject();
    void initAABB(std::vector<glm::vec3> _vertexListMesh, std::vector<unsigned int> _vertexIndexMesh);
    void debugDados();

  private:
    unsigned int indexBufferObject;
    AABB aabb;
    std::vector<unsigned int> index;
};

class HeightMap {
  public:
    HeightMap(int _width, int _height, int _squareX, int _squareZ);
    ~HeightMap();
    void split(MeshData& _mesh);

  private:
    std::vector<NodeHeightMap*> vNodes;

    int width;
    int height;
    int squareX;
    int squareZ;
};

} // namespace Chimera
#endif
