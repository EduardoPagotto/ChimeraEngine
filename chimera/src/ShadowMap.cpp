#include "Video.h"
#include "ShadowMap.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// These store our width and height for the shadow texture.  The higher the
// texture size the better quality shadow.  Must be power of two for most video cards.
# define SHADOW_WIDTH 512
# define SHADOW_HEIGHT 512

namespace Chimera {

ShadowMap::ShadowMap() {

   	pTexture = new Texture("Shadow-Map-Tex_" +  std::to_string(Entity::getNextSerialMaster() + 1) ,"FRAME_BUFFER", 1); //FIXME mudar o shadowmap para um count maior
   	depthMapFBO = pTexture->createTextureFrameBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);

	simpleDepthShader = Singleton<Shader>::getRefSingleton();
}

ShadowMap::~ShadowMap() {
	Singleton<Shader>::releaseRefSingleton();
}

glm::mat4 ShadowMap::calcLightSpaceMatrices(const glm::vec3 &_posicaoLight ) {

	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	//lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
	glm::mat4 lightView = glm::lookAt(_posicaoLight, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	return lightSpaceMatrix;
}

void ShadowMap::createLightViewPosition(const glm::vec3 &_posicaoLight) {

	glm::mat4  lightSpaceMatrix = calcLightSpaceMatrices(_posicaoLight);

	simpleDepthShader->selectCurrent("simpleDepthShader");
	simpleDepthShader->link();
	simpleDepthShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	initSceneShadow();
}

void ShadowMap::initSceneShadow() {

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::endSceneShadow() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowMap::applyShadow() {

	pTexture->apply();

}

}
