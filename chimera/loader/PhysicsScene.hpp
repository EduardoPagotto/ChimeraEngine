#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class PhysicsScene : public Library {
  public:
    PhysicsScene(const std::string& _file, Scene* scene);
    virtual ~PhysicsScene();
    void target();

  private:
    Scene* scene;
};
} // namespace Chimera