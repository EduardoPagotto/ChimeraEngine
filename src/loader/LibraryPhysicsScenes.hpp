#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/node/PhysicsControl.hpp"

namespace ChimeraLoaders {

class LibraryPhysicsScenes : public Library {
  public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string& _url,
                         Chimera::PhysicsControl* _pPhysicsControl);
    virtual ~LibraryPhysicsScenes();
    void target();

  private:
    void loadPhysicControlCollada(tinyxml2::XMLElement* _nNode);

    ListNodes* pListNodes;
    Chimera::PhysicsControl* pPhysicsControl;
};
} // namespace ChimeraLoaders

#endif