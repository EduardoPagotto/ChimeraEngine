#include "ShadowMapVisitor.h"

#include "OpenGLDefs.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"

namespace Chimera {

ShadowMapVisitor::ShadowMapVisitor(const std::string _name, const unsigned &_width, const unsigned &_height) : NodeVisitor() {

	pTexture = new Texture("Shadow-Map-Tex_" + _name + std::string("_") + std::to_string(Entity::getNextSerialMaster() + 1) ,"FRAME_BUFFER", 1); //FIXME mudar o shadowmap para um count maior
   	depthMapFBO = pTexture->createTextureFrameBuffer(_width, _height);
	simpleDepthShader = Singleton<Shader>::getRefSingleton();
}

ShadowMapVisitor::~ShadowMapVisitor() {

	Singleton<Shader>::releaseRefSingleton();
}

void ShadowMapVisitor::execute(Node * _node, const unsigned &_eye)
{
	//eye = _eye;

	Light *nodeLight = (Light*)_node->findChild(Chimera::EntityKind::LIGHT, 0, true);
	//Camera *nodeCam = (Camera*)_node->findChild(Chimera::EntityKind::CAMERA, 0, true);


	createLightViewPosition( nodeLight->getPosition() );

	//HudOn = false;
	//particleOn = false;
	//runningShadow = true;

	DFS(_node);

	endSceneShadow();
}

void ShadowMapVisitor::DFS(Node* u)
{
	u->setColor(1);
	u->accept(this);

	std::vector<Node*>* children = u->getChilds();
	if ((children != nullptr) && (children->size() >0)) {

		for (size_t i = 0; i < children->size(); i++) {
			Node* child = children->at(i);
			if (child->getColor() == 0)
				DFS(child);
		}
	}
	u->setColor(0);
}

glm::mat4 ShadowMapVisitor::calcLightSpaceMatrices(const glm::vec3 &_posicaoLight ) {

	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	//lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
	glm::mat4 lightView = glm::lookAt(_posicaoLight, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	return lightSpaceMatrix;
}

void ShadowMapVisitor::createLightViewPosition(const glm::vec3 &_posicaoLight) {

	glm::mat4  lightSpaceMatrix = calcLightSpaceMatrices(_posicaoLight);

	simpleDepthShader->selectCurrent("simpleDepthShader");
	simpleDepthShader->link();
	simpleDepthShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	initSceneShadow();
}

void ShadowMapVisitor::initSceneShadow() {

	glViewport(0, 0, pTexture->getWidth(), pTexture->getHeight() );
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMapVisitor::endSceneShadow() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

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

    ////////_pMesh->render(projection, view, model);

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
	//model = _pTransform->getModelMatrix(pCoord);
}

void ShadowMapVisitor::visit ( Solid* _pSolid ) {

	//TODO acumular esta matriz
    //model = _pSolid->getModelMatrix(pCoord);
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

