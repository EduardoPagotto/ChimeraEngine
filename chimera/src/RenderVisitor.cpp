#include "RenderVisitor.h"

namespace Chimera {

RenderVisitor::RenderVisitor() {

	pCoord = nullptr;
	pVideo = nullptr;
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

	_pCamera->apply();

	projection = pVideo->getPerspectiveProjectionMatrix(_pCamera->getFov(), _pCamera->getNear(), _pCamera->getFar(), eye);
	view = _pCamera->getViewMatrix();

}

void RenderVisitor::visit ( Mesh* _pMesh ) {

    _pMesh->render(projection, view, model);

}

void RenderVisitor::visit ( Light* _pLight ) {

	_pLight->apply();

}

void RenderVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {

	if (particleOn == true) {
		_pParticleEmitter->render(projection, view, model);
	}

}

void RenderVisitor::visit ( SceneRoot* _pSceneRoot ) {

    _pSceneRoot->apply();

}

void RenderVisitor::visit ( Group* _pGroup ) {

	_pGroup->apply(view, projection);

}

void RenderVisitor::visit ( Chimera::Transform* _pTransform) {

	//TODO acumular esta matriz
	model = _pTransform->getModelMatrix(pCoord);
}

void RenderVisitor::visit ( Solid* _pSolid ) {

	//TODO acumular esta matriz
    model = _pSolid->getModelMatrix(pCoord);
}

void RenderVisitor::visit ( HUD* _pHUD ) {

	if (HudOn == true) {
		if (_pHUD->isOn() == true) {
			_pHUD->render(pVideo->getOrthoProjectionMatrix(eye), view, model);
			//pVideo->restoreMatrix();
		}
	}
}

}
