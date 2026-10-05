#include "chimera_core/bullet/PhysicsControl.hpp"
#include "chimera_base/event.hpp"

namespace ce {

    PhysicsControl::PhysicsControl() {

        collision_config_ = new btDefaultCollisionConfiguration();
        dispatcher_ = new btCollisionDispatcher(collision_config_);

        btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher_);

        broad_phase_ = new btDbvtBroadphase();
        solver_ = new btSequentialImpulseConstraintSolver;
        discret_dynamics_world_ = new btDiscreteDynamicsWorld(dispatcher_, broad_phase_, solver_, collision_config_);

        // true para forca aplicada apenas dentro docallback
        discret_dynamics_world_->setInternalTickCallback(PhysicsControl::doTickCallBack, static_cast<void*>(this),
                                                         false);
    }

    PhysicsControl::~PhysicsControl() {
        remove_all_objs();
        clear_all_shapes();

        delete discret_dynamics_world_;
        delete solver_;
        delete collision_config_;
        delete dispatcher_;
        delete broad_phase_;
    }

    void PhysicsControl::step_sim(const double& ts) { discret_dynamics_world_->stepSimulation(ts); }

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

    void PhysicsControl::remove_all_objs() {
        // remove the rigidbodies from the dynamics world and delete them
        for (int i = discret_dynamics_world_->getNumCollisionObjects() - 1; i >= 0; i--) {

            btCollisionObject* pObj = discret_dynamics_world_->getCollisionObjectArray()[i];
            btRigidBody* pBody = btRigidBody::upcast(pObj);

            if (pBody && pBody->getMotionState()) {
                delete pBody->getMotionState();
            }

            discret_dynamics_world_->removeCollisionObject(pObj);
            delete pObj;
        }
    }

    void PhysicsControl::clear_all_shapes() {
        // // delete collision shapes
        // for (int j = 0; j < m_collisionShapes.size(); j++) {
        //     btCollisionShape* pShape = m_collisionShapes[j];
        //     m_collisionShapes[j] = 0;
        //     delete pShape;
        // }
    }

    bool PhysicsControl::checkAllowCollision(uint32_t* entity) { return true; }

    void PhysicsControl::check_collisions() {

        std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> new_contacts;

        int numManifolds = discret_dynamics_world_->getDispatcher()->getNumManifolds();

        for (int i = 0; i < numManifolds; i++) {

            btPersistentManifold* contactManiFold =
                discret_dynamics_world_->getDispatcher()->getManifoldByIndexInternal(i);

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
                if (contact_actives_.find((*it).first) == contact_actives_.end()) {

                    if (checkAllowCollision((*it).second.first) == true) {
                        sendChimeraEvent(EventCE::COLLIDE_START, (*it).second.first, (*it).second.second);
                    }

                } else {
                    // if (checkAllowCollision((*it).second.first) == true)
                    //     eventsSendCollision(KindOp::ON_COLLIDE, (*it).second.first, (*it).second.second);
                }
            }
        }

        if (!contact_actives_.empty()) {
            for (it = contact_actives_.begin(); it != contact_actives_.end(); it++) {
                if (new_contacts.find((*it).first) == new_contacts.end()) {

                    if (checkAllowCollision((*it).second.first) == true) {
                        sendChimeraEvent(EventCE::COLLIDE_OFF, (*it).second.first, (*it).second.second);
                    }
                }
            }
        }

        contact_actives_ = new_contacts;
    }
} // namespace ce
