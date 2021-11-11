#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/physic/PhysicsControl.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class PhysicsScene : public Library {
  public:
    PhysicsScene(const std::string& _file, PhysicsControl* _pPhysicsControl, Scene* scene);
    virtual ~PhysicsScene();
    void target();

  private:
    PhysicsControl* pPhysicsControl;
    Scene* scene;
};
} // namespace Chimera