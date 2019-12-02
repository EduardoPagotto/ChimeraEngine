#ifndef __CHIMERA_RENDER_VISITOR__HPP
#define __CHIMERA_RENDER_VISITOR__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/node/VisitorShadowMap.hpp"

namespace Chimera {

class RenderVisitor : public VisitorInterface {
  public:
    RenderVisitor();
    virtual ~RenderVisitor();
    virtual void init() override;
    virtual void visit(class NodeCamera* _pCamera) override;
    virtual void visit(class NodeMesh* _pMesh) override;
    virtual void visit(class NodeLight* _pLight) override;
    virtual void visit(class NodeParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class NodeGroup* _pGroup) override;
    virtual void visit(class NodeHUD* _pHUD) override;

    Transform* pTransform;
    CanvasGL* pVideo;
    int eye;

  private:
    bool HudOn;
    bool particleOn;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    Shader* pShader;
    VisitorShadowMap* pShadowMapVisitor;
};
} // namespace Chimera

#endif
