#ifndef __CHIMERA_RENDER_VISITOR__HPP
#define __CHIMERA_RENDER_VISITOR__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/Transform.hpp"

namespace Chimera {

class RenderVisitor : public NodeVisitor {
  public:
    RenderVisitor();
    virtual ~RenderVisitor();
    virtual void init() override;
    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light* _pLight) override;
    virtual void visit(class ParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class HUD* _pHUD) override;

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
    ShadowMapVisitor* pShadowMapVisitor;
};
} // namespace Chimera

#endif
