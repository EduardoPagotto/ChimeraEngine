#include "chimera/node/InitVisitor.hpp"
#include "chimera/node/Camera.hpp"
#include "chimera/node/Group.hpp"
#include "chimera/node/HUD.hpp"
#include "chimera/node/Light.hpp"
#include "chimera/node/Mesh.hpp"
#include "chimera/node/ParticleEmitter.hpp"
#include "chimera/node/ShadowMap.hpp"
#include <iostream>

namespace Chimera {
InitVisitor::InitVisitor() {}
InitVisitor::~InitVisitor() {}
void InitVisitor::init() {}
void InitVisitor::visit(Camera* _pCamera) { _pCamera->init(); }
void InitVisitor::visit(Mesh* _pMesh) { _pMesh->init(); }
void InitVisitor::visit(Light* _pLight) { _pLight->init(); }
void InitVisitor::visit(ParticleEmitter* _pParticleEmitter) { _pParticleEmitter->init(); }
void InitVisitor::visit(Group* _pGroup) { _pGroup->init(); }
void InitVisitor::visit(HUD* _pHUD) { _pHUD->init(); }
} // namespace Chimera
