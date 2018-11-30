#include "InitVisitor.hpp"

#include "Camera.hpp"
#include "Group.hpp"
#include "HUD.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "ParticleEmitter.hpp"
#include "SceneMng.hpp"
#include "Solid.hpp"
#include "Transform.hpp"

#include <iostream>

namespace Chimera {

InitVisitor::InitVisitor() {
    log = spdlog::get("chimera");
    log->debug("Constructor InitVisitor");
}

InitVisitor::~InitVisitor() { log->debug("Destructor InitVisitor"); }

void InitVisitor::visit(Camera* _pCamera) {
    log->debug("Visit Camera: {0}", _pCamera->getName());
    _pCamera->init();
}

void InitVisitor::visit(Mesh* _pMesh) {
    log->debug("InitVisitor Mesh: {0}", _pMesh->getName());
    _pMesh->init();
}

void InitVisitor::visit(Light* _pLight) {
    log->debug("InitVisitor Light: {0}", _pLight->getName());
    _pLight->init();
}

void InitVisitor::visit(ParticleEmitter* _pParticleEmitter) {
    log->debug("InitVisitor ParticleEmitter: {0}", _pParticleEmitter->getName());
    _pParticleEmitter->init();
}

void InitVisitor::visit(SceneMng* _pSceneMng) {
    log->debug("InitVisitor ScenMng: {0}", _pSceneMng->getName());
    _pSceneMng->init();
}

void InitVisitor::visit(Group* _pGroup) {
    log->debug("InitVisitor Group: {0}", _pGroup->getName());
    _pGroup->init(); // seleciona o shader
}

void InitVisitor::visit(Transform* _pTransform) {
    log->debug("InitVisitor Transform: {0}", _pTransform->getName());
    //_pTransform->init();
}

void InitVisitor::visit(Solid* _pSolid) {
    log->debug("InitVisitor Solid: {0}", _pSolid->getName());
    _pSolid->init();
}

void InitVisitor::visit(HUD* _pHUD) {
    log->debug("InitVisitor HUD: {0}", _pHUD->getName());
    _pHUD->init();
}

void InitVisitor::visit(ShadowMap* _pShadowMap) {
    log->debug("InitVisitor ShadowMap: {0}", _pShadowMap->getName());
    _pShadowMap->init();
}

} // namespace Chimera
