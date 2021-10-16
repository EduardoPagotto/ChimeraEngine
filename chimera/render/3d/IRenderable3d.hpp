#ifndef __CHIMERA_IRENDERABLE_3D_Z1__HPP
#define __CHIMERA_IRENDERABLE_3D_Z1__HPP

#include "Renderer3d.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"

namespace Chimera {

class Renderer3d;
class IRenderable3d {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual Shader* getShader() const = 0;
    virtual VertexArray* getVao() const = 0;
    virtual IndexBuffer* getIBO() const = 0;
    virtual AABB* getAABB() = 0;
    virtual void submit(Renderer3d* renderer) = 0;
};

} // namespace Chimera
#endif