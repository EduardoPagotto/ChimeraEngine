#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES_H
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES_H

#include "Library.h"
#include "Light.h"

namespace ChimeraLoaders {

class LibraryPhysicsScenes: public Library {

public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryPhysicsScenes();
    //Chimera::Light *target();
private:
    //std::tuple<Chimera::Color, Chimera::LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
}

#endif