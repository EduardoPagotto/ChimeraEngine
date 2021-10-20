#ifndef __CHIMERA_RENDERABLE_SIMPLE_HPP
#define __CHIMERA_RENDERABLE_SIMPLE_HPP

#include "IRenderable3d.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"

namespace Chimera {

class RenderableSimple : public IRenderable3d {
  public:
    RenderableSimple(Entity entity);
    virtual ~RenderableSimple();

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return ibo->getCount(); }
    virtual Shader* getShader() const { return nullptr; };
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return ibo; }
    virtual AABB* getAABB() override { return &aabb; }
    virtual void submit(Camera* camera, Renderer3d* renderer) override;
    virtual Entity getEntity() const override { return entity; }

  private:
    void createBuffers(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize);

    AABB aabb;
    VertexArray* vao;
    IndexBuffer* ibo;
    Entity entity;
};
} // namespace Chimera
#endif