#ifndef __CHIMERA_RENDERABLE_STATIC_HPP
#define __CHIMERA_RENDERABLE_STATIC_HPP

#include "IRenderable.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera {

class RenderableStatic : public IRenderable {
  public:
    RenderableStatic(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize);
    virtual ~RenderableStatic();

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return ibo->getCount(); }
    virtual Shader* getShader() const { return nullptr; };
    virtual Core::VertexArray* getVao() const { return vao; }
    virtual Core::IndexBuffer* getIBO() const { return ibo; }
    virtual Core::AABB* getAABB() override { return &aabb; }
    virtual void submit(IRenderer3d* renderer) override;

  private:
    Core::AABB aabb;
    Core::VertexArray* vao;
    Core::IndexBuffer* ibo;
};
} // namespace Chimera
#endif