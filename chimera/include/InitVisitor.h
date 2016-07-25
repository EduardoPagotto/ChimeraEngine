#ifndef __INIT_VISITOR__H
#define __INIT_VISITOR__H

#include "NodeVisitor.h"

namespace Chimera {
        
class InitVisitor : public NodeVisitor {
public:
    InitVisitor();
    virtual ~InitVisitor();
    
    virtual void visit(class Camera* _pCamera);
    virtual void visit(class Draw* _pDraw);
    virtual void visit(class Light *_pLight);
    virtual void visit(class ParticleEmitter *_pParticleEmitter);
    virtual void visit(class SceneRoot *_pSceneRoot);
    virtual void visit(class Group* _pGroup);
    virtual void visit(class Transform* _pTransform);
    virtual void visit(class Solid* _pSolid);
    virtual void visit(class Coord* _pCord);
    virtual void visit(class HUD* _pHUD);
    
private:
    
};
    
}

#endif
