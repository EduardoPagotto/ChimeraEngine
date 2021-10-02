#ifndef __CHIMERA_RENDERABLE_DYNAMIC_HPP
#define __CHIMERA_RENDERABLE_DYNAMIC_HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"

namespace Chimera {

class RenderableDynamic {
  public:
    RenderableDynamic(const uint32_t& max);
    virtual ~RenderableDynamic();
    void render(VertexData* pVertice, const uint32_t& size);

  private:
    uint32_t max;
    Core::VertexArray vao;
    Core::VertexBuffer* vbo;
};
} // namespace Chimera
#endif