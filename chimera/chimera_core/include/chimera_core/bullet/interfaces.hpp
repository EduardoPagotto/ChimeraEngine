#pragma once
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

namespace ce {

    class IPhysicsControl {
      public:
        virtual ~IPhysicsControl() = default;
        virtual void clear_all_shapes(void) = 0;
        virtual void remove_all_objs(void) = 0;
        virtual void step_sim(const double& ts) = 0;
        virtual void check_collisions() = 0;
        virtual void set_gravity(const btVector3& _vet) = 0;
        virtual btDiscreteDynamicsWorld* get_world() = 0;
    };
} // namespace ce
