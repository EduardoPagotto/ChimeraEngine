#ifndef NODE_VISITOR__H
#define NODE_VISITOR__H

namespace Chimera {

class NodeVisitor
{
public:
	virtual void visit(class Camera* _pCamera) = 0;
	virtual void visit(class Mesh* _pMesh) = 0;
	virtual void visit(class Light *_pLight) = 0;
	virtual void visit(class ParticleEmitter *_pParticleEmitter) = 0;
	virtual void visit(class SceneMng *_pSceneMng) = 0;
    virtual void visit(class Group* _pGroup) = 0;
	virtual void visit(class Transform* _pTransform) = 0;
	virtual void visit(class Solid* _pSolid) = 0;
	virtual void visit(class HUD* _pHUD) = 0;
};
}

# endif

