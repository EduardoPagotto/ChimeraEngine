#pragma once
#include <stdint.h>

namespace Chimera {

class IRenderable3d {
  public:
    virtual ~IRenderable3d() = default;
    virtual const uint32_t getSize() const = 0;
    virtual class VertexArray* getVao() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;
    virtual const class AABB& getAABB() const = 0;
    virtual void submit(class RenderCommand& command, class IRenderer3d& renderer) = 0;
    virtual void draw(const bool& logData) = 0;
};
} // namespace Chimera
