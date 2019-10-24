#ifndef __CHIMERA_SHADOW_MAP_VISITOR__HPP
#define __CHIMERA_SHADOW_MAP_VISITOR__HPP

#include "ITransform.hpp"
#include "NodeVisitor.hpp"
#include "ShadowMap.hpp"
#include "chimera/render/Shader.hpp"
#include <string>

namespace Chimera {

class ShadowMapVisitor : public NodeVisitor {
  public:
    ShadowMapVisitor(Shader* _pShader);
    virtual ~ShadowMapVisitor();

    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light* _pLight) override;
    virtual void visit(class ParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class HUD* _pHUD) override;
    virtual void visit(class ShadowMap* _pShadowMap) override;

    ITransform* pTransform;
    ShadowMap* shadowMap;

  private:
    Shader* pShader;
    glm::mat4 model;
};
} // namespace Chimera

#endif
