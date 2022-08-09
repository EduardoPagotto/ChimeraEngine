#pragma once
#include "chimera/core/buffer/IndexBuffer.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace Chimera {

class RenderableIBO : public Renderable3D {
  public:
    RenderableIBO(IndexBuffer* ibo, const AABB& aabb) : ibo(ibo), aabb(aabb), Renderable3D() {}
    virtual ~RenderableIBO();

    virtual const uint32_t getSize() const override { return ibo->getSize(); }
    virtual IndexBuffer* getIBO() const override { return ibo; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
    virtual void draw(const bool& logData) override;

  private:
    IndexBuffer* ibo;
    AABB aabb;
};
} // namespace Chimera
