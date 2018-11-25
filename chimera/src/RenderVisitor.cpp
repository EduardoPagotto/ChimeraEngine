#include "RenderVisitor.hpp"

#include "Camera.hpp"
#include "Group.hpp"
#include "HUD.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "ParticleEmitter.hpp"
#include "Solid.hpp"
#include "Transform.hpp"

#include "OpenGLDefs.hpp"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera {

RenderVisitor::RenderVisitor() {

    pCoord = nullptr;
    pVideo = nullptr;
    HudOn = true;
    particleOn = true;
    eye = 0;

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);

    shadowOn = nullptr;
    pShader = nullptr;
}

RenderVisitor::~RenderVisitor() {}

void RenderVisitor::visit(Camera* _pCamera) {}

void RenderVisitor::visit(Mesh* _pMesh) {

    if (pShader == nullptr)
        return;

    int shadows = 1;
    pShader->setGlUniform1i("shadows",
                            shadows); // glUniform1i(glGetUniformLocation(shader.Program,
                                      // "shadows"), shadows);

    // Get the variables from the shader to which data will be passed
    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
    // shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr(
    // glm::inverseTranspose(glm::mat3(_view))));

    _pMesh->getMaterial()->apply(pShader);

    if (shadowOn != nullptr) {
        shadowOn->applyShadow();
        pShader->setGlUniform1i("shadowMap", (int)shadowOn->getShadowIndexTextureSeq());
    }

    _pMesh->render(pShader);
}

void RenderVisitor::visit(Light* _pLight) {}

void RenderVisitor::visit(ParticleEmitter* _pParticleEmitter) {

    if (particleOn == true) {

        if (pShader == nullptr)
            return;

        // Get the variables from the shader to which data will be passed
        pShader->setGlUniformMatrix4fv("projection", 1, false,
                                       glm::value_ptr(projection));
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
        pShader->setGlUniform3f("CameraRight_worldspace", view[0][0], view[1][0],
                                view[2][0]);
        pShader->setGlUniform3f("CameraUp_worldspace", view[0][1], view[1][1],
                                view[2][1]);

        _pParticleEmitter->CameraPosition = CameraPosition;
        _pParticleEmitter->render(pShader);
    }
}

void RenderVisitor::visit(SceneMng* _pSceneMng) {
    //_pSceneRoot->apply();
    // TODO: necessario o SceneMng
}

void RenderVisitor::visit(Group* _pGroup) {

    pShader = _pGroup->getShader();
    shadowOn = _pGroup->executeShadoMap(pCoord);

    if (pShader == nullptr)
        return;

    pShader->link();

    if (shadowOn != nullptr) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE,
                                       glm::value_ptr(shadowOn->lightSpaceMatrix));
    }

    Camera* pCam = (Camera*)_pGroup->findChild(Chimera::EntityKind::CAMERA, 0, false);
    if (pCam != nullptr) {
        pShader->setGlUniform3fv("viewPos", 1, glm::value_ptr(pCam->getPosition()));
        projection = pVideo->getPerspectiveProjectionMatrix(
            pCam->getFov(), pCam->getNear(), pCam->getFar(), eye);
        view = pCam->getViewMatrix();
    }

    Light* pLight = (Light*)_pGroup->findChild(Chimera::EntityKind::LIGHT, 0, false);
    if (pLight != nullptr) {

        pShader->setGlUniform3fv("light.position", 1,
                                 glm::value_ptr(pLight->getPosition()));
        pShader->setGlUniform4fv("light.ambient", 1, pLight->getAmbient().ptr());
        pShader->setGlUniform4fv("light.diffuse", 1, pLight->getDiffuse().ptr());
        pShader->setGlUniform4fv("light.specular", 1, pLight->getSpecular().ptr());
    }
}

void RenderVisitor::visit(Chimera::Transform* _pTransform) {

    model = _pTransform->getModelMatrix(pCoord);
}

void RenderVisitor::visit(Solid* _pSolid) { model = _pSolid->getModelMatrix(pCoord); }

void RenderVisitor::visit(HUD* _pHUD) {

    if (HudOn == true) {
        if (_pHUD->isOn() == true) {

            pShader->setGlUniformMatrix4fv(
                "projection", 1, false,
                glm::value_ptr(pVideo->getOrthoProjectionMatrix(eye)));
            _pHUD->render(pShader);
        }
    }
}

} // namespace Chimera
