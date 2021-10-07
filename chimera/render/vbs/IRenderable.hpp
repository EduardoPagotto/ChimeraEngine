#ifndef __CHIMERA_IRENDERABLE__HPP
#define __CHIMERA_IRENDERABLE__HPP

#include "IRenderer3d.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"

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
#endif