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
    pShader->setUniform1i("shadows", shadows);
    pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

    _pMesh->getMaterial()->setUniform(pShader);

    if (pShadowMapVisitor != nullptr)
        pShadowMapVisitor->applyShadow(pShader);

    // AABB aabbT = _pMesh->meshData.aabb.transformation(model);
    // aabbT.render();

    render3D.submit(_pMesh->pRenderStat);
    render3D.flush();
}

void VisitorRender::visit(NodeLight* _pLight) { _pLight->data.setUniform(pShader); }

void VisitorRender::visit(NodeParticleEmitter* _pParticleEmitter) {

    if (particleOn == true) {

        model = _pParticleEmitter->getTransform()->getModelMatrix(pTransform->getPosition());

        if (pShader == nullptr)
            return;

        // Get the variables from the shader to which data will be passed
        pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
        pShader->setUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
        // shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr(
        // glm::inverseTranspose(glm::mat3(_view))));

        pShader->setUniformMatrix4fv("model", 1, false, glm::value_ptr(model));

        // We will need the camera's position in order to sort the particles
        // w.r.t the camera's distance.
        // There should be a getCameraPosition() function in common/controls.cpp,
        // but this works too.
        glm::vec3 CameraPosition(glm::inverse(view)[3]);

        // Vertex shader
        pShader->setUniform3f("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
        pShader->setUniform3f("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);

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

    pShader->enable();

    if (pShadowMapVisitor != nullptr) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pShader->setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(pShadowMapVisitor->getLightSpaceMatrix()));
    }

    NodeCamera* pCam = (NodeCamera*)_pGroup->findChild(Chimera::Kind::CAMERA, 0, false);
    if (pCam != nullptr) {
        ViewPoint* vp = pCam->getViewPoint();
        pShader->setUniform3fv("viewPos", 1, glm::value_ptr(vp->position));
        pVideo->calcPerspectiveProjectionView(eye, vp, view, projection);
    }
}

void VisitorRender::visit(NodeHUD* _pHUD) {

    if (HudOn == true) {
        if (_pHUD->isOn() == true) {
            pShader->setUniformMatrix4fv("projection", 1, false, glm::value_ptr(pVideo->getOrthoProjectionMatrix(eye)));
            _pHUD->render(pShader);
        }
    }
}
} // namespace Chimera
