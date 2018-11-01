#ifndef __CHIMERA_LOADER_LIBRARY_MATERIALS_H
#define __CHIMERA_LOADER_LIBRARY_MATERIALS_H

#include "Library.h"
//#include "Light.h"

namespace ChimeraLoaders {

class LibraryMaterials: public Library {

public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryMaterials();
    //Chimera::Light *target(glm::mat4 l_pTransform);
private:
    //std::tuple<Chimera::Color, Chimera::LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
}

#endif