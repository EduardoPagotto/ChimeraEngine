#ifndef __CHIMERA_CONSTRAINT__HPP
#define __CHIMERA_CONSTRAINT__HPP

#include <LinearMath/btTransform.h>

#include "Entity.h"
#include "Solid.h"

namespace Chimera {

class Constraint : public Entity {
  public:
    Constraint(std::string _name);
    virtual ~Constraint();

  private:
    // FIXME Tornar privado novamente

    Solid* pPhysicsA;
    Solid* pPhysicsB;

    btTransform transformA;
    btTransform transformB;
};
} // namespace Chimera
#endif
