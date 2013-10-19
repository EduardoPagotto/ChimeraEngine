#include "PhysicWorld.h"

namespace Chimera {

PhysicWorld::PhysicWorld() {
    
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher ( m_pCollisionConfiguration );
    m_pBroadphase = new btDbvtBroadphase();
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pDynamicsWorld = new btDiscreteDynamicsWorld ( m_pDispatcher,m_pBroadphase,m_pSolver,m_pCollisionConfiguration );

    m_lastExec = SDL_GetTicks();
}

PhysicWorld::~PhysicWorld() {
    removeAllObjs();
    clearAllShapes();

    delete m_pDynamicsWorld;
    delete m_pSolver;
    delete m_pCollisionConfiguration;
    delete m_pDispatcher;
    delete m_pBroadphase;
}

void PhysicWorld::stepSim ( void ) {
    static bool s_primeiro = true;

    m_timeElapsed = ( btScalar ) ( 0.001 * ( SDL_GetTicks() - m_lastExec ) );

    if ( s_primeiro == false ) {
        if ( ( m_timeElapsed > 0 ) && ( m_timeElapsed < 10 ) ) {
            m_pDynamicsWorld->stepSimulation ( m_timeElapsed,10 );
            m_pDynamicsWorld->debugDrawWorld();
        }
    } else {
        m_pDynamicsWorld->stepSimulation ( 0.016667f,100 );
        s_primeiro = false;
    }

    m_lastExec = SDL_GetTicks();
}

void PhysicWorld::removeAllObjs() {
    //remove the rigidbodies from the dynamics world and delete them
    for ( int i = m_pDynamicsWorld->getNumCollisionObjects()-1; i >= 0 ; i-- ) {
        
        btCollisionObject* pObj = m_pDynamicsWorld->getCollisionObjectArray() [i];
        btRigidBody* pBody = btRigidBody::upcast ( pObj );
        
        if ( pBody && pBody->getMotionState() )
            delete pBody->getMotionState();

        m_pDynamicsWorld->removeCollisionObject ( pObj );
        delete pObj;
        
    }
}

void PhysicWorld::clearAllShapes() {
    //delete collision shapes
// 	for (int j=0; j < m_collisionShapes.size(); j++)
// 	{
// 		btCollisionShape* pShape = m_collisionShapes[j];
// 		m_collisionShapes[j] = 0;
// 		delete pShape;
// 	}
}

}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
