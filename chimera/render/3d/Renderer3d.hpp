#ifndef __CHIMERA_RENDER_3D___ZZ1_HPP
#define __CHIMERA_RENDER_3D___ZZ1_HPP

#include "IRenderer3d.hpp"
#include "chimera/core/Camera.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d() = default;
    virtual ~Renderer3d() = default;

    virtual void begin(Camera* camera) override;
    virtual void submit(IRenderable3d* renderable) override;
    virtual void end() override;
    virtual void flush(bool useMaterial, Shader* alternativeShader) override;

    virtual void logToggle() override { logData = !logData; }
    virtual bool getLog() const override { return logData; }

  private:
    void reloadShader(Entity entity);

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