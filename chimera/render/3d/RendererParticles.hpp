#pragma once
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/partition/Frustum.hpp"
#include <deque>

namespace Chimera {
class RendererParticles : public IRenderer3d {
  public:
    RendererParticles();
    virtual ~RendererParticles();

    virtual void begin(ICamera* camera) override;
    virtual void submit(const RenderCommand& command) override;
    virtual void end() override;
    virtual void flush() override;
    virtual void logToggle() override { logData = !logData; }
    virtual bool getLog() const override { return logData; }
    virtual inline std::vector<UniformVal>& uQueue() override { return uniformsQueue; }

  private:
    std::deque<RenderCommand> commandQueue;
    std::vector<UniformVal> uniformsQueue;
    Frustum frustum;
    bool logData;
};
} // namespace Chimera