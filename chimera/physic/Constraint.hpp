#pragma once
#include "Solid.hpp"
#include <LinearMath/btTransform.h>

namespace Chimera {

class Constraint {
  public:
    Constraint();
    virtual ~Constraint();

  private:
    Solid* pPhysicsA;
    Solid* pPhysicsB;

    btTransform transformA;
    btTransform transformB;
};
} // namespace Chimera
