#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES_H
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES_H

#include "Library.h"
#include "Group.h"

#include "PhysicsControl.h"
namespace ChimeraLoaders {

class LibraryPhysicsScenes: public Library {

public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string &_url, Chimera::Group *_pScene);
    virtual ~LibraryPhysicsScenes();
    Chimera::PhysicsControl *target();
private:
    void loadPhysicControlCollada(tinyxml2::XMLElement* _nNode, Chimera::PhysicsControl *_pPhysicsControl);
    Chimera::Group *pScene;
};
}

#endif