#ifndef __CHIMERA_PHYSICSWORLD_H
#define __CHIMERA_PHYSICSWORLD_H

#include <map>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <LinearMath/btVector3.h>
#include <SDL2/SDL.h>

#include <Object.h>

namespace Chimera {

	class Object;
	
class PhysicsControl {
public:
	friend class Physics;

	PhysicsControl(void);
	~PhysicsControl(void);

	void clearAllShapes(void);
	void removeAllObjs(void);

	void stepSim(void);

	void checkCollisions();
	
	inline void setGravity(const btVector3 &_vet) {
		discretDynamicsWorld->setGravity(_vet);
	}
	
	inline btScalar countPeriod() {
        period = (btScalar)(clockCounter.getTimeMicroseconds() / 1000000.f);
        clockCounter.reset();
    }
	
	inline btScalar getLastPeriod() {
        return period;
    }
	
private:
    
    static void doTickCallBack(btDynamicsWorld *world, btScalar timeStep);
    void processTickCallBack(btScalar timeStep);
    
	btBroadphaseInterface* broadPhase;

    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;

    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* discretDynamicsWorld;

	/// <summary> evento usando na colisao de corpos se s_dealCollision for false </summary>
	//SDL_Event s_event;
	std::map< btCollisionObject*, std::pair<Object*, Object*> >contactActives;
	
    btClock clockCounter;
    btScalar period;
};

}
#endif