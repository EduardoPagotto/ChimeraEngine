#pragma once
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/buffer/FrameBuffer.hpp"
#include <entt/entt.hpp>

namespace Chimera {

class RenderPassShadow {
  public:
    RenderPassShadow(uint32_t width, uint32_t height);
    void execute(ICamera* camera, IRenderer3d& renderer, entt::registry& eRegistry);

    inline glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }
    void bindDepthBuffer() { shadowBuffer->getDepthAttachemnt()->bind(1); } // getTexture()->bind(1); }
  private:
    Shader shader;
    FrameBuffer* shadowBuffer;
    glm::mat4 lightSpaceMatrix;
    glm::mat4 lightProjection;
};

} // namespace Chimera