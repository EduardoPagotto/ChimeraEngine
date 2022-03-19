#pragma once

#include "chimera/core/Registry.hpp"
#include "pugixml.hpp"
#include <string>

namespace Chimera {

struct InstanceCollada {
    pugi::xml_document doc;
    pugi::xml_node root;
    pugi::xml_node node;
};

const pugi::xml_node colladaGetLibrary(const pugi::xml_node& node, const std::string& libraryName, const std::string key);
InstanceCollada* colladaURL(InstanceCollada* handle, const std::string& libraryName, const std::string& url);

void loadImage(InstanceCollada* handle, const std::string& id);

void loadAll(InstanceCollada* handle, Registry* reg);

} // namespace Chimera