#ifndef __CHIMERA_PHYSICSWORLD_H
#define __CHIMERA_PHYSICSWORLD_H

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btVector3.h> //Bullet
#include <SDL2/SDL.h>

#include "Timer.h"

namespace Chimera {

class PhysicWorld {
public:
	friend class Physics;

	PhysicWorld(void);
	~PhysicWorld(void);

	void clearAllShapes(void);
	void removeAllObjs(void);

	void stepSim(void);

	inline void setGravity(const btVector3 &_vet) {
		m_pDynamicsWorld->setGravity(_vet);
	}
	
	inline btScalar countPeriod() {
        period = (btScalar)(clockCounter.getTimeMicroseconds() / 1000000.f); // m_clock.getTimeMicroseconds();
        clockCounter.reset();
    }
	
	inline btScalar getLastPeriod() {
        return period;
    }
	
private:
    
    static void doTickCallBack(btDynamicsWorld *world, btScalar timeStep);
    void processTickCallBack(btScalar timeStep);
    
	btBroadphaseInterface* m_pBroadphase;
	//btAxisSweep3 *m_pBroadphase;
    btDefaultCollisionConfiguration* m_pCollisionConfiguration;
    btCollisionDispatcher* m_pDispatcher;

    btSequentialImpulseConstraintSolver* m_pSolver;
    btDiscreteDynamicsWorld* m_pDynamicsWorld;

    
    Timer timerSimutation;
	/// <summary> evento usando na colisao de corpos se s_dealCollision for false </summary>
	//SDL_Event s_event;

    btClock clockCounter;
    btScalar period;
};

}
#endif