#pragma once
#include <cstdint>
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include "interfaces.hpp"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <btBulletCollisionCommon.h>
#include <map>

namespace ce {
    class PhysicsControl : public IPhysicsControl {

      public:
        PhysicsControl();
        virtual ~PhysicsControl() override;
        virtual void clear_all_shapes() override;
        virtual void remove_all_objs() override;
        virtual void step_sim(const double& ts) override;
        virtual void check_collisions() override;
        virtual void set_gravity(const btVector3& vet) override { discret_dynamics_world_->setGravity(vet); }
        virtual btDiscreteDynamicsWorld* get_world() override { return discret_dynamics_world_; }

      private:
        bool check_allow_collision(uint32_t* entity);
        static void do_tick_call_back(btDynamicsWorld* world, btScalar time_step);
        void process_tick_call_back(btScalar time_step);

        btBroadphaseInterface* broad_phase_;
        btDefaultCollisionConfiguration* collision_config_;
        btCollisionDispatcher* dispatcher_;
        btSequentialImpulseConstraintSolver* solver_;
        btDiscreteDynamicsWorld* discret_dynamics_world_;
        std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> contact_actives_;
    };
} // namespace ce
