#ifndef __CHIMERA_VERTEX_NODE__HPP
#define __CHIMERA_VERTEX_NODE__HPP

#include "chimera/render/AABB.hpp"
#include "chimera/render/VertexData.hpp"
#include <vector>

namespace Chimera {

class VertexNode {
  public:
    VertexNode();
    ~VertexNode();

    inline void addIndex(unsigned int _val) { index.push_back(_val); }
    inline bool empty() { return (index.size() == 0); }
    void initIndexBufferObject();
    void initAABB(std::vector<VertexData>& vertexDataIn);
    void debugDados();

    void render();
    unsigned int getIndexBufferObject() { return indexBufferObject; }

  private:
    unsigned int indexBufferObject;
    AABB aabb;
    std::vector<unsigned int> index;
};
} // namespace Chimera

#endif
