#include "InitVisitor.h"

#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "ParticleEmitter.h"
#include "SceneRoot.h"
#include "Group.h"
#include "Transform.h"
#include "Solid.h"
#include "HUD.h"

#include <iostream>

namespace Chimera {

InitVisitor::InitVisitor() {
    log = spdlog::stdout_color_st("InitVisitor");
    log->info("Iniciado");
	shader =  Singleton<Shader>::getRefSingleton();
}

InitVisitor::~InitVisitor() {
	Singleton<Shader>::releaseRefSingleton();
}

void InitVisitor::visit ( Camera* _pCamera ) {
    log->debug("Visit Camera: {0}", _pCamera->getName());
    _pCamera->init();
}

void InitVisitor::visit ( Mesh* _pMesh ) {
    log->debug("Visit Mesh: {0}", _pMesh->getName());
    _pMesh->init();
}

void InitVisitor::visit ( Light* _pLight ) {
    log->debug("Visit Light: {0}", _pLight->getName());
    _pLight->init();
}

void InitVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {
    log->debug("Visit ParticleEmitter: {0}", _pParticleEmitter->getName());
    _pParticleEmitter->init();
}

void InitVisitor::visit ( SceneRoot* _pSceneRoot ) {
    log->debug("Visit SceneRoot: {0}", _pSceneRoot->getName());
    _pSceneRoot->init();
}

void InitVisitor::visit ( Group* _pGroup ) {
    log->debug("Visit Group: {0}", _pGroup->getName());
	_pGroup->init(); //seleciona o shader
}

void InitVisitor::visit ( Transform* _pTransform ) {
    log->debug("Visit Transform: {0}", _pTransform->getName());
    //_pTransform->init();
}

void InitVisitor::visit ( Solid* _pSolid ) {
    log->debug("Visit Solid: {0}", _pSolid->getName());
    _pSolid->init();
}

void InitVisitor::visit ( HUD* _pHUD ) {
    log->debug("Visit HUD: {0}", _pHUD->getName());
    _pHUD->init();
}
}
