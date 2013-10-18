#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include "Draw.h"

#include "Physics.h"


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
	//Draw *pDraw;
    btTransform transform;
    btVector3 position;
};

}

#endif