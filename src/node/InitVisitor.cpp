#include "chimera/node/InitVisitor.hpp"

#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/SceneMng.hpp"
#include "chimera/node/Solid.hpp"
#include "chimera/node/Transform.hpp"

#include <iostream>

namespace Chimera {

InitVisitor::InitVisitor() {}

InitVisitor::~InitVisitor() {}

void InitVisitor::visit(Camera* _pCamera) { _pCamera->init(); }

void InitVisitor::visit(Mesh* _pMesh) { _pMesh->init(); }

void InitVisitor::visit(Light* _pLight) { _pLight->init(); }

void InitVisitor::visit(ParticleEmitter* _pParticleEmitter) { _pParticleEmitter->init(); }

void InitVisitor::visit(SceneMng* _pSceneMng) { _pSceneMng->init(); }

void InitVisitor::visit(Group* _pGroup) {
    _pGroup->init(); // seleciona o shader
}

void InitVisitor::visit(Transform* _pTransform) {
    //_pTransform->init();
}

void InitVisitor::visit(Solid* _pSolid) { _pSolid->init(); }

void InitVisitor::visit(HUD* _pHUD) { _pHUD->init(); }

void InitVisitor::visit(ShadowMap* _pShadowMap) { _pShadowMap->init(); }

} // namespace Chimera
