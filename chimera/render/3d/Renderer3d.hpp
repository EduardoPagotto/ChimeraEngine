#pragma once
#include "IRenderer3d.hpp"
#include "RenderCommand.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/Light.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/partition/Frustum.hpp"
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
    virtual inline std::unordered_map<std::string, UValue>& uQueue() override { return uniformsQueue; }

  private:
    std::deque<RenderCommand> commandQueue;
    std::unordered_map<std::string, UValue> uniformsQueue;
    Frustum frustum;
    uint32_t totIBO, totFaces;
};
} // namespace Chimera
