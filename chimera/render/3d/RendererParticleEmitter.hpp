#pragma once
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include <deque>

namespace Chimera {
class RendererParticleEmitter : public IRenderer3d {
  public:
    RendererParticleEmitter();
    virtual ~RendererParticleEmitter();

    virtual void begin(ICamera* camera) override;
    virtual void submit(const RenderCommand& command) override;
    virtual void submitLight(Light* light) override;
    virtual void submitUniform(const UniformVal& uniform) override { uniformsQueue.push_back(uniform); }
    virtual void end() override;
    virtual void flush() override;
    virtual void logToggle() override { logData = !logData; }
    virtual bool getLog() const override { return logData; }

  private:
    ICamera* camera;
    bool logData;
    std::vector<UniformVal> uniformsQueue;
    std::deque<RenderCommand> commandQueue;
};
} // namespace Chimera