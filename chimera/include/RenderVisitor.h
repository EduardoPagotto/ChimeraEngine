#ifndef __RENDER_VISITOR_H__
#define __RENDER_VISITOR_H__

#include "NodeVisitor.h"
#include "Video.h"
#include "ShadersManager.h"
#include "Coord.h"

#include "ShadowMap.h"

namespace Chimera {

class RenderVisitor : public NodeVisitor {
public:
    RenderVisitor();
    virtual ~RenderVisitor();
    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light *_pLight) override;
    virtual void visit(class ParticleEmitter *_pParticleEmitter) override;
    virtual void visit(class SceneMng *_pSceneMng) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class Transform* _pTransform) override;
    virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;

    Coord *pCoord;
    Video *pVideo;
	int eye;

private:
	bool HudOn;
	bool particleOn;

	glm::mat4 projection;
	glm::mat4 view;
    glm::mat4 model;

	ShadersManager *shadersManager;

	ShadowMap *shadowOn;
};
}

#endif
