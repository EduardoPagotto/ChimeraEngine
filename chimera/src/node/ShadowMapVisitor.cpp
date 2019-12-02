#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/NodeParse.hpp"
#include "chimera/node/NodeParticleEmitter.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

ShadowMapVisitor::ShadowMapVisitor(Shader* _pShader, const unsigned& _width, const unsigned& _height)
    : pShader(_pShader), pTransform(nullptr) {
    pTexture = new TexFBO(TEX_KIND::SHADOWMAP, _width, _height);
}

ShadowMapVisitor::~ShadowMapVisitor() { delete pTexture; }

void ShadowMapVisitor::init() { pTexture->init(); }

void ShadowMapVisitor::visit(NodeCamera* _pCamera) {}

void ShadowMapVisitor::visit(NodeMesh* _pMesh) {

    glm::mat4 model = _pMesh->getTransform()->getModelMatrix(pTransform->getPosition());

    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
    _pMesh->render(nullptr);
}

void ShadowMapVisitor::visit(NodeLight* _pLight) {
    // node de luz deve vir anter para funcionar?!
    this->setLightSpaceMatrix(_pLight->lightData.getPosition());
    pShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}

void ShadowMapVisitor::visit(NodeParticleEmitter* _pParticleEmitter) {}

void ShadowMapVisitor::visit(NodeGroup* _pGroup) {
    // Shader selecionado correto no RenderVisitor via Group
    pShader->link();
}

void ShadowMapVisitor::visit(NodeHUD* _pHUD) {}

void ShadowMapVisitor::render(Node* _pGroup, Transform* _pTransform) {
    this->pTransform = _pTransform;
    this->initSceneShadow();
    NodeParse::tree(_pGroup, this);
    this->endSceneShadow();
}

void ShadowMapVisitor::initSceneShadow() {
    glViewport(0, 0, pTexture->getWidth(), pTexture->getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, pTexture->getFrameBufferId());
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapVisitor::endSceneShadow() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void ShadowMapVisitor::applyShadow(Shader* _pShader) { pTexture->apply(_pShader); }

void ShadowMapVisitor::setLightSpaceMatrix(const glm::vec3& _posicaoLight) {

    GLfloat near_plane = 1.0f, far_plane = 150.0f;
    glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);

    // Note that if you use a
    // glm::mat4 lightProjection = glm::perspective(45.0f,
    //                                              (GLfloat)pTexture->getWidth() /(GLfloat)pTexture->getHeight(),
    //                                              near_plane,
    //                                              far_plane);
    // perspective projection matrix you'll have to change the light position as the
    // current light position isn't enough to reflect the whole scene.

    glm::mat4 lightView = glm::lookAt(_posicaoLight, glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
    this->lightSpaceMatrix = lightProjection * lightView;
}
} // namespace Chimera
