#include "chimera/node/VisitorShadowMap.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/NodeParticleEmitter.hpp"
#include "chimera/node/VisitParser.hpp"
#include "chimera/render/OpenGLDefs.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

VisitorShadowMap::VisitorShadowMap(SimpleRender3d* _pRender3d, Shader* _pShader, const unsigned& _width, const unsigned& _height)
    : shader(*_pShader), pTransform(nullptr), pRender3D(_pRender3d) {

    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT)};

    fbSpec.width = _width;
    fbSpec.height = _height;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    frameBufferDepth = new FrameBuffer(fbSpec);
}

VisitorShadowMap::~VisitorShadowMap() { frameBufferDepth; }

void VisitorShadowMap::init() {}

void VisitorShadowMap::visit(NodeCamera* _pCamera) {}

void VisitorShadowMap::visit(NodeMesh* _pMesh) {

    glm::mat4 model = _pMesh->getTransform()->translate(pTransform->getPosition());

    // glm::mat4 projectionMatrixInverse = glm::inverse(glm::mat4(1.0f));
    // glm::mat4 viewMatrixInverse = glm::inverse(view);
    // glm::mat4 viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    // frustum.set(viewProjectionMatrixInverse);

    shader.setUniform("model", model);

    pRender3D->begin(nullptr);
    _pMesh->pRenderStat->submit(pRender3D); // render3D.submit(_pMesh->pRenderStat);
    pRender3D->end();

    pRender3D->flush();
}

void VisitorShadowMap::visit(NodeLight* _pLight) {

    glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 150.0f);
    // Note that if you use a
    // glm::mat4 lightProjection = glm::perspective(45.0f,
    //                                              (GLfloat)pTexture->getWidth() /(GLfloat)pTexture->getHeight(),
    //                                              near_plane,
    //                                              far_plane);
    // perspective projection matrix you'll have to change the light position as the
    // current light position isn't enough to reflect the whole scene.

    glm::mat4 lightView = glm::lookAt(_pLight->data.getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
    this->lightSpaceMatrix = lightProjection * lightView;

    shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
}

void VisitorShadowMap::visit(NodeParticleEmitter* _pParticleEmitter) {}

void VisitorShadowMap::visit(NodeGroup* _pGroup) {
    // Shader selecionado correto no RenderVisitor via Group
    shader.enable();
}

void VisitorShadowMap::visit(NodeHUD* _pHUD) {}

void VisitorShadowMap::render(Node* _pGroup, Transform* _pTransform) {
    this->pTransform = _pTransform;
    frameBufferDepth->bind();
    visitParserTree(_pGroup, this);
    frameBufferDepth->unbind();
}

void VisitorShadowMap::bindDepthBuffer() { frameBufferDepth->getDepthAttachemnt()->bind(1); } // getTexture()->bind(1); }
} // namespace Chimera
