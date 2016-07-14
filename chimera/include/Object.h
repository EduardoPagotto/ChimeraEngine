#ifndef OBJECT_H
#define OBJECT_H

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "Draw.h"
#include "Physics.h"
#include "Material.h"
#include "Texture.h"

#include "State.h"

namespace Chimera {
    
class Object : public Node
{
public:

    friend class Physics;

    Object (Node* _parent, std::string _name );
    

    virtual ~Object();
    virtual void update ( DataMsg *_dataMsg );

    virtual void init();

	void accept(class NodeVisitor* v);

    void applyTorc ( const btVector3 &_vet );
    void applyForce ( const btVector3 &_vet );

    void setPositionRotation ( const btVector3 &_posicao, const btVector3 &_rotation );

    btVector3& getPosition();

    void setTransform ( const btTransform &_trans ) {
        transform = _trans;
    }
    
	State *getState() {
		return pState;
	}

	//FIXME colocar como privada
	Draw *pDraw;

	Material *pMaterial;
    Texture *pTexture;
    
    Physics *pPhysic;

	State *pState;

private:
    void execute ( bool _texture, Object *pObj );

    btTransform transform;
};
}

#endif
