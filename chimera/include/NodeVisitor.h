#ifndef NODE_VISITOR__H
#define NODE_VISITOR__H

#include "Camera.h"
#include "Draw.h"
#include "Light.h"
#include "Object.h"
#include "ParticleEmitter.h"
#include "SkyBox.h"
#include "SceneRoot.h"

namespace Chimera {

class NodeVisitor
{
public:
	NodeVisitor();
	virtual ~NodeVisitor();

	virtual void visit(Camera* _pCamera) = 0;
	virtual void visit(Draw* _pDraw) = 0;
	virtual void visit(Light *_pLight) = 0;
	virtual void visit(Object *_pObject) = 0;
	virtual void visit(ParticleEmitter *_pParticleEmitter) = 0;
	virtual void visit(SkyBox *_pSkyBox) = 0;
	virtual void visit(SceneRoot *_pSceneRoot) = 0;

};
}

# endif

