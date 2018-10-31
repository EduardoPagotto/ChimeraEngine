#ifndef __CHIMERA_LOADER_LIBRARY_IMAGES_H
#define __CHIMERA_LOADER_LIBRARY_IMAGES_H

#include "Library.h"
#include "TextureManager.h"
//#include "PhysicMaterial.h"

namespace ChimeraLoaders {

class LibraryImages: public Library {

public:
    LibraryImages(tinyxml2::XMLElement* _root, const std::string &_url, Chimera::TextureManager *_texManager);
    virtual ~LibraryImages();

   void target();

private:
    Chimera::TextureManager *pTexManager;

};
}

#endif