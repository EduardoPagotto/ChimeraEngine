#ifndef __CHIMERA_LOADER_LIBRARY_MATERIALS_H
#define __CHIMERA_LOADER_LIBRARY_MATERIALS_H

#include "Library.h"
#include "Material.h"

namespace ChimeraLoaders {

class LibraryMaterials: public Library {

public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryMaterials();
    Chimera::Material *target();
private:

};
}

#endif