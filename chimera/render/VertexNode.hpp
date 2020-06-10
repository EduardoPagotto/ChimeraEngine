#ifndef __CHIMERA_VERTEX_NODE__HPP
#define __CHIMERA_VERTEX_NODE__HPP

#include "chimera/render/AABB.hpp"
#include "chimera/render/EBO.hpp"
#include "chimera/render/VertexData.hpp"
#include <vector>

namespace Chimera {

class VertexNode {
  public:
    VertexNode();
    ~VertexNode();

    inline bool empty() { return (index.size() == 0); }
    void addFace(unsigned int& _face, unsigned int& _pa, unsigned int& _pb, unsigned int& _pc);
    void initIndexBufferObject();
    void initAABB(std::vector<VertexData>& vertexDataIn);
    void debugDados();
    void render();

    AABB aabb;

  private:
    EBO ebo;
    std::vector<unsigned int> index;
    std::vector<unsigned int> faces;
};
} // namespace Chimera

#endif
