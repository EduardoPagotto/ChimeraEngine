#ifndef __CHIMERA_RENDERABLE_CHUNK__HPP
#define __CHIMERA_RENDERABLE_CHUNK__HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/3d/RenderableSimple.hpp"

namespace Chimera {

class RenderableChunk : public IRenderable3d {
  public:
    RenderableChunk(Entity entity, std::vector<RenderableSimple*>& vpLeafData, std::vector<VertexData>& vertexData);
    ~RenderableChunk();

    virtual void submit(ICamera* Icamera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void debugDados() override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual Entity getEntity() const override { return entity; }

  private:
    VertexArray* vao;
    std::vector<RenderableSimple*> vpLeaf;
    std::vector<VertexData> vVertex;
    AABB aabb;
    uint32_t totIndex;
    Entity entity;
};

} // namespace Chimera
#endif
