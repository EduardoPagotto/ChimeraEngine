#ifndef __CHIMERA_LOADER_SCENE_H
#define __CHIMERA_LOADER_SCENE_H

#include "Library.h"
#include "Group.h"
#include "PhysicsControl.h"
#include "TextureManager.h"

namespace ChimeraLoaders {

class Scene: public Library {

public:
    Scene(const std::string &_file);
    virtual ~Scene();
    Chimera::Node *target();
private:
    Chimera::PhysicsControl *pPhysicsControl;
    Chimera::Group *pGroup;
    Chimera::TextureManager *pTexManager;
};
}

#endif