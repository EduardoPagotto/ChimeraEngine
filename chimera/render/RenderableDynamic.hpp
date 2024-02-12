#pragma once
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace Chimera {

class RenderableDynamic : public Renderable3D {
  public:
    RenderableDynamic(const uint32_t& max);
    virtual ~RenderableDynamic();
    void render(VertexData* pVertice, const uint32_t& size);

  private:
    uint32_t max;
    VertexBuffer* vbo;
};
} // namespace Chimera
