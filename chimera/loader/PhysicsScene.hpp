#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/loader/Library.hpp"

namespace Chimera {

class PhysicsScene : public Library {
  public:
    PhysicsScene(const std::string& _file, Registry* reg) : Library(nullptr, "!" + _file), reg(reg) {}
    virtual ~PhysicsScene() {}
    void target();

  private:
    Registry* reg;
};
} // namespace Chimera