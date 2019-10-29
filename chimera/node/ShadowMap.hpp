#ifndef __CHIMERA_SHADOW_MAP__HPP
#define __CHIMERA_SHADOW_MAP__HPP

#include "chimera/render/Tex.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class ShadowMap {
  public:
    ShadowMap(const unsigned& _width, const unsigned& _height);
    virtual ~ShadowMap();

    virtual void init();

    glm::mat4 createLightSpaceMatrix(const glm::vec3& _posicaoLight);

    void initSceneShadow();
    void endSceneShadow();
    void applyShadow(Shader* _pShader);

    glm::mat4 lightSpaceMatrix;

  private:
    TexFBO* pTexture;
};

} // namespace Chimera

#endif
