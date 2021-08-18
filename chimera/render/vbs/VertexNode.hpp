#ifndef __CHIMERA_VERTEX_NODE__HPP
#define __CHIMERA_VERTEX_NODE__HPP

#include "EBO.hpp"
#include "VertexData.hpp"
#include "chimera/render/AABB.hpp"
#include <vector>

namespace Chimera {

class VertexNode {
  public:
    VertexNode();
    ~VertexNode();

    inline bool empty() { return (index.size() == 0); }
    void addFace(const uint32_t& _face, const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc);
    void initIndexBufferObject();
    void initAABB(std::vector<VertexData>& vertexDataIn);
    void debugDados();
    void render();

    AABB aabb;

    // private:
    EBO ebo;
    std::vector<uint32_t> index;
    std::vector<uint32_t> faces;
};
} // namespace Chimera

#endif
