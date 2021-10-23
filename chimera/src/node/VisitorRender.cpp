#include "chimera/node/VisitorRender.hpp"
#include "chimera/core/OpenGLDefs.hpp"
#include "chimera/core/windows/CanvasHmd.hpp"
#include "chimera/node/NodeCamera.hpp"
#include "chimera/node/NodeGroup.hpp"
#include "chimera/node/NodeHUD.hpp"
#include "chimera/node/NodeLight.hpp"
#include "chimera/node/NodeMesh.hpp"
#include "chimera/node/NodeParticleEmitter.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

VisitorRender::VisitorRender() {

    pTransform = nullptr;
    pVideo = nullptr;
    HudOn = true;
    particleOn = true;
    eye = 0;

    model = glm::mat4(1.0f);

    pShadowMapVisitor = nullptr;
}

VisitorRender::~VisitorRender() {}

void VisitorRender::init() {}

void VisitorRender::visit(NodeCamera* _pCamera) {}

void VisitorRender::visit(NodeMesh* _pMesh) {

    model = _pMesh->getTransform()->getModelMatrix(pTransform->getPosition());
    if (shader.isInvalid())
        return;

    int shadows = 1;
    shader.setUniform("shadows", shadows);
    shader.setUniform("projection", cameraScene->getProjectionMatrix());
    shader.setUniform("view", cameraScene->getViewMatrix());
    shader.setUniform("model", model);

    _pMesh->getMaterial()->bindMaterialInformation(shader);

    if (pShadowMapVisitor != nullptr) {
        if (!shader.isInvalid())
            shader.setUniform("shadowMap", 1);

        pShadowMapVisitor->bindDepthBuffer();
    }
    render3D.begin(nullptr);
    _pMesh->pRenderStat->submit(&render3D); // render3D.submit(_pMesh->pRenderStat);
    render3D.end();

    render3D.flush();
}

void VisitorRender::visit(NodeLight* _pLight) { _pLight->data.bindLightInformation(shader); }

void VisitorRender::visit(NodeParticleEmitter* _pParticleEmitter) {

    if (particleOn == true) {

        model = _pParticleEmitter->getTransform()->getModelMatrix(pTransform->getPosition());

        if (shader.isInvalid())
            return;

        const glm::mat4 view = cameraScene->getViewMatrix();
        shader.setUniform("projection", cameraScene->getProjectionMatrix());
        shader.setUniform("view", view);
        // shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr(
        // glm::inverseTranspose(glm::mat3(_view))));

        shader.setUniform("model", model);

        // We will need the camera's position in order to sort the particles
        // w.r.t the camera's distance.
        // There should be a getCameraPosition() function in common/controls.cpp,
        // but this works too.
        glm::vec3 CameraPosition(glm::inverse(view)[3]);

        // Vertex shader
        shader.setUniform("CameraRight_worldspace", glm::vec3(view[0][0], view[1][0], view[2][0]));
        shader.setUniform("CameraUp_worldspace", glm::vec3(view[0][1], view[1][1], view[2][1]));

        _pParticleEmitter->CameraPosition = CameraPosition;
        _pParticleEmitter->render(shader);
    }
}

void VisitorRender::visit(NodeGroup* _pGroup) {

    if (_pGroup->getShader() == nullptr) {
        shader = Shader();
        return;
    }

    shader = *_pGroup->getShader();

    // Renderiza o Textura de sombra em ShadowMapVisitor
    pShadowMapVisitor = (VisitorShadowMap*)_pGroup->getNodeVisitor();
    if (pShadowMapVisitor != nullptr)
        pShadowMapVisitor->render(_pGroup, pTransform);

    shader.enable();

    if (pShadowMapVisitor != nullptr) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setUniform("lightSpaceMatrix", pShadowMapVisitor->getLightSpaceMatrix());
    }

    NodeCamera* pCam = (NodeCamera*)_pGroup->findChild(Chimera::Kind::CAMERA, 0, false);
    if (pCam != nullptr) {
        cameraScene = pCam->getCamera();
        cameraScene->processInput(0.01);

        shader.setUniform("viewPos", cameraScene->getPosition());
        if (pVideo->getTotEyes() == 1) {
            glViewport(0, 0, pVideo->getWidth(), pVideo->getHeight());
            cameraScene->setAspectRatio(pVideo->getWidth(), pVideo->getHeight());
            cameraScene->recalculateMatrix(false);
        } else {
            Eye* pEye = ((CanvasHmd*)pVideo)->getEye(eye);
            glViewport(0, 0, pEye->fbTexGeo.w, pEye->fbTexGeo.h);
            cameraScene->setAspectRatio(pEye->fbTexGeo.w, pEye->fbTexGeo.h);
            if (eye == 0) { // right
                cameraScene->recalculateMatrix(false);
            } else { // left
                cameraScene->recalculateMatrix(true);
            }
        }
    }
}

void VisitorRender::visit(NodeHUD* _pHUD) {

    if (HudOn == true) {
        if (_pHUD->isOn() == true) {
            glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(pVideo->getWidth()), 0.0f, static_cast<GLfloat>(pVideo->getHeight()));
            shader.setUniform("projection", proj);
            _pHUD->render(&shader);
        }
    }
}
} // namespace Chimera
