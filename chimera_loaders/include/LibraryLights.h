#ifndef __CHIMERA_LOADER_LIBRARY_LIGHTS_H
#define __CHIMERA_LOADER_LIBRARY_LIGHTS_H

#include "Library.h"
#include "Light.h"

namespace ChimeraLoaders {

class LibraryLights: public Library {

public:
    LibraryLights(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryLights();
    Chimera::Light *target();
private:
    Chimera::Color getColor( tinyxml2::XMLElement* l_nColorVal);
    std::tuple<Chimera::Color, Chimera::LightType> loadDiffuseLightColor(tinyxml2::XMLElement* _nNode);
};
}

#endif