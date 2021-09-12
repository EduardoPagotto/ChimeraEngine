#ifndef __CHIMERA_VERTEX_DYNAMIC_HPP
#define __CHIMERA_VERTEX_DYNAMIC_HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"

namespace Chimera {

class VertexRenderDynamic {
  public:
    VertexRenderDynamic(const uint32_t& max);
    virtual ~VertexRenderDynamic();
    void render(VertexData* pVertice, const uint32_t& size);

  private:
    Core::VertexArray vao;
    Core::VertexBuffer* vbo;
};
} // namespace Chimera
#endif