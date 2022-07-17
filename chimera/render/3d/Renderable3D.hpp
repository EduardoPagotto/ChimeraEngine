#pragma once
#include "IRenderable3d.hpp"
#include <stdint.h>

namespace Chimera {
class Renderable3D : public IRenderable3d {
  public:
    Renderable3D() = default;
    virtual ~Renderable3D();
    virtual const uint32_t getSize() const = 0;
    virtual const class AABB& getAABB() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;
    virtual class VertexArray* getVao() const { return vao; }
    virtual void draw(const bool& logData);

  protected:
    VertexArray* vao = nullptr;
};
} // namespace Chimera
