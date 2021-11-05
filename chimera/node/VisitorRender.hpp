#ifndef __CHIMERA_VISITOR_RENDER__HPP
#define __CHIMERA_VISITOR_RENDER__HPP

#include "chimera/node/VisitorShadowMap.hpp"
#include "chimera/render/CanvasGL.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/Shader.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/vbs/SimpleRender3d.hpp"

namespace Chimera {

class VisitorRender : public VisitorInterface {
  public:
    VisitorRender();
    virtual ~VisitorRender();
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

    SimpleRender3d render3D;

  private:
    bool HudOn;
    bool particleOn;

    ICamera3D* cameraScene;
    glm::mat4 model;

    Shader shader;
    VisitorShadowMap* pShadowMapVisitor;
};
} // namespace Chimera

#endif
