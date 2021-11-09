#ifndef __CHIMERA_PHYSICSWORLD__HPP
#define __CHIMERA_PHYSICSWORLD__HPP

#include <map>

#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <LinearMath/btVector3.h>

#include "chimera/node/Node.hpp"

namespace Chimera {

class PhysicsControl {
  public:
    PhysicsControl(void);
    virtual ~PhysicsControl(void);

    void clearAllShapes(void);
    void removeAllObjs(void);

    void stepSim(const double& ts);

    void checkCollisions();
    bool checkAllowCollision(Node* pNode);

    inline void setGravity(const btVector3& _vet) { discretDynamicsWorld->setGravity(_vet); }

    inline btScalar countPeriod() {
        period = (btScalar)(clockCounter.getTimeMicroseconds() / 1000000.f);
        clockCounter.reset();
        return period;
    }

    inline btScalar getLastPeriod() { return period; }

    inline btDiscreteDynamicsWorld* getWorld() { return discretDynamicsWorld; }

  private:
    static void doTickCallBack(btDynamicsWorld* world, btScalar timeStep);
    void processTickCallBack(btScalar timeStep);

    btBroadphaseInterface* broadPhase;

    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;

    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* discretDynamicsWorld;

    /// <summary> evento usando na colisao de corpos se s_dealCollision for false
    /// </summary>
    // SDL_Event s_event;
    std::map<btCollisionObject*, std::pair<Node*, Node*>> contactActives;
    btClock clockCounter;
    btScalar period;
};
} // namespace Chimera
#endif