#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include "Node.h"

//#include "ChimeraMesh.h"
#include "Physics.h"
#include "Shapes.h"

namespace Chimera {

class Object : public Node {
public:   
	//friend class Loader;

    Object();
    Object( const Object& object );
    
	virtual ~Object(void);
    virtual void update ( DataMsg *dataMsg );

	void setDraw(Draw *_pDraw) {
		m_pDraw = _pDraw;
	}

	void setPhysic(Physics *_pPhysic) {
		m_pPhysic = _pPhysic;
	}

	void applyTorc(const btVector3 &_vet);

	void applyForce(const btVector3 &_vet);

	
private:
	Physics *m_pPhysic;

	Draw *m_pDraw;
};

}

#endif