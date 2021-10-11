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
    pShader->setUniform("shadows", shadows);
    pShader->setUniform("projection", projection);
    pShader->setUniform("view", view);
    pShader->setUniform("model", model);

    _pMesh->getMaterial()->bindMaterialInformation(pShader);

    if (pShadowMapVisitor != nullptr)
        pShadowMapVisitor->applyShadow(pShader);

    //
    // glm::mat4 projectionMatrixInverse = glm::inverse(projection);
    // glm::mat4 viewMatrixInverse = glm::inverse(view);
    // glm::mat4 viewProjectionMatrixInverse = viewMatrixInverse * projectionMatrixInverse;
    // frustum.set(viewProjectionMatrixInverse);

    // AABB aabbT = _pMesh->meshData.aabb.transformation(model);
    // aabbT.render();
    render3D.begin(nullptr);
    _pMesh->pRenderStat->submit(&render3D); // render3D.submit(_pMesh->pRenderStat);
    render3D.end();

    render3D.flush();
}

void VisitorRender::visit(NodeLight* _pLight) { _pLight->data.setUniform(pShader); }

void VisitorRender::visit(NodeParticleEmitter* _pParticleEmitter) {

    if (particleOn == true) {

        model = _pParticleEmitter->getTransform()->getModelMatrix(pTransform->getPosition());

        if (pShader == nullptr)
            return;

        // Get the variables from the shader to which data will be passed
        pShader->setUniform("projection", projection);
        pShader->setUniform("view", view);
        // shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr(
        // glm::inverseTranspose(glm::mat3(_view))));

        pShader->setUniform("model", model);

        // We will need the camera's position in order to sort the particles
        // w.r.t the camera's distance.
        // There should be a getCameraPosition() function in common/controls.cpp,
        // but this works too.
        glm::vec3 CameraPosition(glm::inverse(view)[3]);

        // Vertex shader
        pShader->setUniform("CameraRight_worldspace", glm::vec3(view[0][0], view[1][0], view[2][0]));
        pShader->setUniform("CameraUp_worldspace", glm::vec3(view[0][1], view[1][1], view[2][1]));

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
        pShader->setUniform("lightSpaceMatrix", pShadowMapVisitor->getLightSpaceMatrix());
    }

    NodeCamera* pCam = (NodeCamera*)_pGroup->findChild(Chimera::Kind::CAMERA, 0, false);
    if (pCam != nullptr) {
        ICamera* cam = pCam->getCamera();
        cam->processInput(0.01);

        pShader->setUniform("viewPos", cam->getPosition());
        if (pVideo->getTotEyes() == 1) {
            glViewport(0, 0, pVideo->getWidth(), pVideo->getHeight());
            // frustum.set(cam->recalcMatrix(pVideo->getRatio()));
            cam->recalcMatrix(pVideo->getRatio());
            view = cam->getViewMatrix();
            projection = cam->getProjectionMatrix();

        } else {
            // TODO: melhorar para dentro da propria camera !!
            Eye* pEye = ((CanvasHmd*)pVideo)->getEye(eye);
            glViewport(0, 0, pEye->fbTexGeo.w, pEye->fbTexGeo.h);
            cam->recalcMatrix(1.0f); // FIXME: 1 porque o radio nao e do video mas do eye!!!!
            if (eye == 0) {          // left
                view = cam->getViewMatrix();
            } else {
                // ViewPoint nova = ViewPoint(*vp);
                glm::vec3 novaPosition = cam->getPosition();
                glm::vec3 novaFront = cam->getFront();
                glm::vec3 left_p = novaFront - novaPosition;
                glm::vec3 cross1 = glm::cross(cam->getUp(), left_p);
                glm::vec3 norm1 = glm::normalize(cross1);
                glm::vec3 final_norm1 = norm1 * 5.0f;

                novaPosition = cam->getPosition() - final_norm1;
                novaFront = cam->getFront() - final_norm1;
                view = glm::lookAt(novaPosition, novaFront, cam->getUp());
            }

            projection = cam->getProjectionMatrix();
        }
    }
}

void VisitorRender::visit(NodeHUD* _pHUD) {

    if (HudOn == true) {
        if (_pHUD->isOn() == true) {
            glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(pVideo->getWidth()), 0.0f, static_cast<GLfloat>(pVideo->getHeight()));
            pShader->setUniform("projection", proj);
            _pHUD->render(pShader);
        }
    }
}
} // namespace Chimera
