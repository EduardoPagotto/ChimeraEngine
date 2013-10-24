#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>

#include <log4cxx/logger.h>

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

#include "Physics.h"

#include "Node.h"
#include "Draw.h"
#include "Texture.h"
#include "Effect.h"
#include "Physics.h"


namespace Chimera {

	class Physics;
	
class Object : public Node {
public: 
    
    friend class Loader;
    friend class Physics;

    Object();
    Object( const Object& _object );
    
    virtual ~Object();
    virtual void update ( DataMsg *_dataMsg );

    void init();
    void render();
    
    void applyTorc(const btVector3 &_vet);
    void applyForce(const btVector3 &_vet);
    
	btVector3& getPosition();
	
	bool get_check_collision();
	void on_start_collision(Object *_pObj);
	void on_collision(Object *_pObj);
	void on_end_collision(Object *_pObj);
	
private:
    Physics *pPhysic;
    Draw *pDraw;
    Texture *pTexture;
    Effect *pEffect;
    
	bool collide;
	
    btTransform transform;
    //btVector3 position;
	
	log4cxx::LoggerPtr logger;
};

}

#endif