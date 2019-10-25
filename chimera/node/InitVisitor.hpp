#ifndef __CHIMERA_INIT_VISITOR__HPP
#define __CHIMERA_INIT_VISITOR__HPP

#include "NodeVisitor.hpp"

namespace Chimera {

class InitVisitor : public NodeVisitor {
  public:
    InitVisitor();
    virtual ~InitVisitor();
    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light* _pLight) override;
    virtual void visit(class ParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class Group* _pGroup) override;
    // virtual void visit(class Transform* _pTransform) override;
    // virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;
    virtual void visit(class ShadowMap* _pShadowMap) override;
};
} // namespace Chimera
#endif
