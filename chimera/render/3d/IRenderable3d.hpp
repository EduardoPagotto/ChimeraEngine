#ifndef __CHIMERA_IRENDERABLE_3D_Z1__HPP
#define __CHIMERA_IRENDERABLE_3D_Z1__HPP

#include <stdint.h>

namespace Chimera {

class IRenderer3d;
class IRenderable3d {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual class VertexArray* getVao() const = 0;
    virtual class IndexBuffer* getIBO() const = 0;
    virtual class AABB* getAABB() = 0;
    virtual class Entity getEntity() const = 0;
    virtual void submit(class Camera* camera, class RenderCommand& command, class IRenderer3d* renderer) = 0;
};

} // namespace Chimera
#endif