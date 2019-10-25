#include "chimera/physic/PhysicsControl.hpp"
#include "chimera/core/utils.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

PhysicsControl::PhysicsControl() {

    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);

    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

    broadPhase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    discretDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfig);

    // true para forca aplicada apenas dentro docallback
    discretDynamicsWorld->setInternalTickCallback(PhysicsControl::doTickCallBack, static_cast<void*>(this), false);

    step = 0.016667f;
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

void PhysicsControl::stepSim(void) {

    static bool s_primeiro = true;

    if (s_primeiro == false) {

        countPeriod();
        // discretDynamicsWorld->stepSimulation ( countPeriod(), 50 );
        discretDynamicsWorld->stepSimulation(step);

    } else {
        discretDynamicsWorld->stepSimulation(step);
        s_primeiro = false;
    }
}

void PhysicsControl::doTickCallBack(btDynamicsWorld* world, btScalar timeStep) {

    PhysicsControl* w = static_cast<PhysicsControl*>(world->getWorldUserInfo());
    w->processTickCallBack(timeStep);
}

void PhysicsControl::processTickCallBack(btScalar timeStep) {

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
    for (int i = discretDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {

        btCollisionObject* pObj = discretDynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* pBody = btRigidBody::upcast(pObj);

        if (pBody && pBody->getMotionState()) {
            delete pBody->getMotionState();
        }

        discretDynamicsWorld->removeCollisionObject(pObj);
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

bool PhysicsControl::checkAllowCollision(Node* pNode) {

    //     DataMsg dataMsg ( KindOp::IS_ALLOW_COLLIDE, this, nullptr, nullptr );
    //     pNode->update ( &dataMsg );
    //
    //     return ( dataMsg.isDone() );
    return true;
}

void PhysicsControl::checkCollisions() {

    std::map<btCollisionObject*, std::pair<Node*, Node*>> new_contacts;

    int numManifolds = discretDynamicsWorld->getDispatcher()->getNumManifolds();

    for (int i = 0; i < numManifolds; i++) {

        btPersistentManifold* contactManiFold = discretDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

        btCollisionObject* objA = (btCollisionObject*)contactManiFold->getBody0();
        btCollisionObject* objB = (btCollisionObject*)contactManiFold->getBody1();

        int numContacts = contactManiFold->getNumContacts();
        for (int j = 0; j < numContacts; j++) {

            btManifoldPoint& pt = contactManiFold->getContactPoint(j);

            if (pt.getDistance() < 0.0f) {

                if (new_contacts.find(objB) == new_contacts.end()) {

                    Node* l_pNodeB = (Node*)objB->getUserPointer(); // rigidbody contem o dado
                    Node* l_pNodeA = (Node*)objA->getUserPointer(); // rigidbody contem o dado

                    if (l_pNodeB) {
                        if (checkAllowCollision(l_pNodeB) == true) {
                            new_contacts[objB] =
                                std::pair<Node*, Node*>(static_cast<Node*>(l_pNodeA), static_cast<Node*>(l_pNodeB));
                        }
                        // new_contacts[objB] = std::make_pair<Node*,
                        // Node*>(static_cast<Node*> (l_pNodeA), static_cast<Node*>
                        // (l_pNodeB));
                    }
                }

                if (new_contacts.find(objA) == new_contacts.end()) {

                    Node* l_pNodeA = (Node*)objA->getUserPointer(); // rigidbody contem o dado
                    Node* l_pNodeB = (Node*)objB->getUserPointer(); // rigidbody contem o dado

                    if (l_pNodeA) {
                        if (checkAllowCollision(l_pNodeA) == true) {
                            new_contacts[objA] =
                                std::pair<Node*, Node*>(static_cast<Node*>(l_pNodeB), static_cast<Node*>(l_pNodeA));
                        }
                        // new_contacts[objA] = std::make_pair<Node*, Node*> (
                        // static_cast<Node*> ( l_pNodeB ),static_cast<Node*> ( l_pNodeA )
                        // );
                    }
                }
            }
        }
    }

    std::map<btCollisionObject*, std::pair<Node*, Node*>>::iterator it;
    if (!new_contacts.empty()) {

        for (it = new_contacts.begin(); it != new_contacts.end(); it++) {
            if (contactActives.find((*it).first) == contactActives.end()) {

                if (checkAllowCollision((*it).second.first) == true) {
                    eventsSend(KindOp::START_COLLIDE, (*it).second.first, (*it).second.second);
                }

            } else {

                /*                if (checkAllowCollision( ( *it ).second.first ) == true)
                                                        eventsSendCollision(KindOp::ON_COLLIDE,
                   ( *it ).second.first, ( *it ).second.second);  */
            }
        }
    }

    if (!contactActives.empty()) {
        for (it = contactActives.begin(); it != contactActives.end(); it++) {
            if (new_contacts.find((*it).first) == new_contacts.end()) {

                if (checkAllowCollision((*it).second.first) == true) {
                    eventsSend(KindOp::OFF_COLLIDE, (*it).second.first, (*it).second.second);
                }
            }
        }
    }

    contactActives = new_contacts;
}
} // namespace Chimera
