#ifndef __CHIMERA_IRENDERABLE__HPP
#define __CHIMERA_IRENDERABLE__HPP

#include "chimera/core/Shader.hpp"
#include "chimera/core/buffers/IndexBuffer.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include <deque>

namespace Chimera {

class IRenderable {
  public:
    virtual void debugDados() = 0;
    virtual uint32_t getSize() const = 0;
    virtual Shader* getShader() const = 0;
    virtual Core::VertexArray* getVao() const = 0;
    virtual Core::IndexBuffer* getIBO() const = 0;
    virtual AABB* getAABB() = 0;
    virtual void inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue) = 0;
};

class IRender3d {
  public:
    virtual void begin(glm::vec3* eye, Frustum* frustrun, bool logData) = 0;
    virtual void end() = 0;
    virtual void submit(IRenderable* renderable) = 0;
    virtual void flush() = 0;
};

} // namespace Chimera
#endif