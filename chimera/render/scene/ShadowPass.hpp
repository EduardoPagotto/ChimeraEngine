#include "chimera/core/Registry.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/ITrans.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/buffer/FrameBuffer.hpp"

namespace Chimera {

class ShadowPass {
  public:
    ShadowPass(const uint32_t& width, const uint32_t& height, const glm::mat4& projection);
    virtual ~ShadowPass();
    void exec(Registry& registry, ICamera* camera, IRenderer3d& renderer, ITrans* origem, const bool& logRender);
    void appy(ICamera* camera, IRenderer3d& renderer);

  private:
    Shader shader;
    FrameBuffer* shadowBuffer;
    glm::mat4 lightSpaceMatrix, lightProjection;
};
} // namespace Chimera