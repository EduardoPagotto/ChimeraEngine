#ifndef __CHIMERA_RENDERABLE_SIMPLE_HPP
#define __CHIMERA_RENDERABLE_SIMPLE_HPP

#include "IRenderable3d.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera {

class RenderableSimple : public IRenderable3d {
  public:
    RenderableSimple(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize);
    virtual ~RenderableSimple();

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return ibo->getCount(); }
    virtual Shader* getShader() const { return nullptr; };
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return ibo; }
    virtual AABB* getAABB() override { return &aabb; }
    virtual void submit(Renderer3d* renderer) override;

  private:
    AABB aabb;
    VertexArray* vao;
    IndexBuffer* ibo;
};
} // namespace Chimera
#endif