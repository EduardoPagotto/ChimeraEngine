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

	shader =  Singleton<Shader>::getRefSingleton();
}

RenderVisitor::~RenderVisitor() {

	Singleton<Shader>::releaseRefSingleton();

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
		//_pParticleEmitter->setPosition(pCoord->getPosition()); //FIXME esta errado, verificar como fazer em debug
        _pParticleEmitter->ProjectionMatrix = projection;
        _pParticleEmitter->ViewMatrix = view;
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

	//TODO acumular esta matriz
    model = _pTransform->getMatrix();
	shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model) );

}

void RenderVisitor::visit ( Solid* _pSolid ) {

	//TODO acumular esta matriz
    model = _pSolid->getModelMatrix(pCoord);
	shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model) );


}

void RenderVisitor::visit ( HUD* _pHUD ) {


	if (HudOn == true) {
		if (_pHUD->isOn() == true) {

            projection = pVideo->getOrthoProjectionMatrix(eye); //pVideo->executeViewOrto(eye);

			shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(_pHUD->projection));

            _pHUD->projection = projection;
			_pHUD->renderExecute(textureOn);
			//pVideo->restoreMatrix();
		}
	}
}

}
