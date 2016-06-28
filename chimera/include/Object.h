#ifndef OBJECT_H
#define OBJECT_H

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "Draw.h"
#include "Physics.h"
#include "Material.h"

namespace Chimera {
namespace Graph {
    
class Object : public Node
{
public:

    friend class Physics;

    Object ( std::string _id, std::string _name );
    Object ( const Object& _object );

    virtual ~Object();
    virtual void update ( DataMsg *_dataMsg );

    virtual void clone ( Node **ppNode );

    void init();

    void applyTorc ( const btVector3 &_vet );
    void applyForce ( const btVector3 &_vet );

    void setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation );

    btVector3& getPosition();

    void setTransform ( const btTransform &_trans ) {
        transform = _trans;
    }
    
	//FIXME colocar como privada
	Draw *pDraw;
	Material *pMaterial;

private:
    void execute ( bool _texture, Object *pObj );

    Physics *pPhysic;


    btTransform transform;
};
}
}

#endif
