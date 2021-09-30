#include "chimera/node/VisitorShadowMap.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/NodeParticleEmitter.hpp"
#include "chimera/node/VisitParser.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

VisitorShadowMap::VisitorShadowMap(SimpleRender3d* _pRender3d, Shader* _pShader, const unsigned& _width, const unsigned& _height)
    : pShader(_pShader), pTransform(nullptr), pRender3D(_pRender3d) {
    pTexture = new TextureFBO("theShadow", _width, _height);
}

VisitorShadowMap::~VisitorShadowMap() { delete pTexture; }

void VisitorShadowMap::init() { pTexture->init(); }

void VisitorShadowMap::visit(NodeCamera* _pCamera) {}

void VisitorShadowMap::visit(NodeMesh* _pMesh) {

    glm::mat4 model = _pMesh->getTransform()->getModelMatrix(pTransform->getPosition());

    // glm::mat4 projectionMatrixInverse = glm::inverse(glm::mat4(1.0f));
    // glm::mat4 viewMatrixInverse = glm::inverse(view);
    // glm::mat4 viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    // frustum.set(viewProjectionMatrixInverse);

    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    pRender3D->begin(nullptr);
    _pMesh->pRenderStat->submit(pRender3D); // render3D.submit(_pMesh->pRenderStat);
    pRender3D->end();

    pRender3D->flush();
}

void VisitorShadowMap::visit(NodeLight* _pLight) {
    // node de luz deve vir anter para funcionar?!
    this->setLightSpaceMatrix(_pLight->data.getPosition());
    pShader->setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
}

void VisitorShadowMap::visit(NodeParticleEmitter* _pParticleEmitter) {}

void VisitorShadowMap::visit(NodeGroup* _pGroup) {
    // Shader selecionado correto no RenderVisitor via Group
    pShader->enable();
}

void VisitorShadowMap::visit(NodeHUD* _pHUD) {}

void VisitorShadowMap::render(Node* _pGroup, Transform* _pTransform) {
    this->pTransform = _pTransform;
    this->initSceneShadow();
    visitParserTree(_pGroup, this);
    this->endSceneShadow();
}

void VisitorShadowMap::initSceneShadow() {
    glViewport(0, 0, pTexture->getWidth(), pTexture->getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, pTexture->getFrameBufferId());
    glClear(GL_DEPTH_BUFFER_BIT);
}

void VisitorShadowMap::endSceneShadow() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void VisitorShadowMap::applyShadow(Shader* _pShader) {
    // pTexture->apply(_pShader);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, pTexture->getID());
    if (_pShader != nullptr) {
        _pShader->setUniform1i("shadowMap", 1);
    }
}

void VisitorShadowMap::setLightSpaceMatrix(const glm::vec3& _posicaoLight) {

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
