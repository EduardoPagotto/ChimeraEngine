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
        virtual void clearAllShapes() override;
        virtual void removeAllObjs() override;
        virtual void stepSim(const double& ts) override;
        virtual void checkCollisions() override;
        virtual void setGravity(const btVector3& _vet) override { discret_dynamics_world_->setGravity(_vet); }
        virtual btDiscreteDynamicsWorld* getWorld() override { return discret_dynamics_world_; }

      private:
        btBroadphaseInterface* broad_phase_;
        btDefaultCollisionConfiguration* collision_config_;
        btCollisionDispatcher* dispatcher_;
        btSequentialImpulseConstraintSolver* solver_;
        btDiscreteDynamicsWorld* discret_dynamics_world_;
        std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> contact_actives_;

        bool checkAllowCollision(uint32_t* entity);
        static void doTickCallBack(btDynamicsWorld* world, btScalar timeStep);
        void processTickCallBack(btScalar timeStep);
    };
} // namespace ce
