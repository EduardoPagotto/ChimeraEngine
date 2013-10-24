#include "PhysicsControl.h"

namespace Chimera {

PhysicsControl::PhysicsControl() {

    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher ( collisionConfig );

    broadPhase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    discretDynamicsWorld = new btDiscreteDynamicsWorld ( dispatcher, broadPhase, solver, collisionConfig );

    //true para forca aplicada apenas dentro docallback
    discretDynamicsWorld->setInternalTickCallback ( PhysicsControl::doTickCallBack, static_cast < void *> ( this ), false );

}

PhysicsControl::~PhysicsControl() {
    removeAllObjs();
    clearAllShapes();

    delete discretDynamicsWorld;
    delete solver;
    delete collisionConfig;
    delete dispatcher;
    delete broadPhase;
}

void PhysicsControl::stepSim ( void ) {

    static bool s_primeiro = true;

    if ( s_primeiro == false ) {

        // FIXME Descobri porque o stepSim nao esta sincrnizando com
        // os quadros
        countPeriod();
        // discretDynamicsWorld->stepSimulation ( countPeriod() );
        discretDynamicsWorld->stepSimulation ( 0.016667f );

    } else {
        discretDynamicsWorld->stepSimulation ( 0.016667f );
        s_primeiro = false;
    }

}

void
PhysicsControl::doTickCallBack ( btDynamicsWorld * world, btScalar timeStep ) {

    PhysicsControl *w =static_cast < PhysicsControl * > ( world->getWorldUserInfo() );
    w->processTickCallBack ( timeStep );

}

void PhysicsControl::processTickCallBack ( btScalar timeStep ) {

    // btCollisionObjectArray objects =
    // discretDynamicsWorld->getCollisionObjectArray();
    // discretDynamicsWorld->clearForces();
    // for (int i = 0; i < objects.size(); i++) {
    //
    // btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
    // if (!rigidBody) {
    // continue;
    // }
    //
    // rigidBody->applyGravity();
    // //rigidBody->applyForce(btVector3(-10., 0., 0.), btVector3(0.,
    // 0., 0.));
    // }
    // return;

}

void PhysicsControl::removeAllObjs() {
    // remove the rigidbodies from the dynamics world and delete them
    for ( int i = discretDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i-- ) {

        btCollisionObject *pObj =discretDynamicsWorld->getCollisionObjectArray() [i];
        btRigidBody *pBody = btRigidBody::upcast ( pObj );

        if ( pBody && pBody->getMotionState() )
            delete pBody->getMotionState();

        discretDynamicsWorld->removeCollisionObject ( pObj );
        delete pObj;

    }
}

void PhysicsControl::clearAllShapes() {
    // delete collision shapes
    // for (int j=0; j < m_collisionShapes.size(); j++)
    // {
    // btCollisionShape* pShape = m_collisionShapes[j];
    // m_collisionShapes[j] = 0;
    // delete pShape;
    // }
}

void PhysicsControl::checkCollisions() {

    std::map< btCollisionObject*, std::pair<Object*, Object*> > new_contacts;
    
    int numManifolds = discretDynamicsWorld->getDispatcher()->getNumManifolds();
    
    for ( int i=0; i<numManifolds; i++ ) {
        
        btPersistentManifold* contactManiFold = discretDynamicsWorld->getDispatcher()->getManifoldByIndexInternal ( i );
        
        btCollisionObject* objA = (btCollisionObject*)contactManiFold->getBody0();
        btCollisionObject* objB = (btCollisionObject*)contactManiFold->getBody1();

        int numContacts = contactManiFold->getNumContacts();
        for ( int j=0; j<numContacts; j++ ) {
            
            btManifoldPoint& pt = contactManiFold->getContactPoint ( j );
            
            if ( pt.getDistance() < 0.0f ) {
                
                if ( new_contacts.find ( objB ) == new_contacts.end() ) {
                    
                    Object* ptrObj = (Object*)objB->getUserPointer();//rigidbody contem o dado
                    
                    if ( ptrObj ) {
                        if ( ptrObj->get_check_collision() )
                            new_contacts[objB] = std::make_pair<Object*, Object*> ( static_cast<Object*> ( objB->getUserPointer() ),static_cast<Object*> ( objA->getUserPointer() ) );
                    }
                    
                }

                if ( new_contacts.find ( objA ) == new_contacts.end() ) {
                    
                     Object* ptrObj = (Object*)objA->getUserPointer();
                    
                    if ( ptrObj ) {
                        if ( ptrObj->get_check_collision() )
                            new_contacts[objA] = std::make_pair<Object*, Object*> ( static_cast<Object*> ( objA->getUserPointer() ),static_cast<Object*> ( objB->getUserPointer() ) );
                    }
                }
            }
        }
    }

    std::map< btCollisionObject*, std::pair<Object*, Object*> >::iterator it;
    if ( !new_contacts.empty() ) {
        
        for ( it = new_contacts.begin(); it != new_contacts.end(); it++ ) {
            if ( contactActives.find ( ( *it ).first ) == contactActives.end() ) {
                if ( ( *it ).second.first->get_check_collision() )
                    ( *it ).second.first->on_start_collision ( ( *it ).second.second );
            } else {
                if ( ( *it ).second.first->get_check_collision() )
                    ( *it ).second.first->on_collision ( ( *it ).second.second );
            }
        }
        
    }

    if ( !contactActives.empty() ) {
        for ( it = contactActives.begin(); it != contactActives.end(); it++ ) {
            if ( new_contacts.find ( ( *it ).first ) == new_contacts.end() ) {
                if ( ( *it ).second.first->get_check_collision() )
                    ( *it ).second.first->on_end_collision ( ( *it ).second.second );
            }
        }
    }

    contactActives = new_contacts;

}


}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; ;
