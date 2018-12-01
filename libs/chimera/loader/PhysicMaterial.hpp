#ifndef _PHYSIC_MATERIAL__HPP
#define _PHYSIC_MATERIAL__HPP

#include "chimera/node/Entity.hpp"
#include <LinearMath/btScalar.h>

namespace Chimera {

class PhysicMaterial : Entity {
  public:
    PhysicMaterial(std::string _name) : Entity(EntityKind::SOLID_MATERIAL, _name) {
        frictionDynamic = 10.0;
        frictionStatic = 0.0;
        restitution = 0.0;
    }

    PhysicMaterial(const PhysicMaterial& _physicMaterial) : Entity(_physicMaterial) {
        frictionDynamic = _physicMaterial.frictionDynamic;
        frictionStatic = _physicMaterial.frictionStatic;
        restitution = _physicMaterial.restitution;
    }

    virtual ~PhysicMaterial() {}

    inline void setFrictionDynamic(const float& _friction) { frictionDynamic = _friction; }

    inline void setFrictionStatic(const float& _friction) { frictionStatic = _friction; }

    inline void setRestitution(const float& _restitution) { restitution = _restitution; }

    inline btScalar getFrictionDynamic() const { return frictionDynamic; }

    inline btScalar getFrictionStatic() const { return frictionStatic; }

    inline btScalar getRestitution() const { return restitution; }

  private:
    btScalar frictionDynamic;
    btScalar frictionStatic;
    btScalar restitution;
};
} // namespace Chimera
#endif
