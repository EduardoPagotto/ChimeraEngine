#ifndef __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES__HPP
#define __CHIMERA_LOADER_LIBRARY_PHYSICS_SCENES__HPP

#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/physic/PhysicsControl.hpp"

namespace Chimera {

class LibraryPhysicsScenes : public Library {
  public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string& _url, PhysicsControl* _pPhysicsControl);
    virtual ~LibraryPhysicsScenes();
    void target();

  private:
    void loadPhysicControlCollada(tinyxml2::XMLElement* _nNode);

    ListNodes* pListNodes;
    PhysicsControl* pPhysicsControl;
};
} // namespace Chimera

#endif