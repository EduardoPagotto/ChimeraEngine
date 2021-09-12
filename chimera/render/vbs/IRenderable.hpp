#ifndef __CHIMERA_IRENDERABLE__HPP
#define __CHIMERA_IRENDERABLE__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/AABB.hpp"

namespace Chimera {

class IRenderable {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual Shader* getShader() const = 0;
    virtual Core::VertexArray* getVao() const = 0;
    virtual Core::IndexBuffer* getIBO() const = 0;
    virtual AABB* getAABB() = 0;
};

class IRender3d {
  public:
    virtual void submit(IRenderable* renderable) = 0;
    virtual void flush() = 0;
};

} // namespace Chimera
#endif