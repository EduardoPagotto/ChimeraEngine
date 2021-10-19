#ifndef __CHIMERA_RENDER_3D___ZZ1_HPP
#define __CHIMERA_RENDER_3D___ZZ1_HPP

#include "chimera/core/Camera.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/space/Frustum.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {
class IRenderable3d;
class Renderer3d {
  public:
    Renderer3d() = default;
    virtual ~Renderer3d() = default;

    virtual void begin(Camera* camera);
    virtual void submit(IRenderable3d* renderable);
    virtual void end();
    virtual void flush(bool useMaterial, Shader* alternativeShader);

    void logToggle() { logData = !logData; }
    bool getLog() const { return logData; }

  private:
    std::deque<IRenderable3d*> renderQueue;
    Camera* camera = nullptr;
    Shader* activeShader = nullptr;
    Frustum frustum;

    bool logData = false;
    uint32_t totIBO = 0;
    uint32_t totFaces = 0;
};

} // namespace Chimera
#endif