#ifndef __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS_H
#define __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS_H

#include "Library.h"
#include "ListNodes.h"
#include "Solid.h"

namespace ChimeraLoaders {

class LibraryPhysicModels: public Library {

public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string &_url, Chimera::PhysicsControl *_pWorld);
    virtual ~LibraryPhysicModels();
    void target();
private:
    void loadColladaShape ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Chimera::Solid *_pPhysic);
    ListNodes *pListNodes;
    Chimera::PhysicsControl *pWorld;
};
}

#endif