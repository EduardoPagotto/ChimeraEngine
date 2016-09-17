#include "Video.h"
#include "ShadowMap.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

ShadowMap::ShadowMap(std::string _name, const unsigned &_width, const unsigned &_height) {

	pTexture = new Texture("Shadow-Map-Tex_" + _name, _width, _height);

}

ShadowMap::~ShadowMap() {

	delete pTexture;
	pTexture = nullptr;
}

void ShadowMap::init()
{
	pTexture->init();
}

glm::mat4 ShadowMap::createLightSpaceMatrix(const glm::vec3 &_posicaoLight) {

	GLfloat near_plane = 1.0f, far_plane = 150.0f;
	glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
	//glm::mat4 lightProjection = glm::perspective(45.0f, (GLfloat)pTexture->getWidth() / (GLfloat)pTexture->getHeight(), near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
	glm::mat4 lightView = glm::lookAt(_posicaoLight, glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
	lightSpaceMatrix = lightProjection * lightView;

	return lightSpaceMatrix;
}

void ShadowMap::initSceneShadow() {

	glViewport(0, 0, pTexture->getWidth(), pTexture->getHeight() );
	glBindFramebuffer(GL_FRAMEBUFFER, pTexture->getFrameBufferId());
	glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::endSceneShadow() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowMap::applyShadow() {

	pTexture->apply(1);

}

}
