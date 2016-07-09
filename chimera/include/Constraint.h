#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <LinearMath/btTransform.h>

#include "Entity.h"
#include "Physics.h"

namespace Chimera {
    
class Constraint : public Entity
{
public:

    Constraint ( std::string _name );
    virtual ~Constraint();
private:
	//FIXME Tornar privado novamente

	Physics *pPhysicsA;
	Physics *pPhysicsB;

	btTransform transformA;
	btTransform transformB;

};
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
