#ifndef __CHIMERA_RENDERABLE_CHUNK__HPP
#define __CHIMERA_RENDERABLE_CHUNK__HPP

#include "chimera/core/Transform.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/3d/RenderableFace.hpp"
#include "chimera/render/Material.hpp"

namespace Chimera {

class RenderableChunk : public IRenderable3d {
  public:
    RenderableChunk(Entity entity, std::vector<RenderableFace*>& vpLeafData, std::vector<VertexData>& vertexData);
    ~RenderableChunk();

    virtual void submit(Camera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void debugDados() override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return nullptr; }
    virtual AABB* getAABB() { return &aabb; }
    virtual Entity getEntity() const override { return entity; }
    virtual Material* getMaterial() override { return material; }

  private:
    VertexArray* vao;
    std::vector<RenderableFace*> vpLeaf;
    std::vector<VertexData> vVertex;
    AABB aabb;
    uint32_t totIndex;
    Entity entity;
    Material* material;
    Shader shader;
    Transform* model;
};

} // namespace Chimera
#endif
