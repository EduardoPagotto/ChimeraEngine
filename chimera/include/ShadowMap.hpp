#ifndef __CHIMERA_SHADOW_MAP__HPP
#define __CHIMERA_SHADOW_MAP__HPP

#include "Node.hpp"
#include "TextureManager.hpp"
#include <glm/glm.hpp>

namespace Chimera {

class ShadowMap : public Node {
  public:
    ShadowMap(Node* _pNode, std::string _name, const unsigned& _width, const unsigned& _height);
    virtual ~ShadowMap();

    virtual void init();
    virtual void accept(class NodeVisitor* v) override;

    glm::mat4 createLightSpaceMatrix(const glm::vec3& _posicaoLight);

    void initSceneShadow();
    void endSceneShadow();
    void applyShadow();

    TEX_SEQ getShadowIndexTextureSeq() const { return pTexture->getIndexTextureSeq(); }

    glm::mat4 lightSpaceMatrix;

  private:
    TextureManager* texManager;
    Texture* pTexture;
};

} // namespace Chimera

#endif
