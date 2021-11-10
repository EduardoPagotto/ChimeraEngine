#pragma once

namespace Chimera {

class VisitorInterface {
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
