#ifndef __CHIMERA_RENDERABLESTATIC3D__HPP
#define __CHIMERA_RENDERABLESTATIC3D__HPP

#include "chimera/core/VertexData.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/render/AABB.hpp"

namespace Chimera {

class RenderableStatic3d {
  public:
    RenderableStatic3d(VertexData* pVertexList, const unsigned int& vertSize, uint32_t* pIndexList, const unsigned int& indexSize);
    virtual ~RenderableStatic3d();

    void render();

    void debugDados();

    uint32_t getSize() const { return size; }

    AABB aabb;

  private:
    unsigned int size;
    Core::IndexBuffer* ibo;
};
} // namespace Chimera
#endif