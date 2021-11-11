#ifndef _PHYSIC_MATERIAL__HPP
#define _PHYSIC_MATERIAL__HPP

#include <LinearMath/btScalar.h>

namespace Chimera {

struct PhysicMaterial {
    PhysicMaterial() {
        frictionDynamic = 10.0;
        frictionStatic = 0.0;
        restitution = 0.0;
    }

    PhysicMaterial(const PhysicMaterial& _physicMaterial) {
        frictionDynamic = _physicMaterial.frictionDynamic;
        frictionStatic = _physicMaterial.frictionStatic;
        restitution = _physicMaterial.restitution;
    }

    btScalar frictionDynamic;
    btScalar frictionStatic;
    btScalar restitution;
};
} // namespace Chimera
#endif
