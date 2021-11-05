#pragma once

#include "IRenderer3d.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/buffers/IndexBuffer.hpp"
#include "chimera/render/buffers/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"

namespace Chimera {

class IRenderable {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual Shader* getShader() const = 0;
    virtual VertexArray* getVao() const = 0;
    virtual IndexBuffer* getIBO() const = 0;
    virtual AABB* getAABB() = 0;
    virtual void submit(class IRenderer3d* renderer) = 0;
};
} // namespace Chimera
