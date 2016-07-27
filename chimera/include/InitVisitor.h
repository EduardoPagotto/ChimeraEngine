#ifndef __INIT_VISITOR__H
#define __INIT_VISITOR__H

#include "NodeVisitor.h"

namespace Chimera {
        
class InitVisitor : public NodeVisitor {
public:
    InitVisitor();
    virtual ~InitVisitor();
    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light *_pLight) override;
    virtual void visit(class ParticleEmitter *_pParticleEmitter) override;
    virtual void visit(class SceneRoot *_pSceneRoot) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class Transform* _pTransform) override;
    virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;    
};
    
}

#endif
