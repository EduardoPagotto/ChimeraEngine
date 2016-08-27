#ifndef __RENDER_VISITOR_H__
#define __RENDER_VISITOR_H__

#include "NodeVisitor.h"
#include "Video.h"

namespace Chimera {

class RenderVisitor : public NodeVisitor {
public:
    RenderVisitor();
    virtual ~RenderVisitor();
    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light *_pLight) override;
    virtual void visit(class ParticleEmitter *_pParticleEmitter) override;
    virtual void visit(class SceneRoot *_pSceneRoot) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class Transform* _pTransform) override;
    virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;

	bool HudOn;
	bool particleOn;
	int eye;
    Coord *pCoord;
    Video *pVideo;

	glm::mat4 projection;
	glm::mat4 view;

private:
    glm::mat4 model;
};
}

#endif
