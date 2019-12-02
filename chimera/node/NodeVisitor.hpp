#ifndef __CHIMERA_NODE_VISITOR__HPP
#define __CHIMERA_NODE_VISITOR__HPP

namespace Chimera {

class NodeVisitor {
  public:
    virtual void init() = 0;
    virtual void visit(class NodeCamera* _pCamera) = 0;
    virtual void visit(class NodeMesh* _pMesh) = 0;
    virtual void visit(class NodeLight* _pLight) = 0;
    virtual void visit(class NodeParticleEmitter* _pParticleEmitter) = 0;
    virtual void visit(class NodeGroup* _pGroup) = 0;
    virtual void visit(class NodeHUD* _pHUD) = 0;
};
} // namespace Chimera

#endif
