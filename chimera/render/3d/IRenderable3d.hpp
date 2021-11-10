#pragma once
#include <stdint.h>

namespace Chimera {

class IRenderer3d;
class IRenderable3d {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual class VertexArray* getVao() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;
    virtual const class AABB& getAABB() const = 0;
    virtual class Entity getEntity() const = 0;
    virtual void submit(class ICamera* camera, class RenderCommand& command, class IRenderer3d* renderer) = 0;
};
} // namespace Chimera
