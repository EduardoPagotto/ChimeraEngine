#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/buffer/IndexBuffer.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"

namespace Chimera {

class Renderable3D : public IRenderable3d {
  public:
    Renderable3D(VertexArray* vao, IndexBuffer* ibo, const AABB& aabb) : vao(vao), ibo(ibo), aabb(aabb) {}
    virtual ~Renderable3D();

    virtual const uint32_t getSize() const override { return ibo->getSize(); }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return ibo; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void draw(const bool& logData) override;

  private:
    VertexArray* vao;
    IndexBuffer* ibo;
    AABB aabb;
};
} // namespace Chimera
