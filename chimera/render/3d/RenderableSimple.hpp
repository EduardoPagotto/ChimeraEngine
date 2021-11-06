#pragma once

#include "IRenderable3d.hpp"
#include "PolygonIndex.hpp"
#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

class RenderableSimple : public IRenderable3d {
  public:
    RenderableSimple();
    virtual ~RenderableSimple();

    virtual void debugDados() override;
    virtual uint32_t getSize() const override { return poligonIndex.getSize(); }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return poligonIndex.getIBO(); }
    virtual const AABB& getAABB() const override { return poligonIndex.getAABB(); }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual Entity getEntity() const override { return entity; }

    inline bool empty() const { return poligonIndex.empty(); }

    void setEntity(Entity entity);

    // inicializa criando vao e ibo
    void createBuffers(VertexData* vertexData, const uint32_t& vertexSize, uint32_t* indexData, const uint32_t& indexSize);

    // inicializa face a face e no final cria ibo apenas (VAO e nulo)
    void addTris(const uint32_t& _pa, const uint32_t& _pb, const uint32_t& _pc);
    void initializeBuffer(VertexData* vertexData, const uint32_t& vertexSize);

  private:
    PolygonIndex poligonIndex;
    VertexArray* vao;
    Entity entity;
};
} // namespace Chimera
