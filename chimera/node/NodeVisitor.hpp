#ifndef __CHIMERA_NODE_VISITOR__HPP
#define __CHIMERA_NODE_VISITOR__HPP

namespace Chimera {

class NodeVisitor {
  public:
    virtual void init() = 0;
    virtual void visit(class Camera* _pCamera) = 0;
    virtual void visit(class NodeMesh* _pMesh) = 0;
    virtual void visit(class NodeLight* _pLight) = 0;
    virtual void visit(class ParticleEmitter* _pParticleEmitter) = 0;
    virtual void visit(class Group* _pGroup) = 0;
    virtual void visit(class HUD* _pHUD) = 0;
};
} // namespace Chimera

#endif
