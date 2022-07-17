#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/buffer/IndexBuffer.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"

namespace Chimera {

class Renderable3D : public IRenderable3d {
  public:
    virtual ~Renderable3D();
    virtual const uint32_t getSize() const = 0;
    virtual const AABB& getAABB() const = 0;
    virtual IndexBuffer* getIBO() const = 0;
    virtual VertexArray* getVao() const { return vao; }
    virtual void draw(const bool& logData);

  protected:
    VertexArray* vao = nullptr;
};
} // namespace Chimera
