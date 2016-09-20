#include "RenderVisitor.h"

#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "SceneRoot.h"
#include "Group.h"
#include "Transform.h"
#include "Solid.h"
#include "HUD.h"

#include "OpenGLDefs.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	shadowOn = nullptr;

	shader =  Singleton<Shader>::getRefSingleton();
}

RenderVisitor::~RenderVisitor() {

	Singleton<Shader>::releaseRefSingleton();
}

void RenderVisitor::visit ( Camera* _pCamera ) {

}

void RenderVisitor::visit ( Mesh* _pMesh ) {

	int shadows = 1;
	shader->setGlUniform1i("shadows", shadows); //glUniform1i(glGetUniformLocation(shader.Program, "shadows"), shadows);

	// Get the variables from the shader to which data will be passed
	shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
	shader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
	shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
	//shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

	_pMesh->getMaterial()->apply();

	if (shadowOn != nullptr) {
		shadowOn->applyShadow();
		shader->setGlUniform1i("shadowMap", (int)shadowOn->getShadowIndexTextureSeq());
	}

    _pMesh->render();

}

void RenderVisitor::visit ( Light* _pLight ) {

}

void RenderVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {

	if (particleOn == true) {

		// Get the variables from the shader to which data will be passed
		shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
		shader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
		//shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

		shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

		// We will need the camera's position in order to sort the particles
		// w.r.t the camera's distance.
		// There should be a getCameraPosition() function in common/controls.cpp,
		// but this works too.
		glm::vec3 CameraPosition(glm::inverse(view)[3]);

		// Vertex shader
		shader->setGlUniform3f("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
		shader->setGlUniform3f("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

		// fragment shader
		shader->setGlUniform1i("myTextureSampler", 0);

		_pParticleEmitter->CameraPosition = CameraPosition;
		_pParticleEmitter->render();
	}

}

void RenderVisitor::visit ( SceneRoot* _pSceneRoot ) {

    _pSceneRoot->apply();

}

void RenderVisitor::visit ( Group* _pGroup ) {

	shadowOn = _pGroup->executeShadoMap(pCoord);

	shader->selectCurrent(_pGroup->getShaderName());
	shader->link();

	if (shadowOn != nullptr) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(shadowOn->lightSpaceMatrix));

	}

	Camera *pCam = (Camera*)_pGroup->findChild(Chimera::EntityKind::CAMERA, 0, false);
	if (pCam != nullptr) {
		shader->setGlUniform3fv("viewPos", 1, glm::value_ptr(pCam->getPosition()));
		projection = pVideo->getPerspectiveProjectionMatrix(pCam->getFov(), pCam->getNear(), pCam->getFar(), eye);
		view = pCam->getViewMatrix();
	}

	Light *pLight = (Light*)_pGroup->findChild(Chimera::EntityKind::LIGHT, 0, false);
	if (pLight != nullptr) {

		shader->setGlUniform3fv("light.position", 1, glm::value_ptr(pLight->getPosition()));
		shader->setGlUniform4fv("light.ambient", 1, pLight->getAmbient().ptr());
		shader->setGlUniform4fv("light.diffuse", 1, pLight->getDiffuse().ptr());
		shader->setGlUniform4fv("light.specular", 1, pLight->getSpecular().ptr());

	}
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

			shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr( pVideo->getOrthoProjectionMatrix(eye) ));
			_pHUD->render();
		}
	}
}

}
