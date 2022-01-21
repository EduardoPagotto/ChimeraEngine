#pragma once
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <LinearMath/btVector3.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <map>

namespace Chimera {

class PhysicsControl {
  public:
    PhysicsControl(void);
    virtual ~PhysicsControl(void);
    void clearAllShapes(void);
    void removeAllObjs(void);
    void stepSim(const double& ts);
    void checkCollisions();
    inline void setGravity(const btVector3& _vet) { discretDynamicsWorld->setGravity(_vet); }
    inline btDiscreteDynamicsWorld* getWorld() { return discretDynamicsWorld; }

  private:
    bool checkAllowCollision(uint32_t* entity);
    static void doTickCallBack(btDynamicsWorld* world, btScalar timeStep);
    void processTickCallBack(btScalar timeStep);

    btBroadphaseInterface* broadPhase;

    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;

    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* discretDynamicsWorld;

    std::map<btCollisionObject*, std::pair<uint32_t*, uint32_t*>> contactActives;
};
} // namespace Chimera
