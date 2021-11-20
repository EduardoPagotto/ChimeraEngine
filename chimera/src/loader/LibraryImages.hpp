#pragma once
#include "chimera/loader/Library.hpp"

namespace Chimera {

class LibraryImages : public Library {

  public:
    LibraryImages(tinyxml2::XMLElement* _root, const std::string& _url) : Library(_root, _url) {}
    virtual ~LibraryImages() {}
    std::tuple<std::string, std::string> target();
};
} // namespace Chimera
