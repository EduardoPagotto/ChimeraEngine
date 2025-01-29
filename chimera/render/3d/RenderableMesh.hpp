#pragma once
#include "coreGL/IndexBuffer.hpp"
#include "coreGL/VertexArray.hpp"
#include "render/3d/Renderable3D.hpp"
#include "space/AABB.hpp"
#include "space/Mesh.hpp"

namespace ce {

class RenderableMesh : public Renderable3D {
  public:
    RenderableMesh(Mesh* mesh);
    virtual ~RenderableMesh();

    virtual const uint32_t getSize() const override { return totIndex; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;

  private:
    uint32_t totIndex;
    IRenderable3d* child;
    AABB aabb;
};
} // namespace ce
