#include "chimera/core/Registry.hpp"
#include "chimera/core/buffer/FrameBuffer.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/ITrans.hpp"
#include "chimera/core/visible/Shader.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"

namespace Chimera {

class ShadowPass {
  public:
    ShadowPass(const uint32_t& width, const uint32_t& height, const glm::mat4& projection);
    virtual ~ShadowPass();
    void render(Registry* registry, Camera* camera, EyeView* eyeView, IRenderer3d& renderer, ITrans* origem, const bool& logRender);
    void setProp(const glm::vec3& positionCam, IRenderer3d& renderer);

  private:
    Shader shader;
    FrameBuffer* shadowBuffer;
    glm::mat4 lightSpaceMatrix, lightProjection;
};
} // namespace Chimera