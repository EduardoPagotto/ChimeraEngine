#include "RenderVisitor.h"

namespace Chimera {
   
RenderVisitor::RenderVisitor() {

	pOrigemDesenho = nullptr;
	pVideo = nullptr;
	textureOn = true;
	HudOn = true;
	particleOn = true;
	eye = 0;
}
  
RenderVisitor::~RenderVisitor() {

}

void RenderVisitor::visit ( Camera* _pCamera ) {

}

void RenderVisitor::visit ( Mesh* _pMesh ) {
    
    _pMesh->renderExecute(textureOn);

}

void RenderVisitor::visit ( Light* _pLight ) {

}

void RenderVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {
    
	if (particleOn == true) {
		_pParticleEmitter->setPosition(pOrigemDesenho->getPosition());
		_pParticleEmitter->renderExecute(true);
	}

}

void RenderVisitor::visit ( SceneRoot* _pSceneRoot ) {
    
    _pSceneRoot->apply();

}

void RenderVisitor::visit ( Group* _pGroup ) {

}

void RenderVisitor::visit ( Chimera::Transform* _pTransform) {
    
    _pTransform->applyTransform();
    
}

void RenderVisitor::visit ( Solid* _pSolid ) {
    
    _pSolid->apply(pOrigemDesenho);

}

void RenderVisitor::visit ( Chimera::Coord* ) {

}

void RenderVisitor::visit ( HUD* _pHUD ) {

	if (HudOn == true) {
		pVideo->executeViewOrto(eye);
		_pHUD->renderExecute(textureOn);
		pVideo->restoreMatrix();
	}
    
}

}