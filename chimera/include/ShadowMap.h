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

	void init();

	glm::mat4 createLightSpaceMatrix(const glm::vec3 & _posicaoLight);

	void initSceneShadow();
	void endSceneShadow();
	void applyShadow();

	int getTextureIndex() {
		return 1;
	}

	glm::mat4 lightSpaceMatrix;

private:
    Texture *pTexture;
};

}

# endif
