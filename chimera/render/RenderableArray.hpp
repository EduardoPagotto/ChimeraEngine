#pragma once
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace Chimera {

class RenderableArray : public Renderable3D {
  public:
    RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh);
    virtual ~RenderableArray();

    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
    virtual const uint32_t getSize() const override { return totIndex; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }

  private:
    std::vector<IRenderable3d*> vChild;
    AABB aabb;
    uint32_t totIndex;
};
} // namespace Chimera
