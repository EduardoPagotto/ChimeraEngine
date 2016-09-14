#ifndef __RENDER_VISITOR_H__
#define __RENDER_VISITOR_H__

#include "NodeVisitor.h"
#include "Video.h"

#include "ShadowMap.h"

#include <Shader.h>

namespace Chimera {

class RenderVisitor : public NodeVisitor {
public:
    RenderVisitor();
    virtual ~RenderVisitor();
	
	void execute(Node *_node, const unsigned &_eye);

    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light *_pLight) override;
    virtual void visit(class ParticleEmitter *_pParticleEmitter) override;
    virtual void visit(class SceneRoot *_pSceneRoot) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class Transform* _pTransform) override;
    virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;

    Coord *pCoord;
    Video *pVideo;

    //glm::mat4 lightSpaceMatrix;
private:
	void DFS(Node * u);

	bool HudOn;
	bool particleOn;
	bool runningShadow;

	int eye;

	glm::mat4 projection;
	glm::mat4 view;
    glm::mat4 model;

	//glm::mat4 lightSpaceMatrix;

	Shader *shader;
	ShadowMap *shadoMap;
};
}

#endif
