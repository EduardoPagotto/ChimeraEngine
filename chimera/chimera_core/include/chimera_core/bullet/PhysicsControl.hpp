#pragma once
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include "interfaces.hpp"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <btBulletCollisionCommon.h>
#include <map>

namespace ce {
class PhysicsControl : public ServiceBase<IPhysicsControl> {
  private:
    btBroadphaseInterface* broadPhase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* discretDynamicsWorld;
    std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> contactActives;

    bool checkAllowCollision(uint32_t* entity);
    static void doTickCallBack(btDynamicsWorld* world, btScalar timeStep);
    void processTickCallBack(btScalar timeStep);

  public:
    PhysicsControl();
    virtual ~PhysicsControl() override;
    virtual void clearAllShapes() override;
    virtual void removeAllObjs() override;
    virtual void stepSim(const double& ts) override;
    virtual void checkCollisions() override;
    virtual void setGravity(const btVector3& _vet) override { discretDynamicsWorld->setGravity(_vet); }
    virtual btDiscreteDynamicsWorld* getWorld() override { return discretDynamicsWorld; }
};
} // namespace ce
