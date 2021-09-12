#ifndef __CHIMERA_VISITOR_SHADOW_MAP__HPP
#define __CHIMERA_VISITOR_SHADOW_MAP__HPP

#include "Node.hpp"
#include "VisitorInterface.hpp"
#include "chimera/core/Shader.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/render/Texture.hpp"
#include "chimera/render/vbs/RenderableStatic3d.hpp"
#include <string>

namespace Chimera {

class VisitorShadowMap : public VisitorInterface {
  public:
    VisitorShadowMap(SimpleRender3d* _pRender3d, Shader* _pShader, const unsigned& _width, const unsigned& _height);
    virtual ~VisitorShadowMap();

    virtual void init() override;
    virtual void visit(class NodeCamera* _pCamera) override;
    virtual void visit(class NodeMesh* _pMesh) override;
    virtual void visit(class NodeLight* _pLight) override;
    virtual void visit(class NodeParticleEmitter* _pParticleEmitter) override;
    virtual void visit(class NodeGroup* _pGroup) override;
    virtual void visit(class NodeHUD* _pHUD) override;

    void render(Node* _pGroup, Transform* _pTransform);
    void applyShadow(Shader* _pShader);
    inline glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }

  private:
    void initSceneShadow();
    void endSceneShadow();
    void setLightSpaceMatrix(const glm::vec3& _posicaoLight);

    glm::mat4 lightSpaceMatrix;
    Transform* pTransform;
    Shader* pShader;
    TextureFBO* pTexture;
    SimpleRender3d* pRender3D;
};
} // namespace Chimera

#endif
