#include "chimera/node/RenderVisitor.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/NodeParse.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/ShadowMapVisitor.hpp"
#include "chimera/node/Solid.hpp"
#include "chimera/node/Transform.hpp"

#include "chimera/OpenGLDefs.hpp"

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

    shadowMap = nullptr;
    pShader = nullptr;
}

RenderVisitor::~RenderVisitor() {}

void RenderVisitor::visit(Camera* _pCamera) {}

void RenderVisitor::visit(Mesh* _pMesh) {

    if (pShader == nullptr)
        return;

    int shadows = 1;
    pShader->setGlUniform1i("shadows", shadows);
    // glUniform1i(glGetUniformLocation(shader.Program, "shadows"), shadows);

    // Get the variables from the shader to which data will be passed
    pShader->setGlUniformMatrix4fv("projection", 1, false, glm::value_ptr(projection));
    pShader->setGlUniformMatrix4fv("view", 1, false, glm::value_ptr(view));
    pShader->setGlUniformMatrix4fv("model", 1, false, glm::value_ptr(model));
    // shader->setGlUniformMatrix3fv("noMat", 1, false, glm::value_ptr(
    // glm::inverseTranspose(glm::mat3(_view))));

    _pMesh->getMaterial()->apply(pShader);

    if (shadowMap != nullptr)
        shadowMap->applyShadow("shadowMap", pShader);

    _pMesh->render(pShader);
}

void RenderVisitor::visit(Light* _pLight) {}

void RenderVisitor::visit(ParticleEmitter* _pParticleEmitter) {

    if (particleOn == true) {

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

void RenderVisitor::visit(SceneMng* _pSceneMng) {
    //_pSceneRoot->apply();
    // TODO: necessario o SceneMng
}

void RenderVisitor::visit(Group* _pGroup) {

    pShader = _pGroup->getShader();
    if (pShader == nullptr)
        return;

    // Renderiza o ShadowMap que e filho dr group pelo ShadowMapVisitor
    // retornando uma textura dentro do shadowMap
    ShadowMapVisitor* sVisit = (ShadowMapVisitor*)_pGroup->getNodeVisitor();
    if (sVisit != nullptr) {

        // TODO: colocar a carga na inicializacao??
        shadowMap = (ShadowMap*)_pGroup->findChild(Chimera::EntityKind::SHADOWMAP, 0, false);

        // TODO: passar parametros de outra forma para generalizar aqui
        sVisit->shadowMap = shadowMap;
        sVisit->pCoord = pCoord;

        shadowMap->initSceneShadow();
        NodeParse::tree(_pGroup, sVisit);
        shadowMap->endSceneShadow();
    } else {
        shadowMap = nullptr;
    }

    pShader->link();

    if (shadowMap != nullptr) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pShader->setGlUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, glm::value_ptr(shadowMap->lightSpaceMatrix));
    }

    Camera* pCam = (Camera*)_pGroup->findChild(Chimera::EntityKind::CAMERA, 0, false);
    if (pCam != nullptr) {
        ViewPoint* vp = pCam->getViewPoint();
        pShader->setGlUniform3fv("viewPos", 1, glm::value_ptr(vp->position));

        pVideo->calcPerspectiveProjectionView(eye, vp, view, projection);
        // projection = pVideo->getPerspectiveProjectionMatrix(vp, eye);
        // View Matrix
        // view = glm::lookAt(vp->position, vp->front, vp->up);
    }

    Light* pLight = (Light*)_pGroup->findChild(Chimera::EntityKind::LIGHT, 0, false);
    if (pLight != nullptr)
        pLight->apply(pShader);
}

void RenderVisitor::visit(Chimera::Transform* _pTransform) { model = _pTransform->getModelMatrix(pCoord); }

void RenderVisitor::visit(Solid* _pSolid) { model = _pSolid->getModelMatrix(pCoord); }

void RenderVisitor::visit(HUD* _pHUD) {

    if (HudOn == true) {
        if (_pHUD->isOn() == true) {

            pShader->setGlUniformMatrix4fv("projection", 1, false,
                                           glm::value_ptr(pVideo->getOrthoProjectionMatrix(eye)));
            _pHUD->render(pShader);
        }
    }
}

void RenderVisitor::visit(ShadowMap* _pShadowMap) {}

} // namespace Chimera
