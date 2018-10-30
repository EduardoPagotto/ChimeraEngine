#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_MATERIAL_H
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_MATERIAL_H

#include <map>
#include <string.h>

#include <tinyxml2.h>

#include "PhysicMaterial.h"

namespace ChimeraLoaders {

class LibraryPhysicsMaterials {

public:
    LibraryPhysicsMaterials(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryPhysicsMaterials();

    Chimera::PhysicMaterial *target();

private:
    std::string url;
    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* root;
};
}

#endif