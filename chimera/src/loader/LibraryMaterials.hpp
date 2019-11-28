#ifndef __CHIMERA_LOADER_LIBRARY_MATERIALS__HPP
#define __CHIMERA_LOADER_LIBRARY_MATERIALS__HPP

#include "chimera/core/Material.hpp"
#include "chimera/loader/Library.hpp"

namespace ChimeraLoaders {

class LibraryMaterials : public Library {

  public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryMaterials();
    Chimera::Material* target();

  private:
};
} // namespace ChimeraLoaders

#endif