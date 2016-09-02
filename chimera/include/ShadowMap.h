#ifndef SHADOW_MAP_H_
#define SHADOW_MAP_H_

#include <glm/glm.hpp>
#include "Texture.h"
#include "Node.h"

#include "Shader.h"


namespace Chimera
{

class ShadowMap
{

public:
    ShadowMap();
    virtual ~ShadowMap();

	glm::mat4 calcLightSpaceMatrices ( const glm::vec3 &_posicaoLight );

	void initSceneShadow();
	void endSceneShadow();

private:
	GLuint depthMapFBO;
    Texture *pTexture;
	Shader *simpleDepthShader;
};

}

# endif
