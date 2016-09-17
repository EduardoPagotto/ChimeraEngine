#ifndef __SHADOW_MAP_VISITOR_H__
#define __SHADOW_MAP_VISITOR_H__

#include <string>
//#include <glm/glm.hpp>

#include "NodeVisitor.h"
//#include "Node.h"
#include "Shader.h"
//#include "Texture.h"
#include "Coord.h"
#include "ShadowMap.h"

namespace Chimera {

class ShadowMapVisitor : public NodeVisitor {
public:
    ShadowMapVisitor ( const std::string _name, const unsigned int& _width, const unsigned int& _height );
    virtual ~ShadowMapVisitor();

	ShadowMap* execute(Node *_pNode);

	void init();

    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light *_pLight) override;
    virtual void visit(class ParticleEmitter *_pParticleEmitter) override;
    virtual void visit(class SceneRoot *_pSceneRoot) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class Transform* _pTransform) override;
    virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;

	Coord *pCoord;

private:
	//glm::mat4 calcLightSpaceMatrices ( const glm::vec3 &_posicaoLight );
	//void createLightViewPosition(const glm::vec3 & _posicaoLight);
	//void initSceneShadow();
	//void endSceneShadow();
	ShadowMap *shadowMap;
	Shader *simpleDepthShader;
	//GLuint depthMapFBO;
    //Texture *pTexture;

	glm::mat4 model;
};
}

#endif

