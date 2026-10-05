#pragma once
#include "Solid.hpp"
#include <LinearMath/btTransform.h>

namespace ce {

    class Constraint {
      public:
        Constraint() = default;
        virtual ~Constraint() = default;

      private:
        Solid* p_physics_a_{nullptr};
        Solid* p_physics_b_{nullptr};
        btTransform transform_a_;
        btTransform transform_b_;
    };
} // namespace ce
