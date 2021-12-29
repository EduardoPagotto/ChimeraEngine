#pragma once
#include "Renderable3D.hpp"
#include "chimera/render/buffer/VertexArray.hpp"

namespace Chimera {

class RenderableChunk : public IRenderable3d {
  public:
    RenderableChunk(std::vector<Renderable3D*>& vpLeafData, std::vector<VertexData>& vertexData);
    ~RenderableChunk();

    virtual void submit(ICamera* Icamera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void debugDados() const override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void draw(const bool& logData) override{}; // TODO: ver o que fazer!!!

  private:
    VertexArray* vao;
    std::vector<Renderable3D*> vpLeaf;
    std::vector<VertexData> vVertex;
    AABB aabb;
    uint32_t totIndex;
};
} // namespace Chimera
