#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <LinearMath/btTransform.h>

#include "Node.h"
#include "Physics.h"

namespace Chimera {
namespace Graph {
    
class Constraint : public Node
{
public:

    friend class Loader;

    Constraint ( std::string _name );
    Constraint ( const Constraint& _constraint );
    virtual ~Constraint();

    virtual void clone ( Node **ppNode );

    virtual void update ( DataMsg *dataMsg );

    //FIXME Tornar privado novamente
    
    Physics *pPhysicsA;
    Physics *pPhysicsB;
    
    btTransform transformA;
    btTransform transformB;
private:

};
}
}
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
