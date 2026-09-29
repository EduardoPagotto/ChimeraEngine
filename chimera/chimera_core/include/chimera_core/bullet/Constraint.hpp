#pragma once
#include "Solid.hpp"
#include <LinearMath/btTransform.h>

namespace ce {

    class Constraint {
      public:
        Constraint() = default;
        virtual ~Constraint() = default;

      private:
        Solid* pPhysicsA = nullptr;
        Solid* pPhysicsB = nullptr;
        btTransform transformA;
        btTransform transformB;
    };
} // namespace ce
