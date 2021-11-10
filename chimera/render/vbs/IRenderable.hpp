#pragma once
#include "IRenderer3d.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/buffer/IndexBuffer.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"

namespace Chimera {

class IRenderable {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual class Shader* getShader() const = 0;
    virtual class VertexArray* getVao() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;
    virtual class AABB* getAABB() = 0;
    virtual void submit(class IRenderer3dOld* renderer) = 0;
};
} // namespace Chimera
