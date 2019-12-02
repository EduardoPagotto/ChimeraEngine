#ifndef __CHIMERA_SHADOW_MAP_VISITOR__HPP
#define __CHIMERA_SHADOW_MAP_VISITOR__HPP

#include "Node.hpp"
#include "NodeVisitor.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Tex.hpp"
#include "chimera/core/Transform.hpp"
#include <string>

namespace Chimera {

class ShadowMapVisitor : public NodeVisitor {
  public:
    ShadowMapVisitor(Shader* _pShader, const unsigned& _width, const unsigned& _height);
    virtual ~ShadowMapVisitor();

    virtual void init() override;
    virtual void visit(class NodeCamera* _pCamera) override;
    virtual void visit(class NodeMesh* _pMesh) override;
    virtual void visit(class NodeLight* _pLight) override;
    virtual void visit(class NodeParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class NodeHUD* _pHUD) override;

    void render(Node* _pGroup, Transform* _pTransform);
    void applyShadow(Shader* _pShader);
    inline glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }

  private:
    void initSceneShadow();
    void endSceneShadow();
    void setLightSpaceMatrix(const glm::vec3& _posicaoLight);

    glm::mat4 lightSpaceMatrix;
    Transform* pTransform;
    Shader* pShader;
    TexFBO* pTexture;
};
} // namespace Chimera

#endif
