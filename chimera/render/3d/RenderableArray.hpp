#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace Chimera {

class RenderableArray : public IRenderable3d {
  public:
    RenderableArray(std::vector<TrisIndex>& vPtrTrisIndex, Mesh* mesh);
    virtual ~RenderableArray();

    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
    virtual const uint32_t getSize() const override { return totIndex; }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void draw(const bool& logData) override;

  private:
    VertexArray* vao;
    std::vector<IRenderable3d*> vChild;
    AABB aabb;
    uint32_t totIndex;
};
} // namespace Chimera
