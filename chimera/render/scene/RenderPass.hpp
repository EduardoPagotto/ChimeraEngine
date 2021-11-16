#pragma once
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/buffer/RenderBuffer.hpp"
#include <entt/entt.hpp>

namespace Chimera {
class RenderPass {
  public:
    RenderPass(uint32_t width, uint32_t height);
    virtual ~RenderPass();
    void execute(ICamera* camera, IRenderer3d& renderer, entt::registry& eRegistry);

    void render() { renderBuffer->render(); }

  private:
    uint32_t viewportWidth, viewportHeight;
    RenderBuffer* renderBuffer;
    FrameBuffer* fb;
};

} // namespace Chimera