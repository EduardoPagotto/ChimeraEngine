#ifndef __CHIMERA_LOADER_LIBRARY_IMAGES_H
#define __CHIMERA_LOADER_LIBRARY_IMAGES_H

#include "Library.h"

namespace ChimeraLoaders {

class LibraryImages: public Library {

public:
    LibraryImages(tinyxml2::XMLElement* _root, const std::string &_url);
    virtual ~LibraryImages();
    std::tuple<std::string, std::string> target();
};
}

#endif