#pragma once

#include "pugixml.hpp"
#include <string>

namespace Chimera {

class LoadColladaURL {
  public:
    LoadColladaURL(const std::string& url);
    ~LoadColladaURL();

  private:
    pugi::xml_document doc;
    pugi::xml_node root;
};
} // namespace Chimera