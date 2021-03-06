#include "chimera/node/VisitorRender.hpp"
#include "chimera/OpenGLDefs.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/NodeParticleEmitter.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

VisitorRender::VisitorRender() {

    pTransform = nullptr;
    pVideo = nullptr;
    HudOn = true;
    particleOn = true;
    eye = 0;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    pShadowMapVisitor = nullptr;
    pShader = nullptr;
}

VisitorRender::~VisitorRender() {}

void VisitorRender::init() {}

void VisitorRender::visit(NodeCamera* _pCamera) {}

void VisitorRender::visit(NodeMesh* _pMesh) {

    model = _pMesh->getTransform()->getModelMatrix(pTransform->getPosition());
    if (pShader == nullptr)
        return;

    int shadows = 1;
    pShader->setGlUniform1i("shadows", shadows);
    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    _pMesh->getMaterial()->apply(pShader);

    if (pShadowMapVisitor != nullptr)
        pShadowMapVisitor->applyShadow(pShader);

    // AABB aabbT = _pMesh->meshData.aabb.transformation(model);
    // aabbT.render();

    _pMesh->render(pShader);
}

void VisitorRender::visit(NodeLight* _pLight) { _pLight->data.apply(pShader); }

void VisitorRender::visit(NodeParticleEmitter* _pParticleEmitter) {

    if (particleOn == true) {

        model = _pParticleEmitter->getTransform()->getModelMatrix(pTransform->getPosition());

        if (pShader == nullptr)
            return;

        // Get the variables from the shader to which data will be passed
        pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
        pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
        // shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr(
        // glm::inverseTranspose(glm::mat3(_view))));

        pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

        // We will need the camera's position in order to sort the particles
        // w.r.t the camera's distance.
        // There should be a getCameraPosition() function in common/controls.cpp,
        // but this works too.
        glm::vec3 CameraPosition(glm::inverse(view)[3]);

        // Vertex shader
        pShader->setGlUniform3f("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
        pShader->setGlUniform3f("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

        _pParticleEmitter->CameraPosition = CameraPosition;
        _pParticleEmitter->render(pShader);
    }
}

void VisitorRender::visit(NodeGroup* _pGroup) {

    pShader = _pGroup->getShader();
    if (pShader == nullptr)
        return;

    // Renderiza o Textura de sombra em ShadowMapVisitor
    pShadowMapVisitor = (VisitorShadowMap*)_pGroup->getNodeVisitor();
    if (pShadowMapVisitor != nullptr)
        pShadowMapVisitor->render(_pGroup, pTransform);

    pShader->link();

    if (pShadowMapVisitor != nullptr) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE,
                                       glm::value_ptr(pShadowMapVisitor->getLightSpaceMatrix()));
    }

    NodeCamera* pCam = (NodeCamera*)_pGroup->findChild(Chimera::Kind::CAMERA, 0, false);
    if (pCam != nullptr) {
        ViewPoint* vp = pCam->getViewPoint();
        pShader->setGlUniform3fv("viewPos", 1, glm::value_ptr(vp->position));
        pVideo->calcPerspectiveProjectionView(eye, vp, view, projection);
    }
}

void VisitorRender::visit(NodeHUD* _pHUD) {

    if (HudOn == true) {
        if (_pHUD->isOn() == true) {
            pShader->setGlUniformMatrix4fv("projection", 1, false,
                                           glm::value_ptr(pVideo->getOrthoProjectionMatrix(eye)));
            _pHUD->render(pShader);
        }
    }
}
} // namespace Chimera
