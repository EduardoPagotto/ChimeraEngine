#ifndef __CHIMERA_RENDER_3D___ZZ1_HPP
#define __CHIMERA_RENDER_3D___ZZ1_HPP

#include "IRenderer3d.hpp"
#include "RenderCommand.hpp"
#include "chimera/core/ICamera.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d();
    virtual ~Renderer3d();
    virtual void begin(ICamera* camera) override;
    virtual void submit(const RenderCommand& command) override;
    virtual void submitLight(Light* light) override;
    virtual void end() override;
    virtual void flush() override;
    virtual void logToggle() override { logData = !logData; }
    virtual bool getLog() const override { return logData; }

  private:
    std::deque<RenderCommand> commandQueue;
    std::vector<UniformVal> lightQueue;
    ICamera* camera;
    Shader activeShader;
    Frustum frustum;
    bool logData;
    uint32_t totIBO, totFaces;
};

} // namespace Chimera
#endif