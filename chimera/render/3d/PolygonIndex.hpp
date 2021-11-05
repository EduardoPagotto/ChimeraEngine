#pragma once

#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/partition/AABB.hpp"
#include <vector>

namespace Chimera {

class PolygonIndex {
  public:
    PolygonIndex();
    virtual ~PolygonIndex();

    void addTrisEntire(uint32_t* indexData, const uint32_t& indexSize);
    void addTris(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc);
    void initializeBuffer(VertexData* vertexData, const uint32_t& vertexSize);

    inline bool empty() const { return polygons.empty(); }
    inline const uint32_t getSize() const { return polygons.size(); }
    inline const AABB& getAABB() const { return aabb; }
    inline IndexBuffer* getIBO() const { return ibo; } // FIXME: passar para const depois

    void debugDados() const;

  private:
    IndexBuffer* ibo;
    AABB aabb;
    std::vector<uint32_t> polygons;
};
} // namespace Chimera
