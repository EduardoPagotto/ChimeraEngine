#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_MATERIALS__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_MATERIALS__HPP

#include "Library.hpp"
#include "PhysicMaterial.hpp"

namespace ChimeraLoaders {

class LibraryPhysicsMaterials : public Library {

  public:
    LibraryPhysicsMaterials(tinyxml2::XMLElement* _root, const std::string& _url);
    virtual ~LibraryPhysicsMaterials();
    Chimera::PhysicMaterial* target();
};
} // namespace ChimeraLoaders

#endif