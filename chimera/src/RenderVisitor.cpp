#include "RenderVisitor.h"

namespace Chimera {
   
RenderVisitor::RenderVisitor() {

	pCoord = nullptr;
	pVideo = nullptr;
	textureOn = true;
	HudOn = true;
	particleOn = true;
	eye = 0;

	projection = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
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
		_pParticleEmitter->setPosition(pCoord->getPosition());
		_pParticleEmitter->renderExecute(true);
	}

}

void RenderVisitor::visit ( SceneRoot* _pSceneRoot ) {
    
    _pSceneRoot->apply();

}

void RenderVisitor::visit ( Group* _pGroup ) {

	_pGroup->apply(view, projection);

}

void RenderVisitor::visit ( Chimera::Transform* _pTransform) {
    
    model = _pTransform->getMatrix();//applyTransform();
    glMultMatrixf(glm::value_ptr(model));
}

void RenderVisitor::visit ( Solid* _pSolid ) {
    
	//TODO acumular esta matriz 
    model = _pSolid->getModelMatrix(pCoord);
    glMultMatrixf(glm::value_ptr(model));

}

void RenderVisitor::visit ( HUD* _pHUD ) {

	if (HudOn == true) {
		if (_pHUD->isOn() == true) {
			pVideo->executeViewOrto(eye);
			_pHUD->renderExecute(textureOn);
			pVideo->restoreMatrix();
		}
	}  
}

}