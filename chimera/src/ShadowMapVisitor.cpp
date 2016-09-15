#include "ShadowMapVisitor.h"

#include "OpenGLDefs.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "SceneRoot.h"
#include "Group.h"
#include "Transform.h"
#include "Solid.h"
#include "HUD.h"

#include "NodeParse.h"

namespace Chimera {

ShadowMapVisitor::ShadowMapVisitor(const std::string _name, const unsigned &_width, const unsigned &_height) : NodeVisitor() {

	shadowMap = new ShadowMap(_name, _width, _height);
	simpleDepthShader = Singleton<Shader>::getRefSingleton();
}

ShadowMapVisitor::~ShadowMapVisitor() {

	delete shadowMap;
	shadowMap = nullptr;

	Singleton<Shader>::releaseRefSingleton();
}

void ShadowMapVisitor::execute(Node *_pNode) {

	Light *nodeLight = (Light*)_pNode->findChild(Chimera::EntityKind::LIGHT, 0, false);

	glm::mat4 lightSpaceMatrix = shadowMap->createLightSpaceMatrix( nodeLight->getPosition() );

	simpleDepthShader->selectCurrent("simpleDepthShader");
	simpleDepthShader->link();
	simpleDepthShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	shadowMap->initSceneShadow();

	NodeParse::tree(_pNode,this);

	shadowMap->endSceneShadow();
}

// void ShadowMapVisitor::execute(Node * _node, const unsigned &_eye)
// {
// 	//eye = _eye;
//
// 	Light *nodeLight = (Light*)_node->findChild(Chimera::EntityKind::LIGHT, 0, true);
// 	//Camera *nodeCam = (Camera*)_node->findChild(Chimera::EntityKind::CAMERA, 0, true);
//
//
// 	createLightViewPosition( nodeLight->getPosition() );
//
// 	//HudOn = false;
// 	//particleOn = false;
// 	//runningShadow = true;
//
// 	DFS(_node);
//
// 	endSceneShadow();
// }

// glm::mat4 ShadowMapVisitor::calcLightSpaceMatrices(const glm::vec3 &_posicaoLight ) {
//
// 	GLfloat near_plane = 1.0f, far_plane = 7.5f;
// 	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
// 	//lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
// 	glm::mat4 lightView = glm::lookAt(_posicaoLight, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
// 	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
//
// 	return lightSpaceMatrix;
// }

// void ShadowMapVisitor::createLightViewPosition(const glm::vec3 &_posicaoLight) {
//
// 	glm::mat4  lightSpaceMatrix = calcLightSpaceMatrices(_posicaoLight);
//
// 	simpleDepthShader->selectCurrent("simpleDepthShader");//FIXME mudar para o construtor
// 	simpleDepthShader->link();
// 	simpleDepthShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
//
// 	initSceneShadow();
// }

// void ShadowMapVisitor::initSceneShadow() {
//
// 	glViewport(0, 0, pTexture->getWidth(), pTexture->getHeight() );
// 	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
// 	glClear(GL_DEPTH_BUFFER_BIT);
// 	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// }
//
// void ShadowMapVisitor::endSceneShadow() {
//
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
// }

void ShadowMapVisitor::visit ( Camera* _pCamera ) {

	//shader->setGlUniform3fv("viewPos", 1, glm::value_ptr( _pCamera->getPosition() ));
	//projection = pVideo->getPerspectiveProjectionMatrix(_pCamera->getFov(), _pCamera->getNear(), _pCamera->getFar(), eye);
	//view = _pCamera->getViewMatrix();

}

void ShadowMapVisitor::visit ( Mesh* _pMesh ) {

	//int shadows = 1;
	//shader->setGlUniform1i("shadows", shadows); //glUniform1i(glGetUniformLocation(shader.Program, "shadows"), shadows);

	// Get the variables from the shader to which data will be passed
	//shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
	//shader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
	//shader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
	//shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr( glm::inverseTranspose(glm::mat3(_view))));

	//if (runningShadow == false)
	//	_pMesh->getMaterial()->apply();

	//if (shadoMap != nullptr) {
	//	if (runningShadow == false) {
	//		shadoMap->applyShadow();
	//		shader->setGlUniform1i("shadowMap", shadoMap->getTextureIndex());
	//	}
	//}

    //////_pMesh->render(projection, view, model);

}

void ShadowMapVisitor::visit ( Light* _pLight ) {
/*
	shader->setGlUniform3fv("light.position", 1, glm::value_ptr(_pLight->getPosition()));
	shader->setGlUniform4fv("light.ambient", 1, _pLight->getAmbient().ptr());
	shader->setGlUniform4fv("light.diffuse", 1, _pLight->getDiffuse().ptr());
	shader->setGlUniform4fv("light.specular", 1, _pLight->getSpecular().ptr());
*/
}

void ShadowMapVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {
}

void ShadowMapVisitor::visit ( SceneRoot* _pSceneRoot ) {

    //_pSceneRoot->apply();

}

void ShadowMapVisitor::visit ( Group* _pGroup ) {

// 	Light *nodeLight = (Light*)_pGroup->findChild(Chimera::EntityKind::LIGHT, 0, false);
//
// 	glm::mat4 lightSpaceMatrix = shadowMap->createLightSpaceMatrix( nodeLight->getPosition() );
//
// 	simpleDepthShader->selectCurrent("simpleDepthShader");
// 	simpleDepthShader->link();
// 	simpleDepthShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
//
// 	shadowMap->initSceneShadow();


	//createLightViewPosition( nodeLight->getPosition() );

/*
	if (runningShadow == false) {

		shader->selectCurrent(_pGroup->getShaderName());
		shader->link();

		//shader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	} else  {


		//shader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    }

  */

}

void ShadowMapVisitor::visit ( Chimera::Transform* _pTransform) {

	//TODO acumular esta matriz
	model = _pTransform->getModelMatrix(pCoord);
}

void ShadowMapVisitor::visit ( Solid* _pSolid ) {

	//TODO acumular esta matriz
    model = _pSolid->getModelMatrix(pCoord);
}

void ShadowMapVisitor::visit ( HUD* _pHUD ) {
/*
	if (HudOn == true) {
		if (_pHUD->isOn() == true) {

			shader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr( pVideo->getOrthoProjectionMatrix(eye) ));
			_pHUD->render(projection, view, model);
			//pVideo->restoreMatrix();
		}
	}
*/
}

}

