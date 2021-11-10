#pragma once
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/buffer/VertexBuffer.hpp"

namespace Chimera {

class RenderableDynamic {
  public:
    RenderableDynamic(const uint32_t& max);
    virtual ~RenderableDynamic();
    void render(VertexData* pVertice, const uint32_t& size);

  private:
    uint32_t max;
    VertexArray vao;
    VertexBuffer* vbo;
};
} // namespace Chimera
