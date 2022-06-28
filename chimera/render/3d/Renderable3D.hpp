#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/buffer/IndexBuffer.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/visible/Mesh.hpp"

namespace Chimera {

class Renderable3D : public IRenderable3d {
  public:
    Renderable3D(Mesh* mesh);
    virtual ~Renderable3D();

    virtual const uint32_t getSize() const override { return totIndex; }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
    virtual void draw(const bool& logData) override;

  private:
    VertexArray* vao;
    uint32_t totIndex;
    IRenderable3d* child;
    AABB aabb;
};
} // namespace Chimera
