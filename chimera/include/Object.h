#ifndef OBJECT_H
#define OBJECT_H

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "Draw.h"
#include "Physics.h"
#include "Material.h"

namespace Chimera {
    
class Object : public Node
{
public:

    friend class Physics;

    Object (Node* _parent, std::string _name );
    Object ( const Object& _object );

    virtual ~Object();
    virtual void update ( DataMsg *_dataMsg );

    virtual void init();

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
    Physics *pPhysic;

private:
    void execute ( bool _texture, Object *pObj );

    btTransform transform;
};
}

#endif
