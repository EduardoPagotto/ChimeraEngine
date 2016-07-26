#ifndef __RENDER_VISITOR_H__
#define __RENDER_VISITOR_H__

#include "NodeVisitor.h"
#include "Video.h"

namespace Chimera {
   
class RenderVisitor : public NodeVisitor {
public:
    
    RenderVisitor();
    virtual ~RenderVisitor();
    
    virtual void visit(class Camera* _pCamera);
    virtual void visit(class Mesh* _pMesh);
    virtual void visit(class Light *_pLight);
    virtual void visit(class ParticleEmitter *_pParticleEmitter);
    virtual void visit(class SceneRoot *_pSceneRoot);
    virtual void visit(class Group* _pGroup);
    virtual void visit(class Transform* _pTransform);
    virtual void visit(class Solid* _pSolid);
    virtual void visit(class Coord* _pCord);
    virtual void visit(class HUD* _pHUD);
    
    Solid *pOrigemDesenho;
    bool textureOn;
	bool HudOn;
	bool particleOn;
    Video *pVideo;
    int eye;
    
private:
};    
}

#endif
