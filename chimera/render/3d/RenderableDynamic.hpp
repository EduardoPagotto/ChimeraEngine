#pragma once
// #pragma clang diagnostic ignored "-Wunused-private-field"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace ce {

class RenderableDynamic : public Renderable3D {
  public:
    RenderableDynamic(const uint32_t& max);
    virtual ~RenderableDynamic();
    void render(VertexData* pVertice, const uint32_t& size);

  private:
    [[maybe_unused]]
    uint32_t max;
    VertexBuffer* vbo;
};
} // namespace ce
