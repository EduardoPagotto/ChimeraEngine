#include "chimera/core/bullet/PhysicsControl.hpp"
#include "chimera/base/event.hpp"

namespace ce {

    PhysicsControl::PhysicsControl() {

        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);

        btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

        broadPhase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;
        discretDynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfig);

        // true para forca aplicada apenas dentro docallback
        discretDynamicsWorld->setInternalTickCallback(PhysicsControl::doTickCallBack, static_cast<void*>(this), false);
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

    void PhysicsControl::stepSim(const double& ts) { discretDynamicsWorld->stepSimulation(ts); }

    void PhysicsControl::doTickCallBack(btDynamicsWorld* world, btScalar timeStep) {

        PhysicsControl* w = static_cast<PhysicsControl*>(world->getWorldUserInfo());
        w->processTickCallBack(timeStep);
    }

    void PhysicsControl::processTickCallBack(btScalar timeStep) {

        // btCollisionObjectArray objects = discretDynamicsWorld->getCollisionObjectArray();
        // discretDynamicsWorld->clearForces();
        // for (int i = 0; i < objects.size(); i++) {
        //     btRigidBody* rigidBody = btRigidBody::upcast(objects[i]);
        //     if (!rigidBody) {
        //         continue;
        //     }
        //     rigidBody->applyGravity();
        //     rigidBody->applyForce(btVector3(-10., 0., 0.), btVector3(0., 0., 0.));
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
        // // delete collision shapes
        // for (int j = 0; j < m_collisionShapes.size(); j++) {
        //     btCollisionShape* pShape = m_collisionShapes[j];
        //     m_collisionShapes[j] = 0;
        //     delete pShape;
        // }
    }

    bool PhysicsControl::checkAllowCollision(uint32_t* entity) { return true; }

    void PhysicsControl::checkCollisions() {

        std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> new_contacts;

        int numManifolds = discretDynamicsWorld->getDispatcher()->getNumManifolds();

        for (int i = 0; i < numManifolds; i++) {

            btPersistentManifold* contactManiFold =
                discretDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

            btCollisionObject* objA = (btCollisionObject*)contactManiFold->getBody0();
            btCollisionObject* objB = (btCollisionObject*)contactManiFold->getBody1();

            int numContacts = contactManiFold->getNumContacts();
            for (int j = 0; j < numContacts; j++) {

                if (btManifoldPoint& pt = contactManiFold->getContactPoint(j); pt.getDistance() < 0.0f) {

                    if (new_contacts.find(objB) == new_contacts.end()) {

                        uint32_t* entityB = (uint32_t*)objB->getUserPointer(); // rigidbody contem o dado
                        uint32_t* entityA = (uint32_t*)objA->getUserPointer(); // rigidbody contem o dado

                        if (entityB) {
                            if (checkAllowCollision(entityB) == true) {
                                new_contacts[objB] = std::pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entityA),
                                                                                     static_cast<uint32_t*>(entityB));
                            }
                            // new_contacts[objB] =
                            //     std::make_pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entityA),
                            //     static_cast<uint32_t*>(entityB));
                        }
                    }

                    if (new_contacts.find(objA) == new_contacts.end()) {

                        uint32_t* entityA = (uint32_t*)objA->getUserPointer(); // rigidbody contem o dado
                        uint32_t* entityB = (uint32_t*)objB->getUserPointer(); // rigidbody contem o dado

                        if (entityA) {
                            if (checkAllowCollision(entityA) == true) {
                                new_contacts[objA] = std::pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entityB),
                                                                                     static_cast<uint32_t*>(entityA));
                            }
                            // new_contacts[objA] =
                            //     std::make_pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entityB),
                            //     static_cast<uint32_t*>(entityA));
                        }
                    }
                }
            }
        }

        std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>>::iterator it;
        if (!new_contacts.empty()) {

            for (it = new_contacts.begin(); it != new_contacts.end(); it++) {
                if (contactActives.find((*it).first) == contactActives.end()) {

                    if (checkAllowCollision((*it).second.first) == true) {
                        utilSendEvent(EVENT_COLLIDE_START, (*it).second.first, (*it).second.second);
                    }

                } else {
                    // if (checkAllowCollision((*it).second.first) == true)
                    //     eventsSendCollision(KindOp::ON_COLLIDE, (*it).second.first, (*it).second.second);
                }
            }
        }

        if (!contactActives.empty()) {
            for (it = contactActives.begin(); it != contactActives.end(); it++) {
                if (new_contacts.find((*it).first) == new_contacts.end()) {

                    if (checkAllowCollision((*it).second.first) == true) {
                        utilSendEvent(EVENT_COLLIDE_OFF, (*it).second.first, (*it).second.second);
                    }
                }
            }
        }

        contactActives = new_contacts;
    }
} // namespace ce
