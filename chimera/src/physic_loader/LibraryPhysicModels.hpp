#ifndef __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSIC_MODELS__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/physic/Solid.hpp"

namespace Chimera {

class LibraryPhysicModels : public Library {

  public:
    LibraryPhysicModels(tinyxml2::XMLElement* _root, const std::string& _url, Chimera::PhysicsControl* _pWorld);
    virtual ~LibraryPhysicModels();
    void target(std::map<std::string, Chimera::Solid*>& _mapSolids);

  private:
    void loadColladaShape(tinyxml2::XMLElement* _root, tinyxml2::XMLElement* _nShape, Chimera::Solid* _pPhysic);
    ListNodes* pListNodes;
    Chimera::PhysicsControl* pWorld;
};
} // namespace Chimera

#endif