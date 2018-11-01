#ifndef __CHIMERA_LOADER_LIBRARY_GEOMETRYS_H
#define __CHIMERA_LOADER_LIBRARY_GEOMETRYS_H

#include "Library.h"
//#include "Material.h"

namespace ChimeraLoaders {

class LibraryGeometrys: public Library {

public:
    LibraryGeometrys(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryGeometrys();
    //Chimera::Material *target();
private:

};
}

#endif