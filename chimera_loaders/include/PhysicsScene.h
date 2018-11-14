#ifndef __CHIMERA_LOADER_PHYSICS_SCENE_H
#define __CHIMERA_LOADER_PHYSICS_SCENE_H

#include "Library.h"
#include "PhysicsControl.h"

namespace ChimeraLoaders {

class PhysicsScene: public Library {
public:
    PhysicsScene(const std::string &_file, Chimera::PhysicsControl *_pPhysicsControl);
    virtual ~PhysicsScene();
    void target();
private:
    Chimera::PhysicsControl *pPhysicsControl;

};
}

#endif