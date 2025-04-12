#pragma once
#include "Solid.hpp"
#include <LinearMath/btTransform.h>

namespace ce {

class Constraint {
  private:
    Solid* pPhysicsA = nullptr;
    Solid* pPhysicsB = nullptr;
    btTransform transformA;
    btTransform transformB;

  public:
    Constraint() = default;
    virtual ~Constraint() = default;
};
} // namespace ce
