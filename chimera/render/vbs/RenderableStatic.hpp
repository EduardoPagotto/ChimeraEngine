#pragma once

#include "IRenderable.hpp"
#include "chimera/render/buffers/IndexBuffer.hpp"
#include "chimera/render/buffers/VertexArray.hpp"

namespace Chimera {

class RenderableStatic : public IRenderable {
  public:
    RenderableStatic(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize);
    virtual ~RenderableStatic();

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return ibo->getCount(); }
    virtual Shader* getShader() const { return nullptr; };
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return ibo; }
    virtual AABB* getAABB() override { return &aabb; }
    virtual void submit(IRenderer3d* renderer) override;

  private:
    AABB aabb;
    VertexArray* vao;
    IndexBuffer* ibo;
};
} // namespace Chimera
