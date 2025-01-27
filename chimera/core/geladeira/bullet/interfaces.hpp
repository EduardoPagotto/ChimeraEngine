#pragma once
#include "chimera/core/ServiceLocator.hpp"
#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

namespace Chimera {

class IPhysicsControl : public IService {
  public:
    virtual ~IPhysicsControl() = default;
    virtual void clearAllShapes(void) = 0;
    virtual void removeAllObjs(void) = 0;
    virtual void stepSim(const double& ts) = 0;
    virtual void checkCollisions() = 0;
    virtual void setGravity(const btVector3& _vet) = 0;
    virtual btDiscreteDynamicsWorld* getWorld() = 0;
};
} // namespace Chimera