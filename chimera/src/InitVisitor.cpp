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
	shader =  Singleton<Shader>::getRefSingleton();
}

InitVisitor::~InitVisitor() {
	Singleton<Shader>::releaseRefSingleton();
}

void InitVisitor::visit ( Camera* _pCamera ) {

     std::cout << "Visit Camera:" << _pCamera->getName() << "\n";
    _pCamera->init();
}

void InitVisitor::visit ( Mesh* _pMesh ) {

    std::cout << "Visit Mesh:" << _pMesh->getName() << "\n";
    _pMesh->init();

}

void InitVisitor::visit ( Light* _pLight ) {

    std::cout << "Visit Light:" << _pLight->getName() << "\n";
    _pLight->init();
}

void InitVisitor::visit ( ParticleEmitter* _pParticleEmitter ) {

    std::cout << "Visit ParticleEmitter:" << _pParticleEmitter->getName() << "\n";
    _pParticleEmitter->init();

}

void InitVisitor::visit ( SceneRoot* _pSceneRoot ) {

    std::cout << "Visit SceneRoot:" << _pSceneRoot->getName() << "\n";
    _pSceneRoot->init();

}

void InitVisitor::visit ( Group* _pGroup ) {

    std::cout << "Visit Group:" << _pGroup->getName() << "\n";
	_pGroup->init(); //seleciona o shader

}

void InitVisitor::visit ( Transform* _pTransform ) {

     std::cout << "Visit Transform:" << _pTransform->getName() << "\n";
    //_pTransform->init();

}

void InitVisitor::visit ( Solid* _pSolid ) {

     std::cout << "Visit Solid:" << _pSolid->getName() << "\n";
    _pSolid->init();

}

void InitVisitor::visit ( HUD* _pHUD ) {

    std::cout << "Visit HUD:" << _pHUD->getName() << "\n";
    _pHUD->init();

}

}
