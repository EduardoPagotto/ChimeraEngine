#ifndef __CHIMERA_SHADOW_MAP_VISITOR__HPP
#define __CHIMERA_SHADOW_MAP_VISITOR__HPP

#include <string>
//#include <glm/glm.hpp>

#include "Coord.h"
#include "NodeVisitor.h"
#include "Shader.h"
#include "ShadowMap.h"

namespace Chimera {

class ShadowMapVisitor : public NodeVisitor {
  public:
    ShadowMapVisitor(const std::string _name, const unsigned int& _width,
                     const unsigned int& _height, Shader* _pShader);
    virtual ~ShadowMapVisitor();

    ShadowMap* execute(Node* _pNode);

    void init();

    virtual void visit(class Camera* _pCamera) override;
    virtual void visit(class Mesh* _pMesh) override;
    virtual void visit(class Light* _pLight) override;
    virtual void visit(class ParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class SceneMng* _pSceneMng) override;
    virtual void visit(class Group* _pGroup) override;
    virtual void visit(class Transform* _pTransform) override;
    virtual void visit(class Solid* _pSolid) override;
    virtual void visit(class HUD* _pHUD) override;

    Coord* pCoord;

  private:
    ShadowMap* shadowMap;
    Shader* pShader;
    glm::mat4 model;

    // glm::mat4 calcLightSpaceMatrices ( const glm::vec3 &_posicaoLight );
    // void createLightViewPosition(const glm::vec3 & _posicaoLight);
    // void initSceneShadow();
    // void endSceneShadow();
    // GLuint depthMapFBO;
    // Texture *pTexture;
};
} // namespace Chimera

#endif
