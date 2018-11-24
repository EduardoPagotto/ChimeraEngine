#ifndef __CHIMERA_LOADER_PHYSICS_SCENE__HPP
#define __CHIMERA_LOADER_PHYSICS_SCENE__HPP

#include "Library.h"
#include "PhysicsControl.h"

namespace ChimeraLoaders {

class PhysicsScene : public Library {
  public:
    PhysicsScene(const std::string& _file, Chimera::PhysicsControl* _pPhysicsControl);
    virtual ~PhysicsScene();
    void target();

  private:
    Chimera::PhysicsControl* pPhysicsControl;
};
} // namespace ChimeraLoaders

#endif