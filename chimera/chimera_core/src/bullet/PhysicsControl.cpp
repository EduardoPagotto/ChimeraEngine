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
        discret_dynamics_world_->setInternalTickCallback(PhysicsControl::do_tick_call_back, static_cast<void*>(this),
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

    void PhysicsControl::do_tick_call_back(btDynamicsWorld* world, btScalar time_step) {

        PhysicsControl* w = static_cast<PhysicsControl*>(world->getWorldUserInfo());
        w->process_tick_call_back(time_step);
    }

    void PhysicsControl::process_tick_call_back(btScalar time_step) {

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

            btCollisionObject* p_obj = discret_dynamics_world_->getCollisionObjectArray()[i];
            btRigidBody* p_body = btRigidBody::upcast(p_obj);

            if (p_body && p_body->getMotionState()) {
                delete p_body->getMotionState();
            }

            discret_dynamics_world_->removeCollisionObject(p_obj);
            delete p_obj;
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

    bool PhysicsControl::check_allow_collision(uint32_t* entity) { return true; }

    void PhysicsControl::check_collisions() {

        std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> new_contacts;

        int num_manifolds = discret_dynamics_world_->getDispatcher()->getNumManifolds();

        for (int i = 0; i < num_manifolds; i++) {

            btPersistentManifold* contact_mani_fold =
                discret_dynamics_world_->getDispatcher()->getManifoldByIndexInternal(i);

            btCollisionObject* obj_a = (btCollisionObject*)contact_mani_fold->getBody0();
            btCollisionObject* obj_b = (btCollisionObject*)contact_mani_fold->getBody1();

            int num_contacts = contact_mani_fold->getNumContacts();
            for (int j = 0; j < num_contacts; j++) {

                if (btManifoldPoint& pt = contact_mani_fold->getContactPoint(j); pt.getDistance() < 0.0f) {

                    if (new_contacts.find(obj_b) == new_contacts.end()) {

                        uint32_t* entity_b = (uint32_t*)obj_b->getUserPointer(); // rigidbody contem o dado
                        uint32_t* entity_a = (uint32_t*)obj_a->getUserPointer(); // rigidbody contem o dado

                        if (entity_b) {
                            if (check_allow_collision(entity_b) == true) {
                                new_contacts[obj_b] = std::pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entity_a),
                                                                                      static_cast<uint32_t*>(entity_b));
                            }
                            // new_contacts[objB] =
                            //     std::make_pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entityA),
                            //     static_cast<uint32_t*>(entityB));
                        }
                    }

                    if (new_contacts.find(obj_a) == new_contacts.end()) {

                        uint32_t* entity_a = (uint32_t*)obj_a->getUserPointer(); // rigidbody contem o dado
                        uint32_t* entity_b = (uint32_t*)obj_b->getUserPointer(); // rigidbody contem o dado

                        if (entity_a) {
                            if (check_allow_collision(entity_a) == true) {
                                new_contacts[obj_a] = std::pair<uint32_t*, uint32_t*>(static_cast<uint32_t*>(entity_b),
                                                                                      static_cast<uint32_t*>(entity_a));
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

                    if (check_allow_collision((*it).second.first) == true) {
                        send_chimera_event(EventCE::COLLIDE_START, (*it).second.first, (*it).second.second);
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

                    if (check_allow_collision((*it).second.first) == true) {
                        send_chimera_event(EventCE::COLLIDE_OFF, (*it).second.first, (*it).second.second);
                    }
                }
            }
        }

        contact_actives_ = new_contacts;
    }
} // namespace ce
