#ifndef __CHIMERA_LOADER_LIBRARY_IMAGES__HPP
#define __CHIMERA_LOADER_LIBRARY_IMAGES__HPP

#include "chimera/loader/Library.hpp"

namespace ChimeraLoaders {

class LibraryImages : public Library {

  public:
    LibraryImages(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryImages();
    std::tuple<std::string, std::string> target();
};
} // namespace ChimeraLoaders
#endif
