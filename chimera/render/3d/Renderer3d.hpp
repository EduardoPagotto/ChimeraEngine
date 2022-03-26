#pragma once
#include "IRenderer3d.hpp"
#include "RenderCommand.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/Light.hpp"
#include "chimera/core/visible/Shader.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d();
    virtual ~Renderer3d();
    virtual void begin(ICamera* camera) override;
    virtual void submit(const RenderCommand& command) override;
    virtual void end() override;
    virtual void flush() override;
    virtual inline MapUniform& uQueue() override { return uniformsQueue; }

  private:
    std::deque<RenderCommand> commandQueue;
    MapUniform uniformsQueue;
    Frustum frustum;
    uint32_t totIBO, totFaces;
};
} // namespace Chimera
