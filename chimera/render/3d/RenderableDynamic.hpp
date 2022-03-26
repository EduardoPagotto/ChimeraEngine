#pragma once
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/buffer/VertexBuffer.hpp"
#include "chimera/render/VertexData.hpp"

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
