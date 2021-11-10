#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/physic/PhysicsControl.hpp"

namespace Chimera {

class PhysicsScene : public Library {
  public:
    PhysicsScene(const std::string& _file, PhysicsControl* _pPhysicsControl);
    virtual ~PhysicsScene();
    void target();

  private:
    PhysicsControl* pPhysicsControl;
};
} // namespace Chimera