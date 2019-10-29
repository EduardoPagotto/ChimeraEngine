#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

ShadowMapVisitor::ShadowMapVisitor(Shader* _pShader, ShadowMap* _pShadowMap) {
    pShader = _pShader;
    shadowMap = _pShadowMap;
    pTransform = nullptr;
}

ShadowMapVisitor::~ShadowMapVisitor() {}

void ShadowMapVisitor::init() {
    if (shadowMap != nullptr)
        shadowMap->init();
}

void ShadowMapVisitor::visit(Camera* _pCamera) {}

void ShadowMapVisitor::visit(Mesh* _pMesh) {

    model = _pMesh->getTransform()->getModelMatrix(pTransform->getPosition());

    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
    _pMesh->render(nullptr);
}

void ShadowMapVisitor::visit(Light* _pLight) {

    glm::mat4 lightSpaceMatrix = shadowMap->createLightSpaceMatrix(_pLight->getPosition());
    pShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}

void ShadowMapVisitor::visit(ParticleEmitter* _pParticleEmitter) {}

void ShadowMapVisitor::visit(Group* _pGroup) {
    // Shader selecionado correto no RenderVisitor via Group
    pShader->link();
}

void ShadowMapVisitor::visit(HUD* _pHUD) {}
} // namespace Chimera
