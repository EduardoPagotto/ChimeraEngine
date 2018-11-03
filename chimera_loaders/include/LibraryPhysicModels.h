#ifndef __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS_H
#define __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS_H

#include "Library.h"
//#include "Solid.h"
#include "Group.h"

namespace ChimeraLoaders {

class LibraryPhysicModels: public Library {

public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryPhysicModels();
    Chimera::Group *target();
private:
    void loadColladaShape ( tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Chimera::Solid *_pPhysic);
    //std::tuple<Chimera::Color, Chimera::LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
}

#endif