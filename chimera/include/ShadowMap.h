#ifndef SHADOW_MAP_H_
#define SHADOW_MAP_H_

#include <glm/glm.hpp>
#include "Texture.h"
#include "Node.h"

namespace Chimera
{

class ShadowMap {
public:
    ShadowMap(std::string _name, const unsigned &_width, const unsigned &_height);
    virtual ~ShadowMap();

	glm::mat4 createLightSpaceMatrix(const glm::vec3 & _posicaoLight);

	void initSceneShadow();
	void endSceneShadow();
	void applyShadow();

	unsigned getTextureIndex() {
		return pTexture->getCount();
	}

	glm::mat4 lightSpaceMatrix;

private:
	GLuint depthMapFBO;
    Texture *pTexture;
};

}

# endif
