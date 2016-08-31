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

    void StoreLightMatrices ( const glm::vec3 &posicao );

	void initSceneShadow();
	void endSceneShadow();

	void initApplyShadow();
	void endApplyShadow();

    glm::mat4 lightSpaceMatrix;
private:

    Node *pScene;

    Texture *pTexture;
	GLuint depthMapFBO;

	Shader *simpleDepthShader;
};

}

# endif
