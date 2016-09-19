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

ShadowMapVisitor::ShadowMapVisitor(const std::string _name, const unsigned int & _width, const unsigned int & _height)
{
	shadowMap = new ShadowMap(_name, _width, _height);
	simpleDepthShader = Singleton<Shader>::getRefSingleton();
}

ShadowMapVisitor::~ShadowMapVisitor() {

	delete shadowMap;
	shadowMap = nullptr;

	Singleton<Shader>::releaseRefSingleton();
}

ShadowMap* ShadowMapVisitor::execute(Node *_pNode) {

	Light *nodeLight = (Light*)_pNode->findChild(Chimera::EntityKind::LIGHT, 0, false);

	glm::mat4 lightSpaceMatrix = shadowMap->createLightSpaceMatrix( nodeLight->getPosition() );

	simpleDepthShader->selectCurrent("simpleDepthShader");
	simpleDepthShader->link();
	simpleDepthShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	shadowMap->initSceneShadow();

	NodeParse::tree(_pNode,this);

	shadowMap->endSceneShadow();

	return shadowMap;
}

void ShadowMapVisitor::init()
{
	shadowMap->init();
}

void ShadowMapVisitor::visit ( Camera* _pCamera ) {

	//shader->setGlUniform3fv("viewPos", 1, glm::value_ptr( _pCamera->getPosition() ));
	//projection = pVideo->getPerspectiveProjectionMatrix(_pCamera->getFov(), _pCamera->getNear(), _pCamera->getFar(), eye);
	//view = _pCamera->getViewMatrix();

}

void ShadowMapVisitor::visit ( Mesh* _pMesh ) {

	simpleDepthShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
    _pMesh->render();

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
}

void ShadowMapVisitor::visit ( Group* _pGroup ) {
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
}

}

