#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>			// Header File For The OpenGL32 Library

//#include "Physics.h"
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "Node.h"
#include "Draw.h"
#include "Texture.h"
#include "Effect.h"

namespace Chimera {

class Object : public Node {
public: 
    
	friend class Loader;

    Object();
    Object( const Object& _object );
    
	virtual ~Object();
    virtual void update ( DataMsg *_dataMsg );

// 	void setDraw(Draw *_pDraw) {
// 		pDraw = _pDraw;
// 	}
// 
// 	void setPhysic(Physics *_pPhysic) {
// 		pPhysic = _pPhysic;
// 	}

// 	void applyTorc(const btVector3 &_vet);
// 
// 	void applyForce(const btVector3 &_vet);

	
private:
	//Physics *pPhysic;
	Draw *pDraw;
    Texture *pTexture;
    Effect *pEffect;
    
    btTransform transform;
    btVector3 position;
};

}

#endif