#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_MATERIALS__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_MATERIALS__HPP

#include "PhysicMaterial.hpp"
#include "chimera/loader/Library.hpp"

namespace Chimera {

class LibraryPhysicsMaterials : public Library {

  public:
    LibraryPhysicsMaterials(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryPhysicsMaterials();
    PhysicMaterial* target();
};
} // namespace Chimera

#endif