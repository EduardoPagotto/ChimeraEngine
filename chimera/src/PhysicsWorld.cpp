#include "PhysicWorld.h"

namespace Chimera {

PhysicWorld::PhysicWorld() {
    
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher ( m_pCollisionConfiguration );
    
    m_pBroadphase = new btDbvtBroadphase();
    m_pSolver = new btSequentialImpulseConstraintSolver;
    m_pDynamicsWorld = new btDiscreteDynamicsWorld ( m_pDispatcher,m_pBroadphase,m_pSolver,m_pCollisionConfiguration );
    
    m_pDynamicsWorld->setInternalTickCallback( PhysicWorld::doTickCallBack, static_cast<void*>(this), false); //true para forca aplicada apenas dentro do callback
        
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
    
    if (s_primeiro == false) {
        
        //FIXME Descobri porque o stepSim nao esta sincrnizando com os quadros
        countPeriod();
        //m_pDynamicsWorld->stepSimulation ( countPeriod() );
        m_pDynamicsWorld->stepSimulation ( 0.016667f );
        
    } else {
        m_pDynamicsWorld->stepSimulation ( 0.016667f );
        s_primeiro = false;
    }
    

    
}

void PhysicWorld::doTickCallBack(btDynamicsWorld *world, btScalar timeStep) {
    
    PhysicWorld *w = static_cast<PhysicWorld*>(world->getWorldUserInfo());
    w->processTickCallBack(timeStep);
    
}

void PhysicWorld::processTickCallBack(btScalar timeStep) {
    
//     btCollisionObjectArray objects = m_pDynamicsWorld->getCollisionObjectArray();
//     m_pDynamicsWorld->clearForces();
//     for (int i = 0; i < objects.size(); i++) {
//         
//         btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
//         if (!rigidBody) {
//             continue;
//         }
//         
//         rigidBody->applyGravity();
//         //rigidBody->applyForce(btVector3(-10., 0., 0.), btVector3(0., 0., 0.));
//     }
//     return;
 
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
