#ifndef __CHIMERA_LOADER_LIBRARY_MATERIALS__HPP
#define __CHIMERA_LOADER_LIBRARY_MATERIALS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/render/MatData.hpp"

namespace ChimeraLoaders {

class LibraryMaterials : public Library {

  public:
    LibraryMaterials(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryMaterials();
    Chimera::MatData* target();

  private:
};
} // namespace ChimeraLoaders

#endif