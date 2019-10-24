#ifndef __CHIMERA_CONSTRAINT__HPP
#define __CHIMERA_CONSTRAINT__HPP

#include <LinearMath/btTransform.h>

#include "Solid.hpp"
#include "chimera/node/Entity.hpp"

namespace Chimera {

class Constraint : public Entity {
  public:
    Constraint(std::string _name);
    virtual ~Constraint();

  private:
    Solid* pPhysicsA;
    Solid* pPhysicsB;

    btTransform transformA;
    btTransform transformB;
};
} // namespace Chimera
#endif
