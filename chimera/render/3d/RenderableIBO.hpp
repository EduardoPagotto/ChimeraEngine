#pragma once
#include "chimera/core/gl/buffer/IndexBuffer.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace ce {

class RenderableIBO : public Renderable3D {
  public:
    RenderableIBO(VertexArray* vao, IndexBuffer* ibo, const AABB& aabb);
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
} // namespace ce
