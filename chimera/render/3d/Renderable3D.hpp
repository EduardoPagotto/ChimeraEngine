#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/buffer/VertexArray.hpp"

namespace Chimera {

class Renderable3D : public IRenderable3d {
  public:
    Renderable3D(VertexArray* vao, IndexBuffer* ibo, uint32_t sizeIBO, const AABB& aabb)
        : vao(vao), ibo(ibo), sizeIBO(sizeIBO), aabb(aabb) {}

    virtual ~Renderable3D();

    virtual uint32_t getSize() const override { return sizeIBO; }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return ibo; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void draw(const bool& logData) override;
    inline bool empty() const { return sizeIBO == 0; }

  private:
    VertexArray* vao;
    IndexBuffer* ibo;
    AABB aabb;
    uint32_t sizeIBO;
};
} // namespace Chimera
